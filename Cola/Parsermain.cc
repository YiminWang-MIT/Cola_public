//                                                                    -*-c++-*-
// $Id: Parsermain.cc 2575 2014-09-02 12:30:35Z merkel $
//

#include "FourVector/FourVector.h"
#include "rundatabase.h"
#include "ExpressionParser.parse.cc"
#include "Cola/Reactions.h"
#include "Cola/display.h"
#include "Cola/ColaNtuple.h"
#include "Aqua/MezzoReader.h"
#include <ctype.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

FILE *parsefile = NULL;
AquaTree *aintree = NULL;
AquaTree *outtree = NULL;
MezzoReader *mreader = NULL;
EventDisplay *eventDisplay = NULL;

expression *spinBasis[] = {new literal<FourVector> (FourVector(0,1,0,0)),
			   new literal<FourVector> (FourVector(0,0,1,0)),
			   new literal<FourVector> (FourVector(0,0,0,1))};
expression *AnaX = new literal<double>(0), 
           *AnaY = new literal<double>(0);

void setSpinBasis(expression *x, expression *y, expression *z, 
		  expression *Ax ,expression *Ay) {
  for (int i=0; i<3; i++) delete spinBasis[i];
  spinBasis[0] = x;
  spinBasis[1] = y;
  spinBasis[2] = z;
  AnaX = Ax;
  AnaY = Ay;
}

FourVector normal(0.0), euler(0.0);
FourVector par(0.0),ref(0.0);

FourVector SpinProjection(FourVector particle, FourVector reference)
{
  par = particle;
  ref = reference;
  const double refmomA = 660 / 630.0;
  double angle = rundb.A.angle / 180 * M_PI;
  static AquaNodeDouble 
    *dp = (AquaNodeDouble *) outtree->find("online.A.target.dp"),
    *y0 = (AquaNodeDouble *) outtree->find("online.A.target.y0"),
    *th = (AquaNodeDouble *) outtree->find("online.A.target.th"),
    *ph = (AquaNodeDouble *) outtree->find("online.A.target.ph");
  
  if (!dp->ok()||!y0->ok()||!th->ok()||!ph->ok()) return FourVector(0.0);
  
  Particle part = P_proton;
  part = particle.Lorentz(-reference).rotate(reference);
  double res[3],norm[3], e[3];
  Momentum trans[3]={0.0,0.0,0.0};

  for(int i=0;i<3;i++) {
    part.setSpin(spinBasis[i]->evaluate_f());
    Particle cm = part.rotateTo(reference).Lorentz(reference);
    Momentum lab = cm.getSpin();
    Momentum specspin( lab[1], 
		     -(lab[0]*cos(angle) + lab[2] * sin(angle)), 
		      -lab[0]*sin(angle) + lab[2] * cos(angle));
    //   printf("%f %f %f\n", specspin[0], specspin[1], specspin[2]);
// printf("%f %f %f %f %f\n", (double) *dp,(double)*th,(double)*y0,(double)*ph,rundb.A.momentum/refmomA);
  
  
// These lines test the SpinTrans function, compare results with Th. Pospischil
//    Momentum xx(0,0,1);
//    Momentum test=SpinMatrix::matrix.SpinTrans(xx,0,0,0,0,630);
//    printf("test1: %f %f %f\n",test[0],test[1],test[2]);
//    test=SpinMatrix::matrix.SpinTrans(xx,0,-70,-25,-100,630);
//    printf("test2: %f %f %f\n",test[0],test[1],test[2]);
//    test=SpinMatrix::matrix.SpinTrans(xx,0, 70, 25, 100,630);
//    printf("test3: %f %f %f\n",test[0],test[1],test[2]);
//    printf("%f %f %f %f\n",(double) *dp,(double)*th,(double)*y0,(double)*ph);

    trans[i] = SpinMatrix::matrix.SpinTrans(specspin,
					    (double) *dp,(double)*th,
					    10*(double)*y0,(double)*ph,
					    rundb.A.momentum/refmomA);

   res [i] = trans[i][0] * AnaX->evaluate() + trans[i][1] * AnaY->evaluate();
    norm[i] = pow(trans[i][0] * AnaX->evaluate(), 2)
      +       pow(trans[i][1] * AnaY->evaluate(), 2);
  }
  
  e[2] = atan2(-trans[1][0],                trans[0][0]);
  e[1] = ( cos(e[2])>0 ? 
	   atan2(-trans[2][0] * cos(e[2]),  trans[0][0]) :
	   atan2( trans[2][0] * cos(e[2]), -trans[0][0]) );
  e[0] = ( cos(e[1])>0 ? 
	   atan2(-trans[2][1],  trans[2][2]) :
	   atan2( trans[2][1], -trans[2][2]));

  normal = FourVector(0, norm[0], norm[1], norm[2]);
  euler  = FourVector(0, e[0],    e[1],    e[2]);
  return   FourVector(0, res[0],  res[1],  res[2]);
};

