//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/TMA/ftmInit.cc,v 1.6 2002-12-12 16:51:12 a1online Exp $
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftmInit.h"

ftmInit::ftmInit(const char * initfile)
{
  yerrs = -1;

  if (initfile==NULL) return;
  if (NULL == (file = fopen(initfile,"r"))) {
    char tmpbuf[256];
    sprintf(tmpbuf,"ftmInit: '%s'",initfile);
    perror(tmpbuf);
    return;
  }
  lex = new ftmInitLEX(file);

  yerrs = 0;
  yyparse();

  delete lex;

  fclose(file);
  file = NULL;

  if (yerrs) {
    std::cerr << "WARNING: matrix file '" << initfile
	      << "' contains errors" << std::endl;
  } else {
    ftmEntry *tmp;
    for (tmp=ftmEntries; tmp; tmp=tmp->getNext())
      tmp->findParents(ftmEntries);
  }
}

int
ftmInit::yylex()
{
  yylloc.first_line = lex->lineno;
  int token	    = lex->yylex(&yylval);
  yylloc.last_line  = lex->lineno;
  yylloc.text	    = (char *)lex->yytext;

  return token;
}

void
ftmInit::yyerror(char *m)
{
  yerrs++;
  std::cerr << " - line " << yylloc.first_line << ": "
	    << m << " at token \'" << yylloc.text << '\'' << std::endl;

  if (yerrs>=20) exit(errno);
}

// I have to put the destructor code here
// %define DESTRUCTOR_CODE has no effect
ftmInit::~ftmInit() {
  delete ftmEntries;
  delete xOffsets;
  delete thOffsets;
  delete yOffsets;
  delete phOffsets;
}

int
ftmInit::getEntries(ftmEntry **entries)
{
  if (yerrs || !ftmEntries) {
    std::cerr << "WARNING: using default (unity) matrix initialization"
	      << std::endl;
    delete ftmEntries;
    ftmEntries = new ftmEntry(0,0,0,1,0.0,0.0,0.0,1.0,0.0);
    ftmEntries->putEntry(new ftmEntry(0,0,1,0,0.0,0.0,1.0,0.0,0.0));
    ftmEntries->putEntry(new ftmEntry(0,1,0,0,0.0,1.0,0.0,0.0,0.0));
    ftmEntries->putEntry(new ftmEntry(1,0,0,0,1.0,0.0,0.0,0.0,0.0));
  }

  ftmEntry * entry = ftmEntries;
  int count = 0;

  while (entry) { // count number of entries
    count++;
    entry = entry->getNext();
  }

  *entries = ftmEntries;
  ftmEntries = NULL; // that way the destructor will not destroy the list
  return count;
}

int
ftmInit::getEntries(int **ix, int **ith, int **iy, int **iph, double **dp,
		    double **th, double **y0, double **ph, double **len)
{
  static int    default_index[7] = { 0,   0,   0,   1,   0,   0,   0   };
  static double default_value[7] = { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 };

  if (yerrs || !ftmEntries) {
    std::cerr << "WARNING: using default (unity) matrix initialization"
	      << std::endl;
    *ix  = new int[4]; memcpy(*ix,  default_index+3, 4*sizeof(int));
    *ith = new int[4]; memcpy(*ith, default_index+2, 4*sizeof(int));
    *iy  = new int[4]; memcpy(*iy,  default_index+1, 4*sizeof(int));
    *iph = new int[4]; memcpy(*iph, default_index,   4*sizeof(int));

    *dp  = new double[4]; memcpy(*dp,  default_value+3, 4*sizeof(double));
    *th  = new double[4]; memcpy(*th,  default_value+2, 4*sizeof(double));
    *y0  = new double[4]; memcpy(*y0,  default_value+1, 4*sizeof(double));
    *ph  = new double[4]; memcpy(*ph,  default_value,   4*sizeof(double));
    *len = new double[4]; memcpy(*len, default_value+3, 4*sizeof(double));

    return 4;
  }

  ftmEntry * entry = ftmEntries;
  int count = 0;

  while (entry) { // count number of entries
    count++;
    entry = entry->getNext();
  }

  *dp  = new double[count]; *ix  = new int[count];
  *th  = new double[count]; *ith = new int[count];
  *y0  = new double[count]; *iy  = new int[count];
  *ph  = new double[count]; *iph = new int[count];
  *len = new double[count]; 

  for (count=0, entry=ftmEntries; entry; count++, entry=entry->getNext()) {
    (*dp)[count]  = (*entry)[0]; (*ix)[count]  = (*entry)(0);
    (*th)[count]  = (*entry)[1]; (*ith)[count] = (*entry)(1);
    (*y0)[count]  = (*entry)[2]; (*iy)[count]  = (*entry)(2);
    (*ph)[count]  = (*entry)[3]; (*iph)[count] = (*entry)(3);
    (*len)[count] = (*entry)[4];
  }

  return count;
}

