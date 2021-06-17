/*
**	getopt -- public domain version of standard System V routine
**
**	Strictly enforces the System V Command Syntax Standard;
**	provided by D A Gwyn of BRL for generic ANSI C implementations
**
**	Modified for dLibs on the Atari ST by Dale Schumacher, 07Oct88.
**	#define STRICT to prevent acceptance of clustered options with
**	arguments and ommision of whitespace between option and arg.
*/

#include <stdio.h>
#ifdef SYSV
#include <strings.h>
#define strchr index
#else
# ifdef ATARI
#include <strings.h>
#define strchr index
# else
#include <string.h>
# endif
#endif

int	opterr = 1;			/* error => print message */
int	optind = 1;			/* next argv[] index */
char	*optarg = NULL;			/* option parameter if any */

static int
Err(char *name, char *mess, int c)			/* returns '?' */
//char	*name;			/* program name argv[0] */
//char	*mess;			/* specific message */
//int	c;			/* defective option letter */
{
#ifdef dLibs
	char nbuf[16];
#endif

	if(opterr)
	{
#ifdef dLibs
		_splitpath(name, NULL, NULL, nbuf, NULL);
		name = nbuf;
#endif
		(void)fprintf(stderr, "%s: %s -- %c\n", name, mess, c);
	}

	return '?';			/* erroneous-option marker */
}

int
getopt(int argc, char *argv[], char *optstring)  /* returns letter, '?', EOF */
//int   argc;        /* argument count from main */
//char *argv[];      /* argument vector from main */
//char *optstring;   /* allowed args, e.g. "ab:c" */
{
	static int     sp = 1;		/* position within argument */
#ifdef STRICT
	register int   osp;		/* saved `sp' for param test */
#else
	register int   oind;		/* saved `optind' for param test */
#endif
	register int   c;		/* option letter */
	register char *cp;		/* -> option in `optstring' */

	optarg = NULL;

	if(sp == 1)			/* fresh argument */
		if( optind >= argc            /* no more arguments */
		  || argv[optind][0] != '-'   /* no more options */
		  || argv[optind][1] == '\0'  /* not option; stdin */
		   )
			return EOF;
		else if(strcmp(argv[optind], "--") == 0)
		{
			++optind;       /* skip over "--" */
			return EOF;     /* "--" marks end of options */
		}

	c = argv[optind][sp];		/* option letter */
#ifdef STRICT
	osp =
#endif
	sp++;			/* get ready for next letter */

#ifndef STRICT
	oind = optind;			/* save optind for param test */
#endif
	if ( argv[optind][sp] == '\0' )	/* end of argument */
		{
		++optind;		/* get ready for next try */
		sp = 1;			/* beginning of next argument */
		}

	if ( c == ':' || c == '?'	/* optstring syntax conflict */
	  || (cp = strchr( optstring, c )) == NULL	/* not found */
	   )
		return Err( argv[0], "illegal option", c );

	if ( cp[1] == ':' )		/* option takes parameter */
		{
#ifdef STRICT
		if ( osp != 1 )
			return Err( argv[0],
				    "option must not be clustered",
				    c
				  );

		if ( sp != 1 )		/* reset by end of argument */
			return Err( argv[0],
			       "option must be followed by white space",
				    c
				  );

#else
		if ( oind == optind )	/* argument w/o whitespace */
			{
			optarg = &argv[optind][sp];
			sp = 1;		/* beginning of next argument */
			}

		else
#endif
		if ( optind >= argc )
			return Err( argv[0],
				    "option requires an argument",
				    c
				  );

		else			/* argument w/ whitespace */
			optarg = argv[optind];

		++optind;		/* skip over parameter */
		}

	return c;
	}

