%name reactionscan

%header{
#include "Masses.h"
#include "Reactions.h"
%}

%define MEMBERS           const char *inputstream; \
                          unsigned int pos; \
                          int out1, out2, out3;
%define CONSTRUCTOR_PARAM const char *in
%define CONSTRUCTOR_CODE  inputstream = in; pos=0;
%define PARSE_PARAM       reaction *r, Particle *spec[], Particle *M[]

%union {
  Particle *prt;
  char     *str;
  int      nr;
}

%token NUCLEUS SPEC PARTICLE RESONANCE

%type <prt> NUCLEUS PARTICLE RESONANCE known detect1 detect2 detect3
%type <nr> SPEC

%%
 
input:      reaction | input ',' reaction | input ';' reaction;

reaction:   elastic | twoBody | resonance | triples | inclusive;
 
elastic:    target ePrime ')' known     { r->Out1 = *$4; }
          | target ePrime detect1 ')'   { M[0]=$3; r->Out1 = *$3; };

twoBody:    target ePrime ')' known   known       {
              M[0]=$4; M[1]=$5;
	      r->Out1 = *$4;
	      r->Out2 = *$5;
            }
          | target ePrime detect1 ')' known       {
              M[0]=$3; M[1]=$5;
	      r->Out1 = *$3;
	      r->Out2 = *$5;
            }
          | target ePrime detect1 detect2 ')'     {
              M[0]=$3; M[1]=$4;
	      r->Out1 = *$3;
	      r->Out2 = *$4;
            };

triples:    target ePrime detect1 detect2 ')' known {
              spec[out1] = &r->Decay1;
              spec[out2] = &r->Decay2;
	      M[0]=$6; M[1]=$3; M[2]=$4;
	      r->Out1   = *$6;
	      r->Decay1 = *$3;
	      r->Decay2 = *$4;
          }
          | target ePrime detect1 ')' known known {
              spec[out1] = &r->Decay1;
              M[0]=$6; M[1]=$3; M[2]=$5;
	      r->Out1   = *$6;
	      r->Decay1 = *$3;
	      r->Decay2 = *$5;
          }
          | target ePrime detect1 detect2 detect3 ')' {
              M[0]=$3; M[1]=$4; M[2]=$5;
	      r->Out1 = *$3;
	      r->Out2 = *$4;
	      r->Out3 = *$5;
            };

resonance:  target ePrime RESONANCE '(' detect1 detect2 ')' ')' known { 
              spec[out1] = &r->Decay1;
              spec[out2] = &r->Decay2;
              M[0] = $9; M[1] = $3; M[2] = $5; M[3] = $6;
	      r->Out1   = *$9;
	      r->Out2   = *$3;
	      r->Decay1 = *$5;
	      r->Decay2 = *$6;
            };

inclusive:  target ePrime ')' 'X' 
          | target ePrime detect1 ')' 'X' { M[0]=$3; r->Out1   = *$3; }
          | target ePrime detect1 detect2 ')' 'X'{ 
              M[0]=$3; r->Out1   = *$3; 
              M[1]=$4; r->Out2   = *$4; 
            };

target:     NUCLEUS '(' 'e' ',' { r->target = *$1; };
ePrime:    'e' '\'' | SPEC 'e' '\'' { spec[$1] = &r->electronOut;};
known:      NUCLEUS  { $$ = $1; } | PARTICLE { $$ = $1; };
detect1:    SPEC PARTICLE { out1=$1; spec[$1] = &r->Out1;  $$ = $2;};
detect2:    SPEC PARTICLE { out2=$1; spec[$1] = &r->Out2;  $$ = $2;};
detect3:    SPEC PARTICLE { out3=$1; spec[$1] = &r->Out3;  $$ = $2;};
%%