int
ftmInit::getOffset(double **offs, Number *numbers)
{
  int count = 0;
  Number * nums = numbers;

  while (nums) {
    count++;
    nums = nums->getNext();
  }

  if (!count) {
    *offs = NULL;
    return 0;
  }

  *offs = new double[count];

  for (count=0, nums = numbers; nums; count++, nums=nums->getNext()) {
    (*offs)[count] = (*nums)();
  }

  return count;
}

int
ftmInit::getXoffset(double **offs)
{
  return getOffset(offs, xOffsets);
}

int
ftmInit::getToffset(double **offs)
{
  return getOffset(offs, thOffsets);
}

int
ftmInit::getYoffset(double **offs)
{
  return getOffset(offs, yOffsets);
}

int
ftmInit::getPoffset(double **offs)
{
  return getOffset(offs, phOffsets);
}


#ifdef PARSERTEST

int
main(int argv, char *argc[])
{
  if (argv != 2) {
    std::cerr << "Usage : " << argc[0] << " <filename>" << std::endl;
    return -1;
  }

  int i, entries;
  int xEntries, thEntries, yEntries, phEntries;
  int *ix, *ith, *iy, *iph;
  double *dp, *th, *y0, *ph, *len;
  double *xOffs, *thOffs, *yOffs, *phOffs;

  ix = ith = iy = iph = NULL;
  dp = th  = y0 = ph  = len = NULL;
  xOffs = thOffs = yOffs = phOffs = NULL;
  entries = xEntries = thEntries = yEntries = phEntries = 0;

  for (i = 0; i < 20000; i++) { // test for memory leakage

    delete ix;    delete ith;    delete iy;    delete iph;
    delete dp;    delete th;     delete y0;    delete ph;     delete len;
    delete xOffs; delete thOffs; delete yOffs; delete phOffs; 

    ftmInit * init = new ftmInit(argc[1]);
    entries = init->getEntries(&ix, &ith, &iy, &iph, &dp, &th, &y0, &ph, &len);
    xEntries  = init->getXoffset(&xOffs);
    thEntries = init->getToffset(&thOffs);
    yEntries  = init->getYoffset(&yOffs);
    phEntries = init->getPoffset(&phOffs);

    delete init;
  }

  for (i = 0; i < entries; i++) {
    std::cout << ix[i]  << ' ' << ith[i] << ' ' << iy[i] << ' ' << iph[i] << "  "
	 << dp[i]  << ' ' << th[i]  << ' ' << y0[i] << ' ' << ph[i]  << ' '
	 << len[i] << std::endl;
  }

  std::cout << "x  offset : ";
  for (i = 0; i < xEntries; i++) std::cout << " " << xOffs[i];
  std::cout << std::endl;

  std::cout << "th offset : ";
  for (i = 0; i < thEntries; i++) std::cout << " " << thOffs[i];
  std::cout << std::endl;

  std::cout << "y  offset : ";
  for (i = 0; i < yEntries; i++) std::cout << " " << yOffs[i];
  std::cout << std::endl;

  std::cout << "ph offset : ";
  for (i = 0; i < phEntries; i++) std::cout << " " << phOffs[i];
  std::cout << std::endl;

  delete ix;    delete ith;    delete iy;    delete iph;
  delete dp;    delete th;     delete y0;    delete ph;     delete len;
  delete xOffs; delete thOffs; delete yOffs; delete phOffs; 

  return 0;
}

#endif /* PARSERTEST */