FourVector SpinNormalization(FourVector particle, FourVector reference) {
  if (par!=particle || ref!=reference) SpinProjection(particle, reference);
  return normal;
};

FourVector SpinEuler(FourVector particle, FourVector reference) {
  if (par!=particle || ref!=reference) SpinProjection(particle, reference);
  return euler;
};

void lineparser::yyerror(char *s)
{
  fprintf (stderr, "%s:%d: %s\n", filename, linenumber, s);
}

int lineparser::yylex()
{
  static struct {
    char *Name;
    int token;
  } literalname[] = {
    {"Histogram", HISTOGRAM},
    {"Counter", COUNTER},
    {"Cut", CUT},
    {"NOT", NOT},
    {"TRUE", TTRUE},
    {"FALSE", TFALSE},
    {"not", NOT},
    {"true", TTRUE},
    {"false", TFALSE},
    {"Scalar", DEFSCALAR},
    {"Fourvector", DEFFOURVECTOR},
    {"FourVector", DEFFOURVECTOR},
    {"ScaleHistogram", SCALEHISTOGRAM},
    {"Normalize", NORMALIZE},
    {"Weight", WEIGHT},
    {"GlobalWeight", GLOBALWEIGHT},
    {"Interpolate", INTERPOLATE},
    {"Spectrometer", SPEC},
    {"SpectrometerA", SPECA},
    {"SpectrometerB", SPECB},
    {"SpectrometerC", SPECC},
    {"OHIPS", OHIPS},
    {"OOPS_A", OOPSA},
    {"OOPS_B", OOPSB},
    {"OOPS_C", OOPSC},
    {"OOPS_D", OOPSD},
    {"WriteEvent", WRITEEVENT},
    {"Print", PRINT},
    {"Display", DISPLAY},
    {"Beam", BEAM},
    {"File", FILES},
    {"Directory", DIR},
    {"Label", LABEL},
    {"Convert", CONVERT},
    {"NtupleIdentity", NTID},
    {"NtupleBlock", NTBLOCK},
    {"Ntuple", NTUPLE},
    {"SpinBasis", SPINBASIS},
    {NULL, 0}
  };

  int c;
  while (((c = fgetc(parsefile)) == ' ') 
	 || (c == '\t') 
	 || (c == '°') ); // skip whitespace

  switch (c) {
  case '\\':
    c = fgetc(parsefile);
    linenumber++;
    return yylex();
    break;
  case '!':
    c = fgetc(parsefile);
    if (c == '=') return UNEQUAL;
    ungetc(c, parsefile);
    c = '!';
    break;
  case '=':
    c = fgetc(parsefile);
    if (c == '=') return EQUAL;
    ungetc(c, parsefile);
    c = '=';
    break;
  case '<':
    c = fgetc(parsefile);
    if (c == '=') return LE;
    ungetc(c, parsefile);
    c = '<';
    break;
  case '>':
    c = fgetc(parsefile);
    if (c == '=') return GE;
    ungetc(c, parsefile);
    c = '>';
    break;
  case '&':
    c = fgetc(parsefile);
    if (c == '&') return AND;
    ungetc(c, parsefile);
    c = '&';
    break;
  case '|':
    c = fgetc(parsefile);
    if (c == '|') return OR;
    ungetc(c, parsefile);
    c = '|';
    break;
  case '/':
    c = fgetc(parsefile);
    if (c == '/') {
      while (c !='\n' && c != EOF) c=fgetc(parsefile); 
      return '\n';
    }
    ungetc(c,parsefile);
    c='/';
    break;
  }

  if (c == '!') {
    while (c !='\n' && c != EOF) c=fgetc(parsefile); 
    return c;
  }
  if (c == '"') { 
    int i = c = 0;
    char function[256];
    while  (c != '"' && c != EOF) {
      c = fgetc(parsefile);
      if (c=='\\') {
	c = fgetc(parsefile);
	if (c==EOF) break;
	switch (c) {
	case 'n':  c = '\n'; break;
	case 't':  c = '\t'; break;
	case 'v':  c = '\v'; break;
	case 'b':  c = '\b'; break;
	case 'r':  c = '\r'; break;
	case 'f':  c = '\f'; break;
	case 'a':  c = '\a'; break;
	case '\\': c = '\\'; break;
	case '?':  c = '?'; break;
	case '\'': c = '\''; break;
	case '"':  c = '"'; break;
	default: break;
	}
      }                    
      function[i++] = c;
    }
    function[i-1]=0;
    if (new mmChar(yylval.str = new char[sizeof(function)+1]));
    strcpy(yylval.str, function); 
    return STRING;
  }

  if (isdigit (c)) {
    double scl;
    ungetc(c,parsefile);
    if (fscanf (parsefile,"%lf", &scl));
    if (new mmDouble(yylval.val = new double(scl)));
    return NUM;
  }

  if (isalpha(c) || c=='.' || c=='@') {
    int i = 0;
    char function[255];
    while (isalnum(c) || (c == '.') || (c == '_') ||
	   (c == '@') || (c == '[') || (c == ']')) {
      function[i++]=c;
      if ((c=fgetc(parsefile))==' ') {
        function[i]=0;
        int j=-1;
        while(literalname[++j].token) 
          if (!strcmp(literalname[j].Name,function))
            return literalname[j].token;
        while ((c = fgetc(parsefile)) == ' ');
      }
    }

    function[i]=0;
    ungetc(c,parsefile);

    i = -1; 
    while(literalname[++i].token) 
      if (!strcmp(literalname[i].Name,function))
	return literalname[i].token;
    if ((yylval.str = Cola_Scalar::namescalar(function))) return SCALAR;
    if ((yylval.str = Cola_Fourvector::namefour(function))) return FOURVECTOR;
    if ((yylval.str = isfunction( function))) return FUNC;
    if ((yylval.str = isfunction2(function))) return FUNC2;
    if (!strncmp(function, "@mezzo.", 7)) {
      static double dnull = 0.0;
      yylval.val = (mreader ? mreader->init(function+7, 0) : &dnull);
      return MEZZOITEM;
    }
    if (function[0]=='@') {
      AquaNode *item = (aintree ? aintree->find(function+1, 0) : NULL);
      if (item && new mmAqua(yylval.itm = item)) return AQUANODE;
      item =  (outtree ? outtree->find(function+1, 0) : NULL);
      if (item && new mmAqua(yylval.itm = item)) return AQUANODE;

      int ind = -1;
      char *bra = strchr(function, '[');
      if (bra) {
	ind = atoi(bra+1);
	*bra = 0;
      }
      int i = -1;
      while (lname[++i].Name) {
	if (!strcmp(lname[i].Name, &function[1])) {

	  if (lname[i].type < 1) 
	    fprintf(stderr, "'%s' is not a scalar value\n", function);
          if(lname[i].type == 1) 
	    yylval.val = (double *) lname[i].addr; 
	  else {
	    if (ind<0 || ind >=lname[i].type) 
	      fprintf(stderr,"Index out of bounds for array %s\n",
		      &function[1]); 
	    yylval.val = &((double *)lname[i].addr)[ind]; 
	  }
	  return RUNDBITEM;
	}
      }
      fprintf(stderr, "'%s' not known\n", function);
    }
    if (new mmChar(yylval.str = new char[sizeof(function)+1]));
    strcpy(yylval.str, function); 
    return NAME;
  }
  if (c == 0 || c==EOF ) return 0;
  return c;
}

