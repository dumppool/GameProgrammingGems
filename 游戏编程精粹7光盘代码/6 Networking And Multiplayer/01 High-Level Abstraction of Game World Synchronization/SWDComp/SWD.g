// MySWD grammar definition
//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

options {
	language="Cpp";
}

/* A Simple SWD Recognizer authored by Hyun-jik Bae,imays@paran.com. */

class SWDParser extends Parser;
options {
	exportVocab=SWD;
}

//********* This is the first plce to parse for SWD file
specification returns [ RefCount<CSpecification> spec ]
	{
		spec=RefCount<CSpecification>(new CSpecification);
		RefCount<CConfigurations> configs;
		RefCount<CSynchClass> cls;
	}
	:	configs=configurations { spec->SetConfigs(configs); }
		(cls=synchClassDefinition { spec->AddSynchClass(cls); } )*
		EOF!
	;

// SWD configurations to be applied to every SWD class
configurations returns [ RefCount<CConfigurations> configs ]
	{
		configs=RefCount<CConfigurations>(new CConfigurations);
		RefCount<CConfiguration> config;
	}
	:
	"config" 
	LCURLY
	(config=configuration { configs->AddConfig(config); } )*
	RCURLY
	;
	
// configurations defined in SWD files
configuration returns [ RefCount<CConfiguration> config ]
	{
		config=RefCount<CConfiguration>(new CConfiguration);
	}
	:
	configName:IDENT { config->SetName(configName->getText().c_str()); }
	ASSIGN
	(
		( configValueInt:INT { config->SetValue(configValueInt->getText().c_str()); } ) |
		( configValue:IDENT { config->SetValue(configValue->getText().c_str()); } )
	)
	SEMI
	;
	
// a synch class definition
synchClassDefinition returns [ RefCount<CSynchClass> cls ]
	{
		cls=RefCount<CSynchClass>(new CSynchClass);
		RefCount<CMember> member;
	}
	:
	"synch_class" className:IDENT { cls->SetName(className->getText().c_str()); }
	LCURLY
	(
		( member=memberDefinition { cls->AddMember(member); } ) *
	)
	RCURLY
	;

// a synch class member
memberDefinition returns [ RefCount<CMember> member ]
	{
		member=RefCount<CMember>(new CMember);
		RefCount<CBehavior> beh;
	}
	:
	beh=behavior { member->SetBehavior(beh); }
	type:IDENT { member->SetType(type->getText().c_str()); }
	name:IDENT { member->SetName(name->getText().c_str()); }
	SEMI
	;
	
behavior returns [ RefCount<CBehavior> beh ]
	{
		beh=RefCount<CBehavior>(new CBehavior);
	}
	:
	("conditional" { beh->SetType(BehaviorType_Conditional); } ) |
	("static" { beh->SetType(BehaviorType_Static); } ) 
	// TODO: add more synch attributes here, with OR operator
	;

identifier
	:   IDENT
  	;

/* SWD LEXICAL RULES  */
class SWDLexer extends Lexer;
options {
	exportVocab=SWD;
	k=4;
}

SEMI
options {
  paraphrase = ";";
}
	:	';'
	;

QUESTION
options {
  paraphrase = "?";
}
	:	'?'
	;

LPAREN
options {
  paraphrase = "(";
}
	:	'('
	;

RPAREN
options {
  paraphrase = ")";
}
	:	')'
	;

LBRACK
options {
  paraphrase = "[";
}
	:	'['
	;

RBRACK
options {
  paraphrase = "]";
}
	:	']'
	;

LCURLY
options {
  paraphrase = "{";
}
	:	'{'
	;

RCURLY
options {
  paraphrase = "}";
}
	:	'}'
	;

OR
options {
  paraphrase = "|";
}
	:	'|'
	;

XOR
options {
  paraphrase = "^";
}
	:	'^'
	;

AND
options {
  paraphrase = "&";
}
	:	'&'
	;

COLON
options {
  paraphrase = ":";
}
	:	':'
	;

COMMA
options {
  paraphrase = ",";
}
	:	','
	;

DOT
options {
  paraphrase = ".";
}
	:	'.'
	;

ASSIGN
options {
  paraphrase = "=";
}
	:	'='
	;

NOT
options {
  paraphrase = "!";
}
	:	'!'
	;

