/* Copyright (c) 2005-2007, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Markus Gutschke
 */

#include "ELFCore.h"
#if defined DUMPER

#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/wait.h>

#include "CoreDumper.h"
#include "LinuxSyscallSupport.h"
#include "LinuxThreads.h"
#include "ThreadLister.h"

#ifndef CLONE_UNTRACED
#define CLONE_UNTRACED 0x00800000
#endif


/* Data structures found in x86-32/64 and ARM core dumps on Linux; similar
 * data structures are defined in /usr/include/{linux,asm}/... but those
 * headers conflict with the rest of the libc headers. So we cannot
 * include them here.
 */

#if defined(__i386__) || defined(__x86_64__)
  #if !defined(__x86_64__)
    typedef struct fpregs {     /* FPU registers                             */
      uint32_t  cwd;
      uint32_t  swd;
      uint32_t  twd;
      uint32_t  fip;
      uint32_t  fcs;
      uint32_t  foo;
      uint32_t  fos;
      uint32_t  st_space[20];   /* 8*10 bytes for each FP-reg = 80 bytes     */
    } fpregs;
    typedef struct fpxregs {    /* SSE registers                             */
    #define FPREGS fpxregs
  #else
    typedef struct fpxregs {    /* x86-64 stores FPU registers in SSE struct */
    } fpxregs;
    typedef struct fpregs {     /* FPU registers                             */
    #define FPREGS fpregs
  #endif
    uint16_t  cwd;
    uint16_t  swd;
    uint16_t  twd;
    uint16_t  fop;
    uint32_t  fip;
    uint32_t  fcs;
    uint32_t  foo;
    uint32_t  fos;
    uint32_t  mxcsr;
    uint32_t  mxcsr_mask;
    uint32_t  st_space[32];     /*  8*16 bytes for each FP-reg  = 128 bytes  */
    uint32_t  xmm_space[64];    /* 16*16 bytes for each XMM-reg = 128 bytes  */
    uint32_t  padding[24];
  } FPREGS;
  #undef FPREGS
  #define regs i386_regs        /* General purpose registers                 */
#elif defined(__ARM_ARCH_3__)
  typedef struct fpxregs {      /* No extended FPU registers on ARM          */
  } fpxregs;
  typedef struct fpregs {       /* FPU registers                             */
    struct fp_reg {
      unsigned int sign1:1;
      unsigned int unused:15;
      unsigned int sign2:1;
      unsigned int exponent:14;
      unsigned int j:1;
      unsigned int mantissa1:31;
      unsigned int mantissa0:32;
    } fpregs[8];
    unsigned int   fpsr:32;
    unsigned int   fpcr:32;
    unsigned char  ftype[8];
    unsigned int   init_flag;
  } fpregs;
  #define regs arm_regs         /* General purpose registers                 */
#endif

typedef struct elf_timeval {    /* Time value with microsecond resolution    */
  long tv_sec;                  /* Seconds                                   */
  long tv_usec;                 /* Microseconds                              */
} elf_timeval;


typedef struct elf_siginfo {    /* Information about signal (unused)         */
  int32_t si_signo;             /* Signal number                             */
  int32_t si_code;              /* Extra code                                */
  int32_t si_errno;             /* Errno                                     */
} elf_siginfo;


typedef struct prstatus {       /* Information about thread; includes CPU reg*/
  elf_siginfo    pr_info;       /* Info associated with signal               */
  uint16_t       pr_cursig;     /* Current signal                            */
  unsigned long  pr_sigpend;    /* Set of pending signals                    */
  unsigned long  pr_sighold;    /* Set of held signals                       */
  pid_t          pr_pid;        /* Process ID                                */
  pid_t          pr_ppid;       /* Parent's process ID                       */
  pid_t          pr_pgrp;       /* Group ID                                  */
  pid_t          pr_sid;        /* Session ID                                */
  elf_timeval    pr_utime;      /* User time                                 */
  elf_timeval    pr_stime;      /* System time                               */
  elf_timeval    pr_cutime;     /* Cumulative user time                      */
  elf_timeval    pr_cstime;     /* Cumulative system time                    */
  regs           pr_reg;        /* CPU registers                             */
  uint32_t       pr_fpvalid;    /* True if math co-processor being used      */
} prstatus;


typedef struct prpsinfo {       /* Information about process                 */
  unsigned char  pr_state;      /* Numeric process state                     */
  char           pr_sname;      /* Char for pr_state                         */
  unsigned char  pr_zomb;       /* Zombie                                    */
  signed char    pr_nice;       /* Nice val                                  */
  unsigned long  pr_flag;       /* Flags                                     */
#ifdef __x86_64__
  uint32_t       pr_uid;        /* User ID                                   */
  uint32_t       pr_gid;        /* Group ID                                  */
#else
  uint16_t       pr_uid;        /* User ID                                   */
  uint16_t       pr_gid;        /* Group ID                                  */
#endif
  pid_t          pr_pid;        /* Process ID                                */
  pid_t          pr_ppid;       /* Parent's process ID                       */
  pid_t          pr_pgrp;       /* Group ID                                  */
  pid_t          pr_sid;        /* Session ID                                */
  char           pr_fname[16];  /* Filename of executable                    */
  char           pr_psargs[80]; /* Initial part of arg list                  */
} prpsinfo;


typedef struct user {           /* Ptrace returns this data for thread state */
  regs           regs;          /* CPU registers                             */
  unsigned long  fpvalid;       /* True if math co-processor being used      */
#if defined(__i386__) || defined(__x86_64__)
  fpregs         fpregs;        /* FPU registers                             */
#endif
  unsigned long  tsize;         /* Text segment size in pages                */
  unsigned long  dsize;         /* Data segment size in pages                */
  unsigned long  ssize;         /* Stack segment size in pages               */
  unsigned long  start_code;    /* Starting virtual address of text          */
  unsigned long  start_stack;   /* Starting virtual address of stack area    */
  unsigned long  signal;        /* Signal that caused the core dump          */
  unsigned long  reserved;      /* No longer used                            */
  regs           *regs_ptr;     /* Used by gdb to help find the CPU registers*/
#if defined(__i386__) || defined(__x86_64__)
  fpregs         *fpregs_ptr;   /* Pointer to FPU registers                  */
#endif
  unsigned long  magic;         /* Magic for old A.OUT core files            */
  char           comm[32];      /* User command that was responsible         */
  unsigned long  debugreg[8];
#if defined(__i386__) || defined(__x86_64__)
  unsigned long  error_code;    /* CPU error code or 0                       */
  unsigned long  fault_address; /* CR3 or 0                                  */
#elif defined(__ARM_ARCH_3__)
  fpregs         fpregs;        /* FPU registers                             */
  fpregs         *fpregs_ptr;   /* Pointer to FPU registers                  */
#endif
} user;


#if __WORDSIZE == 64
  #define ELF_CLASS ELFCLASS64
  #define Ehdr      Elf64_Ehdr
  #define Phdr      Elf64_Phdr
  #define Shdr      Elf64_Shdr
  #define Nhdr      Elf64_Nhdr
#else
  #define ELF_CLASS ELFCLASS32
  #define Ehdr      Elf32_Ehdr
  #define Phdr      Elf32_Phdr
  #define Shdr      Elf32_Shdr
  #define Nhdr      Elf32_Nhdr
#endif


#if defined(__x86_64__)
  #define ELF_ARCH  EM_X86_64
#elif defined(__i386__)
  #define ELF_ARCH  EM_386
#elif defined(__ARM_ARCH_3__)
  #define ELF_ARCH  EM_ARM
#endif


/* Re-runs fn until it doesn't cause EINTR
 */
#define NO_INTR(fn)    do {} while ((fn) < 0 && errno == EINTR)
#define MY_NO_INTR(fn) do {} while ((fn) < 0 && my_errno == EINTR)


/* Wrapper for read() which is guaranteed to never return EINTR.
 */
static ssize_t c_read(int f, void *buf, size_t bytes, int *errno_) {
  int my_errno;
  /* scope */ {
    /* Define a private copy of syscall macros, which does not modify the
     * global copy of errno.
     */
    #define SYS_ERRNO my_errno
    #define SYS_INLINE inline
    #undef  SYS_LINUX_SYSCALL_SUPPORT_H
    #define SYS_PREFIX 0
    #include "LinuxSyscallSupport.h"
  
    if (bytes > 0) {
      ssize_t rc;
      MY_NO_INTR(rc = sys0_read(f, buf, bytes));
      if (rc < 0) {
        *errno_ = my_errno;
      }
      return rc;
    }
    return 0;
  }
}


/* Wrapper for write() which is guaranteed to never return EINTR nor
 * short writes.
 */
static ssize_t c_write(int f, const void *void_buf, size_t bytes, int *errno_){
  int my_errno;
  /* scope */ {
    /* Define a private copy of syscall macros, which does not modify the
     * global copy of errno.
     */
    #define SYS_ERRNO my_errno
    #define SYS_INLINE inline
    #undef  SYS_LINUX_SYSCALL_SUPPORT_H
    #define SYS_PREFIX 0
    #include "LinuxSyscallSupport.h"
  
    const unsigned char *buf = (const unsigned char*)void_buf;
    size_t len = bytes;
    while (len > 0) {
      ssize_t rc;
      MY_NO_INTR(rc = sys0_write(f, buf, len));
      if (rc < 0) {
        *errno_ = my_errno;
        return rc;
      } else if (rc == 0) {
        break;
      }
      buf += rc;
      len -= rc;
    }
    return bytes - len;
  }
}


/* The simple synchronous writer is only used when outputting to a pipe
 * instead of a file. In that case, we do not enforce a pre-determined
 * maximum output size.
 */
static int SimpleDone(void *f) {
  return 0;
}


/* Simple synchronous writer function used by CreateElfCore() when writing
 * directly to a pipe.
 */
static ssize_t SimpleWriter(void *f, const void *void_buf, size_t bytes) {
  return c_write(*(int *)f, void_buf, bytes, &errno);
}


struct WriterFds {
  size_t max_length;
  int    write_fd;
  int    compressed_fd;
  int    out_fd;
};


/* Checks whether the maximum number of allowed bytes has been written
 * to the output file already.
 */
static int PipeDone(void *f) {
  struct WriterFds *fds = (struct WriterFds *)f;
  return fds->max_length == 0;
}


/* Writer function that writes directly to a file and honors size limits.
 */
static ssize_t LimitWriter(void *f, const void *void_buf, size_t bytes) {
  struct WriterFds *fds = (struct WriterFds *)f;
  ssize_t rc;
  if (bytes > fds->max_length) {
    bytes = fds->max_length;
  }
  rc = c_write(fds->out_fd, void_buf, bytes, &errno);
  if (rc > 0) {
    fds->max_length -= rc;
  }
  return rc;
}


/* Writer function that can handle writing to one end of a compression
 * pipeline, reading from the other end of the pipe as compressed data
 * becomes available, and finally outputting it to a file.
 */
static ssize_t PipeWriter(void *f, const void *void_buf, size_t bytes) {
  const unsigned char *buf = (const unsigned char *)void_buf;
  struct WriterFds *fds    = (struct WriterFds *)f;
  size_t len               = bytes;
  while (fds->max_length > 0 && len > 0) {
    ssize_t rc;
    struct pollfd pfd[2]   = { { fds->compressed_fd, POLLIN, 0 },
                               { fds->write_fd, POLLOUT, 0 } };
    int nfds = sys_poll(pfd, 2, -1);
    
    if (nfds < 0) {
      /* Abort on fatal unexpected I/O errors.                               */
      break;
    }

    if (nfds > 0 && (pfd[0].revents & POLLIN)) {
      /* Some compressed data has become available. Copy to output file.     */
      char scratch[4096];
      for (;;) {
        size_t l = sizeof(scratch);
        if (l > fds->max_length) {
          l = fds->max_length;
        }
        NO_INTR(rc = read(fds->compressed_fd, scratch, l));
        if (rc < 0) {
          /* The file handle is set to be non-blocking, so we loop until
           * read() returns -1.
           */
          if (errno == EAGAIN) {
            break;
          }
          return -1;
        } else if (rc == 0) {
          fds->max_length = 0;
          break;
        }
        rc = c_write(fds->out_fd, scratch, rc, &errno);
        if (rc <= 0) {
          return -1;
        }
        fds->max_length -= rc;
      }
      nfds--;
    }
    if (nfds > 0 && (pfd[1].revents & POLLOUT)) {
      /* The compressor has consumed all previous data and is ready to
       * receive more.
       */
      NO_INTR(rc = write(fds->write_fd, buf, len));
      if (rc < 0 && errno != EAGAIN) {
        return -1;
      }
      buf += rc;
      len -= rc;
    }
  }
  return bytes - len;
}


/* Flush the remaining data (if any) from the pipe.
 */
static int FlushPipe(struct WriterFds *fds) {
  long flags;
  NO_INTR(flags = sys_fcntl(fds->compressed_fd, F_GETFL, 0));
  NO_INTR(sys_fcntl(fds->compressed_fd, F_SETFL, flags & ~O_NONBLOCK));
  while (fds->max_length > 0) {
    char    scratch[4096];
    size_t  l = sizeof(scratch);
    ssize_t rc;
    if (l > fds->max_length) {
      l = fds->max_length;
    }
    if (l > 0) {
      NO_INTR(rc = read(fds->compressed_fd, scratch, l));
      if (rc < 0) {
        return -1;
      } else if (rc == 0) {
        break;
      }
      if (c_write(fds->out_fd, scratch, rc, &errno) != rc) {
        return -1;
      }
      fds->max_length -= rc;
    }
  }
  return 0;
}

struct io {
  int fd;
  unsigned char *data, *end;
  unsigned char buf[4096];
};


/* Reads one character from the "io" file. This function has the same
 * semantics as fgetc(), but we cannot call any library functions at this
 * time.
 */
static int GetChar(struct io *io) {
  unsigned char *ptr = io->data;
  if (ptr == io->end) {
    /* Even though we are parsing one character at a time, read in larger
     * chunks.
     */
    ssize_t n = c_read(io->fd, io->buf, sizeof(io->buf), &errno);
    if (n <= 0) {
      if (n == 0)
        errno = 0;
      return -1;
    }
    ptr = &io->buf[0];
    io->end = &io->buf[n];
  }
  io->data = ptr+1;
  return *ptr;
}


/* Place the hex number read from "io" into "*hex".  The first non-hex
 * character is returned (or -1 in the case of end-of-file).
 */
static int GetHex(struct io *io, size_t *hex) {
  int ch;
  *hex = 0;
  while (((ch = GetChar(io)) >= '0' && ch <= '9') ||
         (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
    *hex = (*hex << 4) | (ch < 'A' ? ch - '0' : (ch & 0xF) + 9);
  return ch;
}


/* Computes the amount of leading zeros in a memory region.
 */
static size_t LeadingZeros(int *loopback, void *mem, size_t len,
                           size_t pagesize) {
  char   buf[pagesize];
  size_t count;

  char *ptr = buf;
  for (count = 0; count < len; ) {
    /* Read a page by going through the pipe. Assume that we can write at
     * least one page without blocking.
     *
     * "Normal" kernels do not require this hack. But some of the security
     * patches (e.g. grsec) can be configured to disallow read access of
     * executable pages. So, directly scanning the memory range would
     * result in a segmentation fault.
     *
     * If we cannot access a page, we assume that it was all zeros.
     */
    if ((count % pagesize) == 0) {
      if (c_write(loopback[1], (char *)mem + count, pagesize, &errno) < 0 ||
          c_read(loopback[0],  buf,                 pagesize, &errno) < 0) {
        count += pagesize;
        continue;
      } else {
        ptr = buf;
      }
    }
    if (*ptr++) {
      break;
    }
    count++;
  }
  return count & ~(pagesize-1);
}


/* Dynamically determines the byte sex of the system. Returns non-zero
 * for big-endian machines.
 */
static inline int sex() {
  int probe = 1;
  return !*(char *)&probe;
}


/* This function is invoked from a seperate process. It has access to a
 * copy-on-write copy of the parents address space, and all crucial
 * information about the parent has been computed by the caller.
 */
static int CreateElfCore(void *handle,
                         ssize_t (*writer)(void *, const void *, size_t),
                         int (*is_done)(void *), prpsinfo *prpsinfo,
                         user *user, prstatus *prstatus, int num_threads,
                         pid_t *pids, regs *regs, fpregs *fpregs,
                         fpxregs *fpxregs, size_t pagesize) {
  /* Count the number of mappings in "/proc/self/maps". We are guaranteed
   * that this number is not going to change while this function executes.
   */
  int       rc = -1, num_mappings = 0;
  struct io io;
  int       loopback[2] = { -1, -1 };

  if (sys_pipe(loopback) < 0)
    goto done;

  io.data = io.end = 0;
  NO_INTR(io.fd = sys_open("/proc/self/maps", O_RDONLY, 0));
  if (io.fd >= 0) {
    int i, ch;
    while ((ch = GetChar(&io)) >= 0) {
      num_mappings += (ch == '\n');
    }
    if (errno != 0) {
   read_error:
      NO_INTR(sys_close(io.fd));
      goto done;
    }
    NO_INTR(sys_close(io.fd));

    /* Read all mappings. This requires re-opening "/proc/self/maps"         */
    /* scope */ {
      static const int PF_ANONYMOUS = 0x80000000;
      static const int PF_MASK      = 0x00000007;
      struct {
        size_t start_address, end_address, offset;
        int   flags;
      } mappings[num_mappings];
      io.data = io.end = 0;
      NO_INTR(io.fd = sys_open("/proc/self/maps", O_RDONLY, 0));
      if (io.fd >= 0) {
        size_t note_align;
        /* Parse entries of the form:
         * "^[0-9A-F]*-[0-9A-F]* [r-][w-][x-][p-] [0-9A-F]*.*$"
         */
        for (i = 0; i < num_mappings;) {
          static const char * const dev_zero = "/dev/zero";
          const char *dev = dev_zero;
          int    j, is_device, is_anonymous;
          size_t zeros;

          memset(&mappings[i], 0, sizeof(mappings[i]));

          /* Read start and end addresses                                    */
          if (GetHex(&io, &mappings[i].start_address) != '-' ||
              GetHex(&io, &mappings[i].end_address)   != ' ')
            goto read_error;

          /* Read flags                                                      */
          while ((ch = GetChar(&io)) != ' ') {
            if (ch < 0)
              goto read_error;
            mappings[i].flags = (mappings[i].flags << 1) | (ch != '-');
          }

          /* Read offset                                                     */
          if ((ch = GetHex(&io, &mappings[i].offset)) != ' ')
            goto read_error;

          /* Skip over device numbers, and inode number                      */
          for (j = 0; j < 2; j++) {
            while (ch == ' ') {
              ch = GetChar(&io);
            }
            while (ch != ' ' && ch != '\n') {
              if (ch < 0)
                goto read_error;
              ch = GetChar(&io);
            }
            while (ch == ' ') {
              ch = GetChar(&io);
            }
            if (ch < 0)
              goto read_error;
          }

          /* Check whether this is a mapping for a device                    */
          is_anonymous = (ch == '\n' || ch == '[');
          while (*dev && ch == *dev) {
            ch = GetChar(&io);
            dev++;
          }
          is_device = dev >= dev_zero + 5 &&
                      ((ch != '\n' && ch != ' ') || *dev != '\000');

          /* Drop the private/shared bit. This makes the flags compatible with
           * the ELF access bits
           */
          mappings[i].flags    = (mappings[i].flags >> 1) & PF_MASK;
          if (is_anonymous) {
            mappings[i].flags |= PF_ANONYMOUS;
          }

          /* Skip until end of line                                          */
          while (ch != '\n') {
            if (ch < 0)
              goto read_error;
            ch = GetChar(&io);
          }

          /* Skip leading zeroed pages (as found in the stack segment)       */
          if ((mappings[i].flags & PF_R) && !is_device) {
            zeros = LeadingZeros(loopback, (void *)mappings[i].start_address,
                         mappings[i].end_address - mappings[i].start_address,
                         pagesize);
            mappings[i].start_address += zeros;
          }

          /* Remove mapping, if it was not readable, or completely zero
           * anyway. The former is usually the case of stack guard pages, and
           * the latter occasionally happens for unused memory.
           * Also, be careful not to touch mapped devices.
           */
          if ((mappings[i].flags & PF_R) == 0 ||
              mappings[i].start_address == mappings[i].end_address ||
              is_device) {
            num_mappings--;
          } else {
            i++;
          }
        }
        NO_INTR(sys_close(io.fd));

        /* Write out the ELF header                                          */
        /* scope */ {
          Ehdr ehdr;
          memset(&ehdr, 0, sizeof(Ehdr));
          ehdr.e_ident[0] = ELFMAG0;
          ehdr.e_ident[1] = ELFMAG1;
          ehdr.e_ident[2] = ELFMAG2;
          ehdr.e_ident[3] = ELFMAG3;
          ehdr.e_ident[4] = ELF_CLASS;
          ehdr.e_ident[5] = sex() ? ELFDATA2MSB : ELFDATA2LSB;
          ehdr.e_ident[6] = EV_CURRENT;
          ehdr.e_type     = ET_CORE;
          ehdr.e_machine  = ELF_ARCH;
          ehdr.e_version  = EV_CURRENT;
          ehdr.e_phoff    = sizeof(Ehdr);
          ehdr.e_ehsize   = sizeof(Ehdr);
          ehdr.e_phentsize= sizeof(Phdr);
          ehdr.e_phnum    = num_mappings + 1;
          ehdr.e_shentsize= sizeof(Shdr);
          if (writer(handle, &ehdr, sizeof(Ehdr)) != sizeof(Ehdr)) {
            goto done;
          }
        }

        /* Write program headers, starting with the PT_NOTE entry            */
        /* scope */ {
          Phdr   phdr;
          size_t offset   = sizeof(Ehdr) + (num_mappings + 1)*sizeof(Phdr);
          size_t filesz   = sizeof(Nhdr) + 4 + sizeof(struct prpsinfo) +
                            sizeof(Nhdr) + 4 + sizeof(struct user) +
                            num_threads*(
                            + sizeof(Nhdr) + 4 + sizeof(struct prstatus)
                            + sizeof(Nhdr) + 4 + sizeof(struct fpregs));
          #if defined(__i386__) && !defined(__x86_64__)
          if (fpxregs) {
            filesz       += num_threads*(
                             sizeof(Nhdr) + 8 + sizeof(struct fpxregs));
          }
          #endif
          memset(&phdr, 0, sizeof(Phdr));
          phdr.p_type     = PT_NOTE;
          phdr.p_offset   = offset;
          phdr.p_filesz   = filesz;
          if (writer(handle, &phdr, sizeof(Phdr)) != sizeof(Phdr)) {
            goto done;
          }

          /* Now follow with program headers for each of the memory segments */
          phdr.p_type     = PT_LOAD;
          phdr.p_align    = pagesize;
          phdr.p_paddr    = 0;
          note_align      = phdr.p_align - ((offset+filesz) % phdr.p_align);
          if (note_align == phdr.p_align)
            note_align    = 0;
          offset         += note_align;
          for (i = 0; i < num_mappings; i++) {
            offset       += filesz;
            filesz        = mappings[i].end_address -mappings[i].start_address;
            phdr.p_offset = offset;
            phdr.p_vaddr  = mappings[i].start_address;
            phdr.p_memsz  = filesz;

            /* Do not write contents for memory segments that are read-only  */
            if ((mappings[i].flags & (PF_ANONYMOUS|PF_W)) == 0) {
              filesz      = 0;
            }
            phdr.p_filesz = filesz;
            phdr.p_flags  = mappings[i].flags & PF_MASK;
            if (writer(handle, &phdr, sizeof(Phdr)) != sizeof(Phdr)) {
              goto done;
            }
          }
        }

        /* Write note section                                                */
        /* scope */ {
          Nhdr nhdr;
          memset(&nhdr, 0, sizeof(Nhdr));
          nhdr.n_namesz   = 4;
          nhdr.n_descsz   = sizeof(struct prpsinfo);
          nhdr.n_type     = NT_PRPSINFO;
          if (writer(handle, &nhdr, sizeof(Nhdr)) != sizeof(Nhdr) ||
              writer(handle, "CORE", 4) != 4 ||
              writer(handle, prpsinfo, sizeof(struct prpsinfo)) !=
              sizeof(struct prpsinfo)) {
            goto done;
          }
          nhdr.n_descsz   = sizeof(struct user);
          nhdr.n_type     = NT_PRXREG;
          if (writer(handle, &nhdr, sizeof(Nhdr)) != sizeof(Nhdr) ||
              writer(handle, "CORE", 4) != 4 ||
              writer(handle, user, sizeof(struct user)) !=sizeof(struct user)){
            goto done;
          }

          for (i = num_threads; i-- > 0; ) {
            /* Process status and integer registers                          */
            nhdr.n_descsz = sizeof(struct prstatus);
            nhdr.n_type   = NT_PRSTATUS;
            prstatus->pr_pid = pids[i];
            prstatus->pr_reg = regs[i];
            if (writer(handle, &nhdr, sizeof(Nhdr)) != sizeof(Nhdr) ||
                writer(handle, "CORE", 4) != 4 ||
                writer(handle, prstatus, sizeof(struct prstatus)) !=
                sizeof(struct prstatus)) {
              goto done;
            }

            /* FPU registers                                                 */
            nhdr.n_descsz = sizeof(struct fpregs);
            nhdr.n_type   = NT_FPREGSET;
            if (writer(handle, &nhdr, sizeof(Nhdr)) != sizeof(Nhdr) ||
                writer(handle, "CORE", 4) != 4 ||
                writer(handle, fpregs+1, sizeof(struct fpregs)) !=
                sizeof(struct fpregs)) {
              goto done;
            }

            /* SSE registers                                                 */
            #if defined(__i386__) && !defined( __x86_64__)
            /* Linux on x86-64 stores all FPU registers in the SSE structure */
            if (fpxregs) {
              nhdr.n_namesz = 8;
              nhdr.n_descsz = sizeof(struct fpxregs);
              nhdr.n_type   = NT_PRXFPREG;
              if (writer(handle, &nhdr, sizeof(Nhdr)) != sizeof(Nhdr) ||
                  writer(handle, "LINUX\000\000", 8) != 8 ||
                  writer(handle, fpxregs+1, sizeof(struct fpxregs)) !=
                  sizeof(struct fpxregs)) {
                goto done;
              }
              nhdr.n_namesz = 4;
            }
            #endif
          }
        }

        /* Align all following segments to multiples of page size            */
        if (note_align) {
          char scratch[note_align];
          memset(scratch, 0, sizeof(scratch));
          if (writer(handle, scratch, sizeof(scratch)) != sizeof(scratch)) {
            goto done;
          }
        }

        /* Write all memory segments                                         */
        for (i = 0; i < num_mappings; i++) {
          if (mappings[i].flags & (PF_ANONYMOUS|PF_W) &&
              writer(handle, (void *)mappings[i].start_address,
                     mappings[i].end_address - mappings[i].start_address) !=
                     mappings[i].end_address - mappings[i].start_address) {
            goto done;
          }
        }

        rc = 0;
      }
    }
  }

done:
  if (is_done(handle)) {
    rc = 0;
  }

  if (loopback[0] >= 0)
    NO_INTR(sys_close(loopback[0]));
  if (loopback[1] >= 0)
    NO_INTR(sys_close(loopback[1]));
  return rc;
}


struct CreateArgs {
  int *fds;
  int openmax;
  const char *PATH;
  const struct CoredumperCompressor* compressors;
  int zip_in[2];
  int zip_out[2];
};

static int CreatePipelineChild(void *void_arg) {
  int my_errno;
  /* scope */ {
    /* Define a private copy of syscall macros, which does not modify the
     * global copy of errno.
     */
    #define SYS_ERRNO my_errno
    #define SYS_INLINE inline
    #undef  SYS_LINUX_SYSCALL_SUPPORT_H
    #define SYS_PREFIX 0
    #include "LinuxSyscallSupport.h"
  
    struct CreateArgs *args = (struct CreateArgs *)void_arg;
    int i;
    
    /* Use pipe to tell parent about the compressor that we chose.
     * Make sure the file handle for the write-end of the pipe is
     * bigger than 2, so that it does not interfere with the
     * stdin/stdout/stderr file handles which must be 0-2.
     */
    MY_NO_INTR(sys0_close(args->fds[0]));
    while (args->fds[1] <= 2) {
      MY_NO_INTR(args->fds[1] = sys0_dup(args->fds[1]));
    }
    sys0_fcntl(args->fds[1], F_SETFD, FD_CLOEXEC);
    
    /* Move the filehandles for stdin/stdout/stderr, so that they
     * map to handles 0-2. stdin/stdout are connected to pipes, and
     * stderr points to "/dev/null".
     */
    while (args->zip_in[0]  <= 2) {
      MY_NO_INTR(args->zip_in[0]  = sys0_dup(args->zip_in[0]));
    }
    while (args->zip_out[1] <= 2) {
      MY_NO_INTR(args->zip_out[1] = sys0_dup(args->zip_out[1]));
    }
    MY_NO_INTR(sys0_dup2(args->zip_in[0],  0));
    MY_NO_INTR(sys0_dup2(args->zip_out[1], 1));
    MY_NO_INTR(sys0_close(2));
    MY_NO_INTR(sys0_dup2(sys0_open("/dev/null", O_WRONLY, 0), 2));
  
    /* Close all handles other than stdin/stdout/stderr and the
     * pipe to the parent. This also takes care of all the filehandles
     * that we temporarily created by calling sys_dup().
     */
    for (i = 3; i < args->openmax; i++)
      if (i != args->fds[1])
        MY_NO_INTR(sys0_close(i));
  
    while (args->compressors->compressor != NULL &&
           *args->compressors->compressor) {
      extern char **environ;
      
      const char        *compressor = args->compressors->compressor;
      const char *const *cmd_args   = args->compressors->args;
      
      /* Try next compressor description. If the compressor exists,
       * the fds[1] file handle will get closed on exec(). The
       * parent detects this, and eventually updates
       * selected_compressor with the compressor that is now running.
       *
       * Please note, the caller does not need to call wait() for any
       * compressor that gets launched, because our parent process is
       * going to die soon; thus, the compressor will be reaped by "init".
       */
      c_write(args->fds[1], &args->compressors, sizeof(&args->compressors),
              &my_errno);
      if (strchr(compressor, '/')) {
        /* Absolute or relative path precedes name of executable             */
        sys0_execve(compressor, cmd_args, (const char * const *)environ);
      } else {
        /* Search for executable along PATH variable                         */
        const char *ptr = args->PATH;
        if (ptr != NULL) {
          for (;;) {
            const char *end = ptr;
            while (*end && *end != ':') end++;
            if (ptr == end) {
              /* Found current directory in PATH                             */
              sys0_execve(compressor, cmd_args, (const char * const *)environ);
            } else {
              /* Compute new file name                                       */
              char executable[strlen(compressor) + (end - ptr) + 2];
              memcpy(executable, ptr, end-ptr);
              executable[end - ptr] = '/';
              strcpy(executable + (end - ptr + 1), compressor);
              sys0_execve(executable, cmd_args, (const char * const *)environ);
            }
            if (!*end)
              break;
            ptr = end + 1;
          }
        }
      }
      ++args->compressors;
    }
  
    /* No suitable compressor found. Tell parent about it.                   */
    c_write(args->fds[1], &args->compressors, sizeof(&args->compressors),
            &my_errno);
    MY_NO_INTR(sys0_close(args->fds[1]));
    sys__exit(0);
    return 0;
  }
}


/* Create a pipeline for sending the core file from the child process back to
 * the caller. Optionally include a compressor program in the loop. The
 * "compressors" variable will be updated to point to the compressor that was
 * actually used.
 */
static int CreatePipeline(int *fds, int openmax, const char *PATH,
                          const struct CoredumperCompressor** compressors) {
  /* Create a pipe for communicating between processes                       */
  if (sys_pipe(fds) < 0)
    return -1;

  /* Find a suitable compressor program, if necessary                        */
  if (*compressors != NULL && (*compressors)->compressor != NULL) {
    char stack[4096];
    struct CreateArgs args;
    pid_t comp_pid;

    args.fds         = fds;
    args.openmax     = openmax;
    args.PATH        = PATH;
    args.compressors = *compressors;

    if (sys_pipe(args.zip_in) < 0) {
   fail0: {
        int saved_errno = errno;
        NO_INTR(sys_close(fds[0]));
        NO_INTR(sys_close(fds[1]));
        errno = saved_errno;
        return -1;
      }
    } else if (sys_pipe(args.zip_out) < 0) {
   fail1: {
        int saved_errno = errno;
        NO_INTR(sys_close(args.zip_in[0]));
        NO_INTR(sys_close(args.zip_in[1]));
        errno = saved_errno;
        goto fail0;
      }
    }

    /* We use clone() here, instead of the more common fork(). This ensures
     * that the WriteCoreDump() code path never results in making a COW
     * instance of the processes' address space. This increases the likelihood
     * that we can dump core files even if we are using a lot of memory and
     * the kernel disallows overcomitting of memory.
     * After cloning, both the parent and the child share the same instance
     * of errno. We must make sure that at least one of these processes
     * (in our case, the child) uses modified syscall macros that update
     * a local copy of errno, instead.
     */
    comp_pid = sys_clone(CreatePipelineChild, stack + sizeof(stack) - 16,
                         CLONE_VM|CLONE_UNTRACED|SIGCHLD, &args, 0, 0, 0);
    if (comp_pid < 0) {
      int clone_errno = errno;
      NO_INTR(sys_close(args.zip_out[0]));
      NO_INTR(sys_close(args.zip_out[1]));
      errno = clone_errno;
      goto fail1;
    }

    /* Close write-end of pipe, and read from read-end until child closes
     * its reference to the pipe.
     */
    NO_INTR(sys_close(fds[1]));
    *compressors = NULL;
    while (c_read(fds[0], compressors, sizeof(*compressors), &errno)) {
    }
    NO_INTR(sys_close(fds[0]));
    
    /* Fail if either the child never even executed (unlikely), or
     * did not find any compressor that could be executed.
     */
    if (*compressors == NULL || (*compressors)->compressor == NULL) {
      int saved_errno1 = errno;
      NO_INTR(sys_close(args.zip_out[0]));
      NO_INTR(sys_close(args.zip_out[1]));
      errno = saved_errno1;
   fail2: {
        int saved_errno2 = errno;
        NO_INTR(sys_close(args.zip_in[0]));
        NO_INTR(sys_close(args.zip_in[1]));
        errno = saved_errno2;
        return -1;
      }
    }
    
    if (*(*compressors)->compressor) {
      /* Found a good compressor program, which is now connected to
       * zip_in/zip_out.
       */
      fds[0] = args.zip_out[0];
      fds[1] = args.zip_in[1];
      NO_INTR(sys_close(args.zip_in[0]));
      NO_INTR(sys_close(args.zip_out[1]));
    } else {
      /* No suitable compressor found, but the caller allowed
       * uncompressed core files. So, just close unneeded file handles,
       * and reap the child's exit code.
       */
      int status;
      fds[0] = -1;
      fds[1] = -1;
      NO_INTR(sys_close(args.zip_in[0]));NO_INTR(sys_close(args.zip_out[0]));
      NO_INTR(sys_close(args.zip_in[1]));NO_INTR(sys_close(args.zip_out[1]));
      while (sys_waitpid(comp_pid, &status, 0) < 0) {
        if (errno != EINTR) {
          goto fail2;
        }
      }
    }
  }
  return 0;
}


/* If this code is being built without support for multi-threaded core files,
 * some of our basic assumptions are not quite right. Most noticably, the
 * fake thread lister ends up calling InternalGetCoreDump() from the main
 * (i.e. only) thread in the application, which cannot be ptrace()'d at this
 * time. This prevents us from retrieving CPU registers.
 *
 * We work around this problem by delaying the call to ptrace() until we
 * have forked. We also need to double-fork here, in order to make sure that
 * the core writer process can get reaped by "init" after it reaches EOF.
 */
static inline int GetParentRegs(void *frame, regs *cpu, fpregs *fp,
                                fpxregs *fpx, int *hasSSE) {
#ifdef THREADS
  return 1;
#else
  int rc = 0;
  char scratch[4096];
  pid_t pid = getppid();
  if (sys_ptrace(PTRACE_ATTACH, pid, (void *)0, (void *)0) == 0 &&
      waitpid(pid, (void *)0, __WALL) >= 0) {
    memset(scratch, 0xFF, sizeof(scratch));
    if (sys_ptrace(PTRACE_GETREGS, pid, scratch, scratch) == 0) {
      memcpy(cpu, scratch, sizeof(struct regs));
      SET_FRAME(*(Frame *)frame, *cpu);
      memset(scratch, 0xFF, sizeof(scratch));
      if (sys_ptrace(PTRACE_GETFPREGS, pid, scratch, scratch) == 0) {
        memcpy(fp, scratch, sizeof(struct fpregs));
        memset(scratch, 0xFF, sizeof(scratch));
        #if defined(__i386__) && !defined( __x86_64__)
        /* Linux on x86-64 stores all FPU registers in the SSE structure     */
        if (sys_ptrace(PTRACE_GETFPXREGS, pid, scratch, scratch) == 0) {
          memcpy(fpx, scratch, sizeof(struct fpxregs));
        } else {
          *hasSSE = 0;
        }
        #else
        *hasSSE = 0;
        #endif
        rc = 1;
      }
    }
  }
  sys_ptrace_detach(pid);

  /* Need to double-fork, so that "init" can reap the core writer upon EOF.  */
  switch (sys_fork()) {
    case -1:
      return 0;
    case 0:
      return rc;
    default:
      sys__exit(0);
  }
#endif
}


/* Internal function for generating a core file. This function works for
 * both single- and multi-threaded core files. It assumes that all threads
 * are already suspended, and will resume them before returning.
 *
 * The caller must make sure that prctl(PR_SET_DUMPABLE, 1) has been called,
 * or this function might fail.
 */
int InternalGetCoreDump(void *frame, int num_threads, pid_t *pids,
                        va_list ap
                     /* const char *file_name, size_t max_length,
                        const char *PATH,
                        const struct CoredumperCompressor *compressors,
                        const struct CoredumperCompressor **selected_comp */) {
  long          i;
  int           rc = -1, fd = -1, threads = num_threads, hasSSE = 1;
  user          user;
  prpsinfo      prpsinfo;
  prstatus      prstatus;
  regs          thread_regs[threads];
  fpregs        thread_fpregs[threads];
  fpxregs       thread_fpxregs[threads];
  int           pair[2];
  int           main_pid = ((Frame *)frame)->tid;

  /* Get thread status                                                       */
  memset(thread_regs,    0, threads * sizeof(struct regs));
  memset(thread_fpregs,  0, threads * sizeof(struct fpregs));
  memset(thread_fpxregs, 0, threads * sizeof(struct fpxregs));

  /* Threads are already attached, read their registers now                  */
#ifdef THREADS
  for (i = 0; i < threads; i++) {
    char scratch[4096];
    memset(scratch, 0xFF, sizeof(scratch));
    if (sys_ptrace(PTRACE_GETREGS, pids[i], scratch, scratch) == 0) {
      memcpy(thread_regs + i, scratch, sizeof(struct regs));
      if (main_pid == pids[i])
        SET_FRAME(*(Frame *)frame, thread_regs[i]);
      memset(scratch, 0xFF, sizeof(scratch));
      if (sys_ptrace(PTRACE_GETFPREGS, pids[i], scratch, scratch) == 0) {
        memcpy(thread_fpregs + i, scratch, sizeof(struct fpregs));
        memset(scratch, 0xFF, sizeof(scratch));
        #if defined(__i386__) && !defined( __x86_64__)
        /* Linux on x86-64 stores all FPU registers in the SSE structure     */
        if (sys_ptrace(PTRACE_GETFPXREGS, pids[i], scratch, scratch) == 0) {
          memcpy(thread_fpxregs + i, scratch, sizeof(struct fpxregs));
        } else {
          hasSSE = 0;
        }
        #else
        hasSSE = 0;
        #endif
      } else
        goto ptrace;
    } else {
   ptrace: /* Oh, well, undo everything and get out of here                  */
      ResumeAllProcessThreads(threads, pids);
      goto error;
    }
  }

  /* Get parent's CPU registers, and user data structure                 */
  for (i = 0; i < sizeof(struct user); i += sizeof(int))
    sys_ptrace(PTRACE_PEEKUSER, pids[0], (void *)i,
               ((int *)&user) + i/sizeof(int));
  memcpy(&user.regs, thread_regs, sizeof(struct regs));
#endif

  /* Build the PRPSINFO data structure                                       */
  memset(&prpsinfo, 0, sizeof(struct prpsinfo));
  prpsinfo.pr_sname = 'R';
  prpsinfo.pr_nice  = sys_getpriority(PRIO_PROCESS, 0);
  prpsinfo.pr_uid   = sys_geteuid();
  prpsinfo.pr_gid   = sys_getegid();
  prpsinfo.pr_pid   = main_pid;
  prpsinfo.pr_ppid  = sys_getppid();
  prpsinfo.pr_pgrp  = sys_getpgrp();
  prpsinfo.pr_sid   = sys_getsid(0);
  /* scope */ {
    char scratch[4096], *cmd = scratch, *ptr;
    ssize_t size, len;
    int cmd_fd;
    memset(&scratch, 0, sizeof(scratch));
    size = sys_readlink("/proc/self/exe", scratch, sizeof(scratch));
    len = 0;
    for (ptr = cmd; *ptr != '\000' && size-- > 0; ptr++) {
      if (*ptr == '/') {
        cmd = ptr+1;
        len = 0;
      } else
        len++;
    }
    memcpy(prpsinfo.pr_fname, cmd,
           len > sizeof(prpsinfo.pr_fname) ? sizeof(prpsinfo.pr_fname) : len);
    NO_INTR(cmd_fd = sys_open("/proc/self/cmdline", O_RDONLY, 0));
    if (cmd_fd >= 0) {
      char *ptr;
      ssize_t size = c_read(cmd_fd, &prpsinfo.pr_psargs,
                            sizeof(prpsinfo.pr_psargs), &errno);
      for (ptr = prpsinfo.pr_psargs; size-- > 0; ptr++)
        if (*ptr == '\000')
          *ptr = ' ';
      NO_INTR(sys_close(cmd_fd));
    }
  }

  /* Build the PRSTATUS data structure                                       */
  /* scope */ {
    int stat_fd;
    memset(&prstatus, 0, sizeof(struct prstatus));
    prstatus.pr_pid     = prpsinfo.pr_pid;
    prstatus.pr_ppid    = prpsinfo.pr_ppid;
    prstatus.pr_pgrp    = prpsinfo.pr_pgrp;
    prstatus.pr_sid     = prpsinfo.pr_sid;
    prstatus.pr_fpvalid = 1;
    NO_INTR(stat_fd = sys_open("/proc/self/stat", O_RDONLY, 0));
    if (stat_fd >= 0) {
      char scratch[4096];
      ssize_t size = c_read(stat_fd, scratch, sizeof(scratch) - 1, &errno);
      if (size >= 0) {
        unsigned long tms;
        char *ptr = scratch;
        scratch[size] = '\000';

        /* User time                                                         */
        for (i = 13; i && *ptr; ptr++) if (*ptr == ' ') i--;
        tms = 0;
        while (*ptr && *ptr != ' ') tms = 10*tms + *ptr++ - '0';
        prstatus.pr_utime.tv_sec  = tms / 1000;
        prstatus.pr_utime.tv_usec = (tms % 1000) * 1000;

        /* System time                                                       */
        if (*ptr) ptr++;
        tms = 0;
        while (*ptr && *ptr != ' ') tms = 10*tms + *ptr++ - '0';
        prstatus.pr_stime.tv_sec  = tms / 1000;
        prstatus.pr_stime.tv_usec = (tms % 1000) * 1000;

        /* Cumulative user time                                              */
        if (*ptr) ptr++;
        tms = 0;
        while (*ptr && *ptr != ' ') tms = 10*tms + *ptr++ - '0';
        prstatus.pr_cutime.tv_sec  = tms / 1000;
        prstatus.pr_cutime.tv_usec = (tms % 1000) * 1000;

        /* Cumulative system time                                            */
        if (*ptr) ptr++;
        tms = 0;
        while (*ptr && *ptr != ' ') tms = 10*tms + *ptr++ - '0';
        prstatus.pr_cstime.tv_sec  = tms / 1000;
        prstatus.pr_cstime.tv_usec = (tms % 1000) * 1000;

        /* Pending signals                                                   */
        for (i = 14; i && *ptr; ptr++) if (*ptr == ' ') i--;
        while (*ptr && *ptr != ' ')
          prstatus.pr_sigpend = 10*prstatus.pr_sigpend + *ptr++ - '0';

        /* Held signals                                                      */
        if (*ptr) ptr++;
        while (*ptr && *ptr != ' ')
          prstatus.pr_sigpend = 10*prstatus.pr_sigpend + *ptr++ - '0';
      }
      NO_INTR(sys_close(stat_fd));
    }
  }

  /* scope */ {
    int openmax  = sys_sysconf(_SC_OPEN_MAX);
    int pagesize = sys_sysconf(_SC_PAGESIZE);
    sigset_t old_signals, blocked_signals;

    const char *file_name =
      va_arg(ap, const char *);
    size_t max_length =
      va_arg(ap, size_t);
    const char *PATH =
      va_arg(ap, const char *);
    const struct CoredumperCompressor *compressors =
      va_arg(ap, const struct CoredumperCompressor *);
    const struct CoredumperCompressor **selected_compressor =
      va_arg(ap, const struct CoredumperCompressor **);

    if (selected_compressor != NULL) {
      /* For now, assume that the core dump is uncompressed; we will later
       * override this setting, if we can find a suitable compressor program.
       */
      *selected_compressor = compressors;
      while (*selected_compressor &&
             (*selected_compressor)->compressor != NULL) {
        ++*selected_compressor;
      }
    }

    if (file_name == NULL) {
      /* Create a file descriptor that can be used for reading data from
       * our child process. This is a little complicated because we need
       * to make sure there is no race condition with other threads
       * calling fork() at the same time (this is somewhat mitigated,
       * because our threads are supposedly suspended at this time). We
       * have to avoid other processes holding our file handles open. We
       * can do this by creating the pipe in the child and passing the
       * file handle back to the parent.
       */
      if (sys_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) >= 0) {
        /* Block signals prior to forking. Technically, POSIX requires
         * us to call pthread_sigmask(), if this is a threaded
         * application. When using glibc, we are OK calling
         * sigprocmask(), though. We will end up blocking additional
         * signals that libpthread uses internally, but that
         * is actually exactly what we want.
         *
         * Also, POSIX claims that this should not actually be
         * necessarily, but reality says otherwise.
         */
        sigfillset(&blocked_signals);
        sys_sigprocmask(SIG_BLOCK, &blocked_signals, &old_signals);
        
        /* Create a new core dump in child process; call sys_fork() in order to
         * avoid complications with pthread_atfork() handlers. In the child
         * process, we should only ever call system calls.
         */
        if ((rc = sys_fork()) == 0) {
          int  fds[2];
          
          /* Create a pipe for communicating between processes. If
           * necessary, add a compressor to the pipeline.
           */
          if (CreatePipeline(fds, openmax, PATH, &compressors) < 0 ||
              (fds[0] < 0 && sys_pipe(fds) < 0)) {
            sys__exit(1);
          }
        
          /* Pass file handle to parent                                      */
          /* scope */ {
            char cmsg_buf[CMSG_SPACE(sizeof(int))];
            struct iovec  iov;
            struct msghdr msg;
            struct cmsghdr *cmsg;
            memset(&iov, 0, sizeof(iov));
            memset(&msg, 0, sizeof(msg));
            iov.iov_base            = (void *)&compressors;
            iov.iov_len             = sizeof(compressors);
            msg.msg_iov             = &iov;
            msg.msg_iovlen          = 1;
            msg.msg_control         = &cmsg_buf;
            msg.msg_controllen      = sizeof(cmsg_buf);
            cmsg                    = CMSG_FIRSTHDR(&msg);
            if (!cmsg) {
              /* This can't happen, but static analyzers still complain...   */
              sys__exit(1);
            }
            cmsg->cmsg_level        = SOL_SOCKET;
            cmsg->cmsg_type         = SCM_RIGHTS;
            cmsg->cmsg_len          = CMSG_LEN(sizeof(int));
            *(int *)CMSG_DATA(cmsg) = fds[0];
            while (sys_sendmsg(pair[1], &msg, 0) < 0) {
              if (errno != EINTR)
                sys__exit(1);
            }
            while (sys_shutdown(pair[1], SHUT_RDWR) < 0) {
              if (errno != EINTR)
                sys__exit(1);
            }
          }
          
          /* Close all file handles other than the write end of our pipe     */
          for (i = 0; i < openmax; i++) {
            if (i != fds[1]) {
              NO_INTR(sys_close(i)); }
          }
          
          /* If compiled without threading support, this is the only
           * place where we can request the parent's CPU
           * registers. This function is a no-op when threading
           * support is available.
           */
          if (!GetParentRegs(frame, thread_regs, thread_fpregs,
                             thread_fpxregs, &hasSSE)) {
            sys__exit(1);
          }
          
          CreateElfCore(&fds[1], SimpleWriter, SimpleDone, &prpsinfo, &user,
                        &prstatus, threads, pids, thread_regs, thread_fpregs,
                        hasSSE ? thread_fpxregs : NULL, pagesize);
          NO_INTR(sys_close(fds[1]));
          sys__exit(0);

          /* Make the compiler happy. We never actually get here.            */
          return 0;
        } else if (rc > 0) {
          #ifndef THREADS
          /* Child will double-fork, so reap the process, now.               */
          sys_waitpid(rc, (void *)0, __WALL);
          #endif
        }

        /* In the parent                                                     */
        sys_sigprocmask(SIG_SETMASK, &old_signals, (void *)0);
        NO_INTR(sys_close(pair[1]));
        
        /* Get pipe file handle from child                                   */
        /* scope */ {
          void *buffer[1];
          char cmsg_buf[CMSG_SPACE(sizeof(int))];
          struct iovec  iov;
          struct msghdr msg;
          for (;;) {
            int nbytes;
            memset(&iov, 0, sizeof(iov));
            memset(&msg, 0, sizeof(msg));
            iov.iov_base       = buffer;
            iov.iov_len        = sizeof(void *);
            msg.msg_iov        = &iov;
            msg.msg_iovlen     = 1;
            msg.msg_control    = &cmsg_buf;
            msg.msg_controllen = sizeof(cmsg_buf);
            if ((nbytes = sys_recvmsg(pair[0], &msg, 0)) > 0) {
              struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
              if (cmsg != NULL && cmsg->cmsg_level == SOL_SOCKET &&
                  cmsg->cmsg_type == SCM_RIGHTS)
                fd = *(int *)CMSG_DATA(cmsg);
              if (nbytes == sizeof(void *) &&
                  *buffer != NULL && selected_compressor != NULL)
                *selected_compressor = *buffer;
              break;
            } else if (nbytes == 0 || errno != EINTR) {
              break;
            }
          }
        }
        sys_shutdown(pair[0], SHUT_RDWR);
        NO_INTR(sys_close(pair[0]));
      }
    } else {
      /* Synchronously write the core to a file. If necessary, compress the
       * data on the fly. All other threads are suspended during this time.
       * In principle, we could use the same code that we used earlier for
       * building a core file on the fly. But that results in creating a COW
       * copy of the address space (as a result of the call to fork()), and
       * some accounting applications are sensitive to the sudden spike in
       * memory usage.
       * So, instead, we run a single thread and make use of callback
       * functions that internally invoke poll() for managing the I/O.
       */
      int fds[2] = { -1, -1 };
      int saved_errno, rc;
      const char *suffix = "";
      struct WriterFds writer_fds;
      ssize_t (*writer)(void *, const void *, size_t);

      /* If compiled without threading support, this is the only
       * place where we can request the parent's CPU
       * registers. This function is a no-op when threading
       * support is available.
       */
      if (!GetParentRegs(frame, thread_regs, thread_fpregs,
                         thread_fpxregs, &hasSSE)) {
        goto error;
      }
      
      /* Create a pipe for communicating between processes. If
       * necessary, add a compressor to the pipeline.
       */
      if (compressors != NULL && compressors->compressor != NULL) {
        if (CreatePipeline(fds, openmax, PATH, &compressors) < 0) {
          goto error;
        }
      }
      if (selected_compressor) {
        *selected_compressor = compressors;
      }

      writer_fds.out_fd = -1;
      if (max_length > 0) {
        /* Open the output file. If necessary, pick a filename suffix that
         * matches the selected compression type.
         */
        if (compressors != NULL && compressors->compressor != NULL &&
            compressors->suffix != NULL) {
          suffix = compressors->suffix;
        }
        /* scope */ {
          char extended_file_name[strlen(file_name) + strlen(suffix) + 1];
          strcat(strcpy(extended_file_name, file_name), suffix);
          NO_INTR(writer_fds.out_fd = sys_open(extended_file_name,
                                               O_WRONLY|O_CREAT|O_TRUNC,
                                               0600));
          if (writer_fds.out_fd < 0) {
            saved_errno = errno;
            if (fds[0] >= 0) NO_INTR(sys_close(fds[0]));
            if (fds[1] >= 0) NO_INTR(sys_close(fds[1]));
            errno = saved_errno;
            goto error;
          }
        }
        
        /* Set up a suitable writer funtion.                                 */
        writer_fds.max_length = max_length;
        if (fds[0] >= 0) {
          /* The PipeWriter() can deal with multi I/O requests on the
           * compression pipeline.
           */
          long flags;
          NO_INTR(flags = sys_fcntl(fds[0], F_GETFL, 0));
          NO_INTR(sys_fcntl(fds[0], F_SETFL, flags | O_NONBLOCK));
          NO_INTR(flags = sys_fcntl(fds[1], F_GETFL, 0));
          NO_INTR(sys_fcntl(fds[1], F_SETFL, flags | O_NONBLOCK));
          writer_fds.write_fd      = fds[1];
          writer_fds.compressed_fd = fds[0];
          writer                   = PipeWriter;
        } else {
          /* If no compression is needed, then we can directly write to the
           * file. This avoids quite a bit of unnecessary overhead.
           */
          writer                   = LimitWriter;
        }
        
        rc = CreateElfCore(&writer_fds, writer, PipeDone, &prpsinfo, &user,
                           &prstatus, threads, pids, thread_regs,
                           thread_fpregs, hasSSE ? thread_fpxregs : NULL,
                           pagesize);
        if (fds[0] >= 0) {
          saved_errno = errno;
          /* Close the input side of the compression pipeline, and flush
           * the remaining compressed data bytes out to the file.
           */
          if (fds[1] >= 0) { NO_INTR(sys_close(fds[1])); fds[1] = -1; }
          if (FlushPipe(&writer_fds) < 0) {
            rc = -1;
          } else {
            errno = saved_errno;
          }
        }
      } else {
        rc = 0;
      }

      /* Close all remaining open file handles.                              */
      saved_errno = errno;
      if (writer_fds.out_fd >= 0) NO_INTR(sys_close(writer_fds.out_fd));
      if (fds[0] >= 0)            NO_INTR(sys_close(fds[0]));
      if (fds[1] >= 0)            NO_INTR(sys_close(fds[1]));
      errno = saved_errno;

      if (rc < 0) {
        goto error;
      }

      /* If called with a filename, we do not actually return a file handle,
       * but instead just signal whether the core file has been written
       * successfully.
       */
      fd = 0;
    }
  }

  ResumeAllProcessThreads(threads, pids);
  return fd;

error:
  /* scope */ {
    int saved_errno = errno;
    if (fd > 0)
      NO_INTR(sys_close(fd));
    errno = saved_errno;
  }
  ResumeAllProcessThreads(threads, pids);
  return -1;
}
#endif