void
parse_file(AquaTree *atree, AquaTree *out, MezzoReader *mezzoReader,
	   reaction *Reaction, char *fname,
	   const char *Label, const char *Unit)
{
  lineparser *pr = new lineparser;
  aintree = atree;
  outtree = out;
  mreader = mezzoReader;
  eventDisplay = new EventDisplay(atree,out);
  writeevent = zero;
  globalweight = NULL;
  char *name = fname;
  filename = fname;

  extern char *ntvarn[MAX_NT_VAR];
  extern char *ntdes[MAX_NT_VAR];
  extern char *ntbck[MAX_NT_VAR];
  extern char *nttitle;
  extern char *nttitle;
  extern int  ntiden, ntnvar;

  Cola_histogram::delete_all();
  Cola_Scalar::delete_all();
  Cola_Counter::delete_all();
  Cola_Fourvector::delete_all();
  Cola_print::delete_all();
  memManager::delete_all();
  Cola_histogram::zLabel = Label;
  Cola_histogram::zUnit  = Unit;
 
  if (!(parsefile = fopen(fname, "r"))) {
    sprintf(name = new char[strlen(fname)+5], "col/%s", fname);
    if (!(parsefile = fopen(name, "r"))) { 
      perror(fname); 
      return;
    }
  }
  fclose(parsefile);
  filename = fname;

  char command[10240] = GNUCPP;
  strcat(command," \"-w\" ");
  char *rstr = new char[strlen(Reaction->getName())+1];
  strcpy(rstr,Reaction->getName());
  char *token = strtok(rstr,";");
  while(token) {
    char reac[255];
    while(*token==' ') token++;
    sprintf(reac," \"-Areaction(\\\"%s\\\")\" ",token);
    strcat(command,reac);
    if (Unit[0]) strcat(command,"\"-Dsimulation\" ");
    token=strtok(NULL,";");
  }
  delete [] rstr;
  char *quote;
  while ((quote = strrchr(command,'\'')))
    memmove(quote, quote+1, strlen(quote)); 
  if (Reaction->getA())   strcat(command,"\"-Aspectrometer(A)\" ");
  if (Reaction->getB())   strcat(command,"\"-Aspectrometer(B)\" ");
  if (Reaction->getC())   strcat(command,"\"-Aspectrometer(C)\" ");
  if (Reaction->getD())   strcat(command,"\"-Aspectrometer(D)\" ");
  if (Reaction->getO())   strcat(command,"\"-Aspectrometer(O)\" ");
  if (Reaction->getH3())  strcat(command,"\"-Aspectrometer(H3)\" ");
  if (Reaction->getTOF()) strcat(command,"\"-Aspectrometer(TOF)\" ");
  if (Reaction->getNPOL()) strcat(command,"\"-Aspectrometer(NPOL)\" ");
  if (Reaction->getNDET()) strcat(command,"\"-Aspectrometer(NDET)\" ");
  if (Reaction->getSI())   strcat(command,"\"-Aspectrometer(SI)\" ");
  if (Reaction->getKAOS()) strcat(command,"\"-Aspectrometer(KAOS)\" ");
  strcat(command, name);
  if (!(parsefile = popen(command,"r"))) { perror(name); return; }
  pr->yyparse();
  fclose(parsefile);  

  if (ntvars)
  {
    for (int k=0; k < MAX_NT_VAR; k++)
      {
	ntvarn[k] = ntvarname[k];
	ntdes[k] = ntdescr[k];
	ntbck[k] = ntblock[k];
      }
    ntiden = ntid;
    nttitle = nttit;
    ntnvar = ntvars;
  }
}

