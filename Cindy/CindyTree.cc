//
// Created 1993-96 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyTree.cc,v 4.21 2003-08-05 09:36:29 distler Exp $
//
// Implementation of class CindyTree
//

#define _POSIX_C_SOURCE 2
#include "Cindy++.h"
#include "CindyNode.h"
#include "CindyStream.h"
#include "CindyCompiler.h"
#include <unistd.h>

char *SharedMemoryKeyFile = "shm";

CindyTree::CindyTree(int lvl) : CindyBase(lvl)
{
  ctx	 = NULL;
  flags	 = dataReady = 0;
  in	 = NULL;
  input  = "-";
  out    = NULL;
  output = NULL;
}

int
CindyTree::rewind(int events)
{
    if (!in || (in->rewind() == -1)) return -1;

    if (events > 0)
	in->setRemainingEvents(events);

    return 0;
}

int
CindyTree::iStream(int events)
{
    if (in) delete in;

    if (flags & gnutarFlag) {
	if (flags & zipFlag) 
	    in = new CindyStream(input, GNUTAR " xzOf ");
	else
	    in = new CindyStream(input, GNUTAR " xOf ");
    }
    else {
	if (flags & zipFlag) 
	    in = new CindyStream(input, GZIP " -cd ");
	else
	    in = new CindyStream(input);
    }

    if (!in) return -1;

    if (events > 0)
	in->setRemainingEvents(events);

    if (flags & debugFlag)
	in->debugOn();

    return 0;
}

int
CindyTree::oStream(char *run)
{
    if (out) delete out;

    if (flags & zipFlag) {
	if (getenv("G" "ZIP"))
	    out = new CindyOutStream(output, GZIP);
	else
	    out = new CindyOutStream(output, GZIP " -3");
    }
    else
	out = new CindyOutStream(output,NULL);

    if (!out) return -1;

    if (run) {
      struct tm tms;
      if (6 == sscanf(run, "%2d%2d%2d%2d%2d%2d",
		      &tms.tm_year, &tms.tm_mon,  &tms.tm_mday, 
		      &tms.tm_hour, &tms.tm_min,  &tms.tm_sec)) {
	tms.tm_mon--; tms.tm_isdst = -1;
	putRunName(run);
	putIdent(mktime(&tms), (flags & configFlag) ? 1 : 0);
      }
    } else if (flags & configFlag) putIdent(0, 1);

    return (out->getTag() == CindyOutStream::none) ? -1 : 0;
}