LT_
options {
  paraphrase = "<";
}
	:	'<'
	;

LSHIFT
options {
  paraphrase = "<<";
}
	: "<<"
	;

GT
options {
  paraphrase = ">";
}
	:	'>'
	;

RSHIFT
options {
  paraphrase = ">>";
}
	: ">>"
	;

DIV
options {
  paraphrase = "/";
}
	:	'/'
	;

PLUS
options {
  paraphrase = "+";
}
	:	'+'
	;

MINUS
options {
  paraphrase = "-";
}
	:	'-'
	;

TILDE
options {
  paraphrase = "~";
}
	:	'~'
	;

STAR
options {
  paraphrase = "*";
}
	:	'*'
	;

MOD
options {
  paraphrase = "%";
}
	:	'%'
	;

SCOPEOP
options {
  paraphrase = "::";
}
	:  	"::"
	;

WS_
options {
  paraphrase = "white space";
}
	:	(' '
	|	'\t'
	|	'\n'  { newline(); }
	|	'\r')
		{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }
	;

PREPROC_DIRECTIVE
options {
  paraphrase = "a preprocessor directive";
}

	:
	'#'
	(~'\n')* '\n'
	{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }
	;

SL_COMMENT
options {
  paraphrase = "a comment";
}

	:
	"//"
	(~'\n')* '\n'
	{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); newline(); }
	;

ML_COMMENT
options {
  paraphrase = "a comment";
}
	:
	"/*"
	(
			STRING_LITERAL
		|	CHAR_LITERAL
		|	'\n' { newline(); }
		|	'*' ~'/'
		|	~'*'
	)*
	"*/"
	{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP);  }
	;

CHAR_LITERAL
options {
  paraphrase = "a character literal";
}
	:
	'\''
	( ESC | ~'\'' )
	'\''
	;

STRING_LITERAL
options {
  paraphrase = "a string literal";
}
	:
	'"'
	(ESC|~'"')*
	'"'
	;

protected
ESC
options {
  paraphrase = "an escape sequence";
}
	:	'\\'
		(	'n'
		|	't'
		|	'v'
		|	'b'
		|	'r'
		|	'f'
		|	'a'
		|	'\\'
		|	'?'
		|	'\''
		|	'"'
		|	('0' | '1' | '2' | '3')
			(
				/* Since a digit can occur in a string literal,
				 * which can follow an ESC reference, ANTLR
				 * does not know if you want to match the digit
				 * here (greedy) or in string literal.
				 * The same applies for the next two decisions
				 * with the warnWhenFollowAmbig option.
				 */
				options {
					warnWhenFollowAmbig = false;
				}
			:	OCTDIGIT
				(
					options {
						warnWhenFollowAmbig = false;
					}
				:	OCTDIGIT
				)?
			)?
		|   'x' HEXDIGIT
			(
				options {
					warnWhenFollowAmbig = false;
				}
			:	HEXDIGIT
			)?
		)
	;

protected
VOCAB
options {
  paraphrase = "an escaped character value";
}
	:	'\3'..'\377'
	;

protected
DIGIT
options {
  paraphrase = "a digit";
}
	:	'0'..'9'
	;

protected
OCTDIGIT
options {
  paraphrase = "an octal digit";
}
	:	'0'..'7'
	;

protected
HEXDIGIT
options {
  paraphrase = "a hexadecimal digit";
}
	:	('0'..'9' | 'a'..'f' | 'A'..'F')
	;

/* octal literals are detected by checkOctal */

HEX
options {
  paraphrase = "a hexadecimal value value";
}

	:    ("0x" | "0X") (HEXDIGIT)+
	;

INT
options {
  paraphrase = "an integer value";
}
	:    (DIGIT)+                  // base-10
             (  '.' (DIGIT)*                      	{$setType(FLOAT);}
	         (('e' | 'E') ('+' | '-')? (DIGIT)+)?
	     |   ('e' | 'E') ('+' | '-')? (DIGIT)+   	{$setType(FLOAT);}
             )?
	;

FLOAT
options {
  paraphrase = "an floating point value";
}

	:    '.' (DIGIT)+ (('e' | 'E') ('+' | '-')? (DIGIT)+)?
     	;

IDENT
options {
  testLiterals = true;
  paraphrase = "an identifer";
}

	:	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')*
	;
