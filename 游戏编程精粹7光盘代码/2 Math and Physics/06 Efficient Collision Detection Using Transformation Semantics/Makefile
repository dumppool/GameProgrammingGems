# simple Makefile for building the demo.
#

CPP  = g++
LIBS = -lm 
CXXINCS = 
CXXFLAGS = $(CXXINCS) -fno-strict-aliasing
CFLAGS = $(CXXFLAGS)
# list of files to compile and link
LINKOBJ = tsemantics.o
	  
	  


# ------------------------------------------------------------------------------------------
# All targets, builds a plugin and copies it to the given directory
# ------------------------------------------------------------------------------------------
all: sample


# ------------------------------------------------------------------------------------------
# Cleans all temporary files and compilation results.
# ------------------------------------------------------------------------------------------
clean:
	rm -f $(LINKOBJ)

# ------------------------------------------------------------------------------------------
# Builds the sample application
# ------------------------------------------------------------------------------------------
sample: $(LINKOBJ)
	$(CPP) -o ./tsemantics $(LINKOBJ) $(LIBS)


# ------------------------------------------------------------------------------------------
# C++ compilation rule
# ------------------------------------------------------------------------------------------
.cpp.o:
	$(CPP) $(CXXFLAGS) -Wall -O2 -c -o $@ $<