int
Srekursion(CindyTree *ct, char *root, CindyCount& count,
	   TPtr typ, const char *nam, const char *lnam, int lvl)
{
  if (typ->member == NULL) {
    count.position_align(typ->align);
    if (root) {
      if (!strcmp(SHORT_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (short *)(root+count.get_position()));
      else if (!strcmp(USHORT_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (unsigned short *)(root+count.get_position()));
      else if (!strcmp(INT_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (int *)(root+count.get_position()));
      else if (!strcmp(UINT_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (unsigned int *)(root+count.get_position()));
      else if (!strcmp(LONG_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (long *)(root+count.get_position()));
      else if (!strcmp(ULONG_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (unsigned long *)(root+count.get_position()));
      else if (!strcmp(FLOAT_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (float *)(root+count.get_position()));
      else if (!strcmp(DOUBLE_NAME, typ->sname))
	ct->add(lvl,count.get_level(lvl),count.get_level(lvl-1)-1,
		nam, (double *)(root+count.get_position()));
      else {
	std::cerr << "type '" << *typ->name << "' not supported" << std::endl;
	exit(errno);
      } 
    }
    count.check(lnam);
    count.inc(lvl,typ->len);
    return(0);
  }

  if (root) ct->add(lvl,count.get_level(lvl),
		    (lvl ? count.get_level(lvl-1)-1 : 0), nam);
  count.inc(lvl);
  lvl++;

  Member * mem = typ->member;
  while (mem != NULL) {
    if (mem->array()) {
      if (root) {
	char *memname = new char[strlen(mem->cname())+2];
	strcpy(memname+1, mem->cname()); memname[0] = '.';
	ct->add(lvl, count.get_level(lvl),
		(lvl ? count.get_level(lvl-1)-1 : 0), memname);
	delete memname;
      }
      count.inc(lvl);
      for (int arr = 0; arr < mem->array(); arr++) {
	char *name = new char[strlen(lnam)+strlen(mem->cname())+32];
	sprintf(name, "%s.%s[%d]", lnam, mem->cname(), arr);
	char *bracket = rindex(name, '[');
	Srekursion(ct, root, count, mem->type(), bracket, name, lvl+1);
	delete name;
      }
    }
    else {
      char *name = new char[strlen(lnam)+strlen(mem->cname())+2];
      sprintf(name, "%s.%s", lnam, mem->cname());
      char *point = rindex(name, '.');
      Srekursion(ct, root, count, mem->type(), point, name, lvl);
      delete name;
    }
    mem = mem->next();
  }

  return(0);
} // end of Srekursion()


int
CindyTree::nextArchiveUnpackAndCompile()
{
  if ((flags & noDecodeFlag) && !(flags & configFlag)) return 0;

  flags &= ~printExpFlag;
  if (system("rm -rf tmp"));

  const int useLogFile		= 1;
  const int useHeaderFile	= 2;

  int useFile = 0;
  char *logFile = NULL;
  char *headerFile = NULL;

  do {
    if (!in) return -1;

    if (in->getHeader() != 1) { // 1 = data available
      closeIO();
      return -1;
    }

    if (in->configurationData())
      in->unpackConfigData("tmp");
    else if (in->identificationData())
      in->getIdent();
    else if (in->runNameData())
      in->getRunName();

    if (in->getExperimentString()) {
      logFile = new char[strlen(in->getExperimentString())+9];
      sprintf(logFile, "tmp/%s.log", in->getExperimentString());
      headerFile = new char[strlen(in->getExperimentString())+7];
      sprintf(headerFile, "tmp/%s.h", in->getExperimentString());

      while (!useFile) {
	std::ifstream hfd(headerFile);
	if (hfd.is_open()) {
	  useFile = useHeaderFile;
	  //	  continue;
	}
	if (flags & debugFlag)
	  std::cerr << "checking: " << headerFile << ' '
		    << useFile << std::endl;

	std::ifstream lfd(logFile);
	if (lfd.is_open()) {
	  useFile = useLogFile;
	}
	if (flags & debugFlag)
	  std::cerr << "checking: " << logFile << ' ' << useFile << std::endl;

	if (!useFile) sleep(1);
      }
    }
  } while (!useFile);

  if (flags & noDecodeFlag) return 0;

  char *cpp_command = new char[strlen(GNUCPP)+strlen(logFile)+8];
  switch (useFile) {
  case useLogFile:
    sprintf(cpp_command, GNUCPP " -Itmp %s", logFile);
    break;
  case useHeaderFile:
    sprintf(cpp_command, GNUCPP " -Itmp %s", headerFile);
    break;
  default:
    break;
  }

  FILE *cppin = popen(cpp_command, "r");
  delete cpp_command;

  CindyCompiler cc(fileno(cppin));

  if (cc.yyparse()) {
    perror("yyparse");
    return(errno);
  }

  fclose(cppin);

  if (flags & debugFlag)
    PrintVars("-",in->getExperimentString(),cc.Vars,"Cindy++");

  VPtr var = cc.Vars;
  const char *exp = in->getExperimentString();

  while (var != NULL) {
    if (!strlen(exp)) break;
    if (!strcmp(var->name, exp)) break;
    var = var -> next;
  }

  if (var == NULL) {
    std::cerr << "Cindy++: " << exp << " not defined" << std::endl;
    return(errno);
  }

  CindyCount count;
  char *root = NULL;

  Srekursion(this, root, count, var->typ, exp, exp, 0);

  int lvl;
  for (lvl=0; count.get_level(lvl)>0; lvl++);

  alloc(lvl);
  if (flags & debugFlag)
    std::cerr << "alloc(" << lvl << ");" << std::endl;
  while (lvl--) {
    alloc(lvl,count.get_level(lvl));
    if (flags & debugFlag)
      std::cerr << "alloc(" << lvl << ',' << count.get_level(lvl)
		<< ");" << std::endl;
  }
  
  root = new char[count.get_position()];
  setroot(root,count.get_position(),count.length());
  if (flags & debugFlag)
    std::cerr << "setroot(root," << var->typ->len << ','
	      << count.length() << ");" << std::endl;

  count.reset();
  Srekursion(this, root, count, var->typ, exp, exp, 0);

  if (flags & identFlag) {
    std::cout.setf(std::ios::left,std::ios::adjustfield);
    std::cout.width(decodeFieldWidth-24);
    std::cout << in->getIdentificationString()
	      << in->getTimeOfDataTakingString();
    std::cout.setf(std::ios::right,std::ios::adjustfield);
    int oldfill = std::cout.fill('-');
    std::cout.width(decodeFieldWidth);
    std::cout << "---" << std::endl;
    std::cout.fill(oldfill);
  }

  return 0;
} // end of CindyTree::nextArchiveUnpackAndCompile()


int
CindyTree::closeIO()
{
  if (in) {
    delete in;
    in = NULL;
    input  = "-";
  }
  if (out) {
    delete out;
    out = NULL;
    output = NULL;
  }
  if (ctx) {
    ctx->closeIO();
  }
  return 0;
}

int
CindyTree::analyseLastEvent()
{
    if (!in || !in->normalEventData()) return -1;

    clearData();
    (*tree)->fastAnalyse(in->getData(), in->getLengthOfDataField());

    return 0;
}

int
CindyTree::analyseNextEvent()
{
  int fertig;

  if (ctx) {
    if (ctx->dataIsReady() && ctx->out) {
      ctx->putPackedData();
      ctx->out->putIdentX(ctx->configDate,
			  (in ? in->getTimeOfDataTaking() : 0));
    }
  }

  do {

    if (!in) return -1;

    switch (in->getHeader()) {
    case 0:         // no data available
      closeIO();
      return -1;
      break;
    case -1:        // time out during select
      return 1;
      break;
    default:
      break;
    }

    if ((fertig = in->normalEventData())) {
      clearData();
//      (*tree)->analyse(in->getData(),in->getLengthOfDataField());
      (*tree)->fastAnalyse(in->getData(),in->getLengthOfDataField());
    } else if (in->identificationData()) {
      if (ctx) {
	if (ctx->out)
	  ctx->out->putOldIdent(in->getIdent(), in->getLengthOfDataField());
      } else {
	if (flags & outputFlag) {
	  if (out) out->putData(in->getHeaderPtr(),in->getIdent());
	} else in->getIdent();
      }
      if ((flags & printExpFlag) &&
	  (strcmp(getExperimentName(),in->getExperimentString()))) {
	if (flags & ignoreExpFlag) {
	  std::cerr << "WARNING:\tanalysis program is configured for '"
		    << getExperimentName() << "'\n\t\tdata is configured for '"
		    << in->getExperimentString() << "'\n";
	} else {
	  std::cerr << "Fatal ERROR:\tanalysis program is configured for '"
		    << getExperimentName() << "'\n\t\tdata is configured for '"
		    << in->getExperimentString() << "'\n";
	  return -1;
	}
      }
      flags &= ~printExpFlag;
    } else if (in->oldIdentData()) {
      if (ctx) {
	if (ctx->out)
	  ctx->out->putOldIdent(in->getOldIdent(), in->getLengthOfDataField());
      } else {
	if (flags & outputFlag) {
	  if (out) out->putData(in->getHeaderPtr(),in->getOldIdent());
	} else in->getOldIdent();
      }
    } else if (in->runNameData()) {
      in->getRunName();
    } else if (flags & outputFlag) {
      if ((in->configurationData() && (in->getIdentificationString()==NULL))
	  || (in->oldIdentData())) {
	if (!ctx && out) out->putData(in->getHeaderPtr(),in->getData());
      }
    }
    
  } while (!fertig);

  return 0;
}

int
CindyTree::putEventData()
{
  return((in && out) ? out->putData(in->getHeaderPtr(),in->getData()) : -1);
}

int
CindyTree::putIdent(time_t t, int cfg)
{
  if (out) {
    if (cfg) out->putTar(configuration,configSize);
    out->putIdent(configDate, t);
  }
  return 0;
}

int
CindyTree::putComment(const char * str)
{
  return (out ? out->putComment(str) : -1);
}

int
CindyTree::putOldIdent(const char * str, time_t t)
{
  if (!out) return -1;

  int tpos = strlen(str)+1;
  char * oldIdent = new char[tpos+5];
  memset(oldIdent, 0, tpos+5);
  strcpy(oldIdent, str);
  out->mem_swap(oldIdent+tpos, &t);
  out->putOldIdent((Uint2 *)oldIdent, (tpos+5)/2);
  delete oldIdent;

  return 0;
}

int
CindyTree::putRunName(const char * runName)
{
  return (out ? out->putRunName(runName) : -1);
}

int
CindyTree::packEventData(void *ptr,int len)
{
  CindyNode *node, *node2;
  node = node2 = base[((char *)ptr - dataroot)>>1];

  while (NULL != node)
    node = node->mark(dataReady);
  while (--len > 0)
    (++node2)->mark(dataReady);

  return 0;
}

int
CindyTree::putPackedData()
{
  if (!dataReady) return 0;

  short *data = new short[dataReady];
  int packSize = (*tree)->pack(data);
  if (out) out->putData(data,packSize);
  delete data;
	      
  return dataReady=0;
}


int
CindyTree::putPackedData(int count, int code)
{
  if (!dataReady) return 0;

  short *data = new short[dataReady];
  int packSize = (*tree)->pack(data);
  if (out) out->putData(data,packSize,count,code);
  delete data;
	      
  return dataReady=0;
}

int
CindyTree::decodeNextEvent()
{
  switch (in->getHeader()) {
  case 0:         // no data available
    return -1;
    break;
  case -1:        // time out during select
    return 1;
    break;
  default:
    break;
  }

  if (in->normalEventData()) {
    if (!(flags & noDecodeFlag)) {
      if (flags & verboseFlag) {
	CindyNode::setFieldWidth(longestName);
	(*tree)->decode(in->getData(),in->getLengthOfDataField(),"");
	std::cout.setf(std::ios::right,std::ios::adjustfield);
	int oldfill = std::cout.fill('-');
	std::cout.width(decodeFieldWidth);
	std::cout << "---";
	std::cout.fill(oldfill);
	std::cout.width(7);
	std::cout << in->getEventCounter() << std::endl;
      }
      else {
	(*tree)->decode(in->getData(),in->getLengthOfDataField());
	std::cout << std::endl;
      }
    }
    return 0;
  }

  if (in->identificationData()) {
    in->getIdent();
    if ((flags & printExpFlag) &&
	(strcmp(getExperimentName(),in->getExperimentString()))) {
      std::cerr << "Fatal ERROR:\tanalysis program is configured for '"
		<< getExperimentName() << "'\n\t\tdata is configured for '"
		<< in->getExperimentString() << "'\n";
      return -1;
    }
    flags &= ~printExpFlag;
    if (flags & identFlag) {
      std::cout.setf(std::ios::left,std::ios::adjustfield);
      std::cout.width(decodeFieldWidth-24);
      std::cout << in->getIdentificationString();
      std::cout << in->getTimeOfDataTakingString();
      std::cout.setf(std::ios::right,std::ios::adjustfield);
      int oldfill = std::cout.fill('-');
      std::cout.width(decodeFieldWidth);
      std::cout << "---" << std::endl;
      std::cout.fill(oldfill);
    }
    return 0;
  }

  if (in->oldIdentData()) {
      if (flags & identFlag) {
	  std::cout.setf(std::ios::left,std::ios::adjustfield);
	  std::cout.width(decodeFieldWidth-24);
	  std::cout << (char *)in->getOldIdent();
	  std::cout << in->getOldTimeOfDataTakingString();
	  std::cout.setf(std::ios::right,std::ios::adjustfield);
	  int oldfill = std::cout.fill('-');
	  std::cout.width(decodeFieldWidth);
	  std::cout << "---" << std::endl;
	  std::cout.fill(oldfill);
      }
      return 0;
  }

  if (in->runCommentData()) {
      if (flags & identFlag) {
	  std::cout << "Comment : " << (char *)in->getData();
	  std::cout.setf(std::ios::right,std::ios::adjustfield);
	  int oldfill = std::cout.fill('-');
	  std::cout.width(decodeFieldWidth);
	  std::cout << "---" << std::endl;
	  std::cout.fill(oldfill);
      }
      return 0;
  }

  if (in->runNameData()) {
      if (flags & identFlag) {
	  std::cout << "Name : " << (char *)in->getData() << std::endl;
	  std::cout.setf(std::ios::right,std::ios::adjustfield);
	  int oldfill = std::cout.fill('-');
	  std::cout.width(decodeFieldWidth);
	  std::cout << "---" << std::endl;
	  std::cout.fill(oldfill);
      }
      return 0;
  }

  if (in->errorMessageData()) {
      if (flags & identFlag) {
	  std::cout << "Message : " << (char *)in->getData();
	  std::cout.setf(std::ios::right,std::ios::adjustfield);
	  int oldfill = std::cout.fill('-');
	  std::cout.width(decodeFieldWidth);
	  std::cout << "---" << std::endl;
	  std::cout.fill(oldfill);
      }
      return 0;
  }

  if (in->configurationData()) {
    if (flags & configFlag)
      in->unpackConfigData("tmp");
    return 0;
  }

  if (in->specialEventData()) {
    if (flags & specialFlag)
      in->unpackSpecialData("special");
    return 0;
  }

  return 0;
}

void
CindyTree::decodeHelp(char *progname)
{
  std::cerr << std::endl << progname << ':' << std::endl;
  std::cerr << '\t' << "cindy++ decode program" << std::endl;
  std::cerr << " -c\t\t"      << "extract configuration." << std::endl;
  std::cerr << " -d\t\t"      << "debug output." << std::endl;
  std::cerr << " -e\t\t"      << "print experiment name." << std::endl;
  std::cerr << " -f <file>\t" << "input file. default: STDIN" << std::endl;
  std::cerr << " -g\t\t"      << "input is gnutar file." << std::endl;
  std::cerr << " -i\t\t"      << "print identification." << std::endl;
  std::cerr << " -n\t\t"      << "no decode output." << std::endl;
  std::cerr << " -p <pattern>\t" << "search pattern." << std::endl;
#ifdef HAVE_REGEX_H
  std::cerr << " -r <regex>\t"<< "search regular expression." << std::endl;
#endif
  std::cerr << " -s\t\t"      << "extract special data." << std::endl;
  std::cerr << " -v\t\t"      << "verbose output." << std::endl;
  std::cerr << " -z\t\t"      << "uncompress input." << std::endl << std::endl;
}

int
CindyTree::decodeGetOpt(int argc, char *argv[]) 
{
  int   opt;

  while (EOF != (opt = getopt(argc, argv, "cdef:ghinp:r:svz?"))) {
    switch (opt) {
    case 'c':
      flags |= configFlag;
      break;
    case 'd':
      flags |= debugFlag;
      break;
    case 'e':
      flags |= printExpFlag;
      break;
    case 'f':
      input = optarg;
      break;
    case 'g':
      flags |= gnutarFlag;
      break;
    case 'i':
      flags |= identFlag;
      break;
    case 'n':
      flags |= noDecodeFlag;
      break;
#ifdef HAVE_REGEX_H
    case 'r':
      CindyNode::setRegex(optarg);
      break;
#endif
    case 's':
      flags |= specialFlag;
      break;
    case 'p':
      CindyNode::setPattern(optarg);
      break;
    case 'v':
      flags |= verboseFlag;
      break;
    case 'z':
      flags |= zipFlag;
      break;
    default:
      decodeHelp(*argv);
      return -1;
    }
  }

  if ((flags & printExpFlag) && getExperimentName())
    std::cerr << *argv << ": configured for experiment '"
      << getExperimentName() << "'\n";
  flags |= printExpFlag;

  return iStream();
}

void
CindyTree::analyseHelp(char *progname)
{
  std::cerr << std::endl << progname << ':' << std::endl;
  std::cerr << '\t' << "cindy++ analysis program" << std::endl;
  std::cerr << " -c\t\t" << "clear Histograms. default: create" << std::endl;
  std::cerr << " -d\t\t" << "debug output." << std::endl;
  std::cerr << " -e\t\t" << "print experiment name." << std::endl;
  std::cerr << " -E\t\t" << "do not stop if configuration is wrong."
	    << std::endl;
  std::cerr << " -f <file>\t" << "input  file. default: STDIN" << std::endl;
  std::cerr << " -g\t\t" << "input is gnutar file." << std::endl;
  std::cerr << " -n <num>\t" << "stop after # events." << std::endl;
  std::cerr << " -o <file>\t" << "output file. default: none" << std::endl;
  std::cerr << " -u\t\t" << "update Histograms. default: create" << std::endl;
  std::cerr << " -z\t\t" << "uncompress input." << std::endl << std::endl;
}

int
CindyTree::analyseGetOpt(int argc, char *argv[], CindyTree *cx)
{
  int   opt, events=0;
  char *runname=NULL;

  while (EOF != (opt = getopt(argc, argv, "Ecdef:ghn:o:r:uz?"))) {
    switch (opt) {
    case 'E':
      flags |= ignoreExpFlag;
      break;
    case 'c':
      flags |= clearHistoFlag;
      flags &= ~updateHistoFlag;
      break;
    case 'd':
      flags |= debugFlag;
      break;
    case 'e':
      flags |= printExpFlag;
      break;
    case 'f':
      input = optarg;
      break;
    case 'g':
      flags |= gnutarFlag;
      break;
    case 'n':
      events = atoi(optarg);
      break;
    case 'o':
      output = optarg;
      flags |= outputFlag;
      break;
    case 'r':
      runname = optarg;
      break;
    case 'u':
      flags |= updateHistoFlag;
      flags &= ~clearHistoFlag;
      break;
    case 'z':
      flags |= zipFlag;
      break;
    default:
      analyseHelp(*argv);
      return -1;
    }
  }

  if (flags & clearHistoFlag)
    histomode = CLEAR_HISTO;
  else if (flags & updateHistoFlag)
    histomode = UPDATE_HISTO;
  else
    histomode = CREATE_HISTO;

  if ((flags & printExpFlag) && getExperimentName())
    std::cerr << *argv << ": configured for experiment '"
	      << getExperimentName() << "'\n";
  flags |= printExpFlag;

  if (output) {
    if (cx) {
      ctx = cx;
      ctx->putOpt(flags|configFlag,NULL,output,runname);
      flags &= ~outputFlag;
      output = NULL;
    }
    else if (oStream()) return -1;
  }
  return iStream(events);
}

long
CindyTree::openShmKeyFile(long key)
{
  if (key == -1) {
    std::ifstream fshm(SharedMemoryKeyFile);
    fshm >> key;
    fshm.close();
  }
  else {
    std::ofstream fshm(SharedMemoryKeyFile);
    fshm << key << std::endl;
    fshm.close();
  }

  return key;
}

int
CindyTree::putOpt(int flag, char *in, char *out, char *run)
{
  int err = 0;

  flags = flag;
  if (in) {
    input = in;
  }
  err = iStream();
  if (err) return err;
  if (out) {
    output = out;
    err = oStream(run);
  }
  return err;
}

unsigned short
CindyTree::eventCount()
{
  return (in ? in->getEventCounter() : 0);
}

unsigned short
CindyTree::eventCode()
{
  return (in ? in->getEventCode() : 0);
}

const char *
CindyTree::getRunName()
{
  return (in ? in->getRunNameString() : "?");
}

time_t
CindyTree::getTimeOfDataTaking()
{
  return (in ? in->getTimeOfDataTaking() : 0);
}

const char *
CindyTree::getInputFilename()
{
  return (in ? in->getFilename() : "?");
}

const char *
CindyTree::getOutputFilename()
{
  return (out ? out->getFilename() : "?");
}

const char *
CindyTree::getExperimentName()
{
  return (*tree ? (*tree)->tellname() : (const char *)NULL);
}

CindyTree::~CindyTree()
{
  if (tree)     delete tree;
  if (base)     delete base;
  if (in)       delete in;
  if (out)      delete out;
  if (level)    delete level;
  if (ok_flags) delete ok_flags;
}