int
evaluate_expressions(const FourVector& A, const FourVector& B,
		     const FourVector& C, const FourVector& D,
		     const FourVector& O, const FourVector& H3,
                     const FourVector& TOF, const FourVector& NPOL,
		     const FourVector& NDET, const FourVector& SI,
		     const FourVector& KAOS,
		     const FourVector& beam, const double weight)
{
  writeevent->reset();
  dodisplay->reset();
  spinBasis[0]->reset();
  spinBasis[1]->reset();
  spinBasis[2]->reset();
  AnaX->reset();
  AnaY->reset();
  Cola_histogram::reset_all();
  Cola_print::reset_all(); 
  Cola_Counter::reset_all();

  if (speca) {
    speca->setValid((A.energy() > 0) ? ISFALSE : NOTVALID); 
    speca->value = A;
  }
  if (specb) {
    specb->setValid((B.energy() > 0) ? ISFALSE : NOTVALID); 
    specb->value = B;
  }
  if (specc) {
    specc->setValid((C.energy() > 0) ? ISFALSE : NOTVALID); 
    specc->value = C;
  }
  if (specd) {
    specd->setValid((D.energy() > 0) ? ISFALSE : NOTVALID); 
    specd->value = D;
  }
  if (speco) {
    speco->setValid((O.energy() > 0) ? ISFALSE : NOTVALID); 
    speco->value = O;
  }
  if (spech3) {
    spech3->setValid((H3.energy() > 0) ? ISFALSE : NOTVALID); 
    spech3->value = H3;
  }
  if (spectof) {
    spectof->setValid((TOF.energy() > 0) ? ISFALSE : NOTVALID); 
    spectof->value = TOF;
  }
  if (specnpol) {
    specnpol->setValid((NPOL.energy() > 0) ? ISFALSE : NOTVALID); 
    specnpol->value = NPOL;
  }
  if (specndet) {
    specndet->setValid((NDET.energy() > 0) ? ISFALSE : NOTVALID); 
    specndet->value = NDET;
  }
  if (specsi) {
    specsi->setValid((SI.energy() > 0) ? ISFALSE : NOTVALID); 
    specsi->value = SI;
  }
  if (speck) {
    speck->setValid((KAOS.energy() > 0) ? ISFALSE : NOTVALID); 
    speck->value = KAOS;
  }
  if (beamv) {
    beamv->setValid((beam.energy() > 0) ? ISFALSE : NOTVALID); 
    beamv->value = beam;
  }

  static FILE *output = 
    (getenv("COLAPRINT") ? fopen((char *) getenv("COLAPRINT"),"w") : stdout);

  double w = weight;
  if (globalweight) {
    globalweight->reset();
    w = globalweight->isValid() ? globalweight->evaluate() : 0;
  }
  Cola_histogram::fill_all(w);
  Cola_Counter::fill_all(w);
  if (output) {
    Cola_print::print_all(output);
    fflush(output);
  }
  if (dodisplay->isValid())
    if (dodisplay->evaluate()) {
      eventDisplay->doDisplay();
    }
  return (int) (writeevent->isValid() ? writeevent->evaluate() : 0);
}


void
printCounter(std::ostream * out)
{
  if (!out) return;

  Cola_Counter *cc = Cola_Counter::first;
  while (cc) {
    char line[255];
    cc->toString(line);
    (*out) << line << std::endl;
    cc = cc->next;
  }
}
