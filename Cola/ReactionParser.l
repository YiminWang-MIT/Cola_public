//                                                                    -*-c++-*-
%name reactionlex 
%{
#include "ReactionParser.parse.h"
#include <cstring>
  
%}
%define INPUT_NOCODE
%define MEMBERS              reactionscan *parser; const char *inputstream;
%define CONSTRUCTOR_PARAM    reactionscan *h,      const char *input
%define CONSTRUCTOR_CODE     parser=h;             inputstream=input;

NUCLEUS   [A-Z][a-z0-9]*|"NB1004"|"NB1044"|"NB1094"
SPEC      "A:"|"B:"|"C:"|"D:"|"O:"|"H3:"|"TOF:"|"NPOL:"|"NDET:"|"SI:"|"KAOS:"
PARTICLE  "e+"|"e-"|"mu-"|"mu+"|"p"|"d"|"n"|"t"|"eta"|"pi+"|"pi-"|"pi0"|"gamma"|"K+"|"K-"|"Lambda"|"Sigma0"
RESONANCE "rho"|"rho600"|"rho700"|"omega"|"Delta"
%%
":"|"e"|"X"     return yytext[0];
";"|","|"'"|"("|")" return yytext[0];
{SPEC}        { if (!strcmp((char*)yytext, "A:"))    parser->yylval.nr = 0;
                if (!strcmp((char*)yytext, "B:"))    parser->yylval.nr = 1;
		if (!strcmp((char*)yytext, "C:"))    parser->yylval.nr = 2;
		if (!strcmp((char*)yytext, "D:"))    parser->yylval.nr = 3;
		if (!strcmp((char*)yytext, "O:"))    parser->yylval.nr = 4;
		if (!strcmp((char*)yytext, "H3:"))   parser->yylval.nr = 5;
		if (!strcmp((char*)yytext, "TOF:"))  parser->yylval.nr = 6;
                if (!strcmp((char*)yytext, "NPOL:")) parser->yylval.nr = 7;
                if (!strcmp((char*)yytext, "NDET:")) parser->yylval.nr = 8;
                if (!strcmp((char*)yytext, "SI:"))   parser->yylval.nr = 9;
		if (!strcmp((char*)yytext, "KAOS:")) parser->yylval.nr = 10;
                return parser->SPEC; 
              }
{RESONANCE}   { parser->yylval.prt = Particle::find((char*)yytext);
                if (!parser->yylval.prt) parser->yyerror("Unknown resonance");
                return parser->RESONANCE; 
              }
{NUCLEUS}     { parser->yylval.prt = Particle::find((char*)yytext);
                if (!parser->yylval.prt) parser->yyerror("Unknown nucleus");
                return parser->NUCLEUS;
              }
{PARTICLE}    { parser->yylval.prt = Particle::find((char*)yytext);
                if (!parser->yylval.prt) parser->yyerror("Unknown particle");
                return parser->PARTICLE;
              }
[ \t]+  
[A-Ba-b][A-Ba-b]*:  parser->yyerror("Unknown spectrometer");
.                   parser->yyerror("Unidentified token");
%%
	   

int reactionlex::yy_input(char buf[], int &result, int)
{ 
  if (parser->pos==strlen(inputstream)) 
    result=YY_NULL; 
  else { 
    strcpy(buf, inputstream); 
    result = parser->pos = strlen(inputstream);
  }
  return result;
}

  
void reactionscan::yyerror(char *msg)
{
  std::cerr << "\033[31mERROR parsing reaction \"" 
	    << inputstream << "\": " << msg << "\033[4;m" << std::endl;
}

int reactionscan::yylex() 
{
  static reactionlex *lexi;
  if (pos==0) lexi = new reactionlex(this, inputstream);
  return lexi->yylex(); 
}






