#								 -*-makefile-*-
# Created 1996 by	Michael O. Distler, Harald Merkel,
#			Institut für Kernphysik,
#			Johannes Gutenberg-Universität Mainz
#
# $Header: /tmp/cvsroot/Cola/Makefile.inc.in,v 1.41 2008-02-27 10:46:15 patrick Exp $
#
# global makefile
#

topdir		= .

SHELL		= /bin/sh
CC		= gcc
CPP		= gcc -E
GNUCPP		= gcc -E -P -x c
CXX		= g++
F77		= f77
FC		= gfortran
INSTALL		= /usr/bin/install -c
INSTALL_PROGRAM	= ${INSTALL}
INSTALL_DATA	= ${INSTALL} -m 644
RANLIB		= ranlib

AQUACONFIG	= /usr/local/a1/bin/AquaConfig
BISONXX		= ../Bison/Bison++
BISONFLAGS	= -S $(srcdir)/../Bison/bison.cc -H $(srcdir)/../Bison/bison.h
CINDY		= ../Cindy/Cindy++
FLEXXX		= ../Flex/Flex++
FLEXFLAGS	= -S$(srcdir)/../Flex/flexskel.cc -H$(srcdir)/../Flex/flexskel.h
FIND		= find
GZIP		= gzip
YACC		= bison -y

DEFS		= -DHAVE_CONFIG_H
OPTS		=  -O2 -g
LIBS		= -lnsl 
WARN		= -Wall -Wno-write-strings
ISSHARED	= 1
ISSTATIC	= 0

prefix		= /usr/local/a1
exec_prefix	= /usr/local/a1
binprefix	=
manprefix	=

bindir		= $(exec_prefix)/bin
libdir		= $(exec_prefix)/lib
incdir		= $(prefix)/include
docdir		= $(prefix)/doc
mandir		= $(prefix)/man/man1
manext		= 1

sharedir	= $(prefix)/share
exampledir	= $(sharedir)/examples

PGLIBS		= 
PGINCL		= -I/usr/include/postgresql -I/usr/include

GSLLIBS		= -L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas -lm
GSLINCL		= -I/usr/include

ARCH		= x86_64-unknown-linux-gnu
CERNLIB         = /usr/lib/x86_64-linux-gnu
ROOTLIB		= 1
ROOT_CFLAGS	= -pthread -m64 -I/usr/include/root
ROOT_LIBS	= -L/usr/lib/x86_64-linux-gnu -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

FFLAGS		= -g -O2 -ffixed-line-length-none
FLIBS		= -lf2c -lgfortran
FCFLAGS		= -g -O2

X_CFLAGS	= 
X_LIBS		= 
MOTIFLIBS	= $(X_LIBS) -lXm -lXpm -lXext -lXmu -lXt -lX11 -lSM -lICE

FPIC		= -fPIC
PICDIR		= pic
SHARED_EXT	= so
SHARED_LIB	= $(patsubst %.a,%.$(SHARED_EXT),$(library))
SHFLAGS		= -Wl,-soname,$(basename $(shlib))
BUILD_LIBS	= $(library)

ifeq ($(ISSHARED),1)
BUILD_LIBS     += $(shlib)
LDSHARE		= -Xlinker --disable-new-dtags -Xlinker -rpath -Xlinker $(libdir)
LIB4VEC		= ../FourVector/lib4vec.$(SHARED_EXT)
LIBAQUA		= $(libdir)/libaqua.$(SHARED_EXT)
LIBCH		= ../Chamber/libch++.$(SHARED_EXT)
LIBCINDY	= ../Cindy/libcindy.$(SHARED_EXT)
LIBDMAIN	= ../dmaini/libdmain.$(SHARED_EXT)
LIBHBOOK	= ../HBook/libHBook.$(SHARED_EXT)
LIBHMBOOK	= ../HMBook/libhmbook.$(SHARED_EXT)
else
LIB4VEC		= ../FourVector/lib4vec.a
LIBAQUA		= $(libdir)/libaqua.a
LIBCH		= ../Chamber/libch++.a
LIBCINDY	= ../Cindy/libcindy.a
LIBDMAIN	= ../dmaini/libdmain.a
LIBHBOOK	= ../HBook/libHBook.a
LIBHMBOOK	= ../HMBook/libhmbook.a
endif
LIBHMVIEW	= ../HMBook/libhmview.a
LIBMODEL	= ../Model/libmodel.a
LIBSIMUL	= ../Simulation/libsimul.a
LIBSIMREADER    = ../Simulation/EventSim/libeventsimreader.a
LIBELASTICRADIATIVE= ../Simulation/ElasticRadiative/libElasticRadiative.a
LIBDETECT	= ../Detector/libdetector.a
LIBAQUA_SETUP	= $(libdir)/libaqua.$(SHARED_EXT) \
		  $(libdir)/libsetup.$(SHARED_EXT)
LIBAQUA_READER	= $(LIBAQUA_SETUP) $(libdir)/libreader.$(SHARED_EXT)
LDREADER	= -Xlinker --disable-new-dtags -Xlinker -rpath -Xlinker $(libdir) -ldl
LIBTMA		= ../TMA/libtma.a

define link_shlib
  $(CC) -shared $(SHFLAGS) -o $(shlib) $^
  -rm -f $(SHARED_LIB)
  -ln -s $(shlib) $(SHARED_LIB)
endef

define install_shlib
  if test -f $(shlib);then \
    $(INSTALL_PROGRAM) $(shlib) $(libdir)/$(shlib); \
    rm -f $(libdir)/$(SHARED_LIB) $(basename $(libdir)/$(shlib)); \
    ln -s $(shlib) $(libdir)/$(SHARED_LIB); \
    ln -s $(shlib) $(basename $(libdir)/$(shlib)); \
  fi
endef

LIBGCC	       := $(shell $(CC) --print-libgcc-file-name)

WX_CFLAGS	= -I/usr/local/wxwin/include/base -I/usr/local/wxwin/include/x $(X_CFLAGS)
ifeq ($(ISSHARED),1)
WX_LIBS		= -L/usr/local/wxwin/lib -lwx_motif
else
WX_LIBS		= /usr/local/wxwin/lib/libwx_motif.a
endif

DOCXX		= /usr/local/src/Cola/Doc/doc++
DOCXXSTY	= docxx.sty
DVIPS		= /usr/bin/dvips
LATEX		= /usr/bin/latex -interaction batchmode
htmldir		= html
texdir		= tex

.PHONY : all doc lib install uninstall clean veryclean distclean

define reportsuccess
  -@ tput smacs;echo "	lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqk"
  -@ tput smacs;echo -n "	x"
  -@ tput rmacs;printf "%16s done          " $@
  -@ tput smacs;echo "x"
  -@ tput smacs;echo "	mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj"; tput rmacs
endef

define reportaction
  -@ printf "%-26s -> %s\n"  $(<F) $(@F)
endef

$(PICDIR)/%.o	: %.c
	-@ printf "%-26s -> %s\n"  $(<F) $@
	@test -d $(PICDIR) || mkdir $(PICDIR)
	@$(CC) $(CPPFLAGS) $(CXXFLAGS) $(FPIC) -c $< -o $@

$(PICDIR)/%.o	: %.cc
	-@ printf "%-26s -> %s\n"  $(<F) $@
	@test -d $(PICDIR) || mkdir $(PICDIR)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(FPIC) -c $< -o $@

$(PICDIR)/%.o	: %.f
	-@ printf "%-26s -> %s\n"  $(<F) $@
	@test -d $(PICDIR) || mkdir $(PICDIR)
	@$(F77) $(FFLAGS) $(TARGET_ARCH) $(FPIC) -c $< $(OUTPUT_OPTION)

#
# The following lines suppress the lengthy command line echoing.
# If you want to see the command line for debuging, use  "make -n"
#
%.o : %.c
	$(reportaction)
	@ $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

%.o : %.cc
	$(reportaction)
	@ $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

%.o: %.f
	$(reportaction)
	@ $(F77) $(FFLAGS) $(TARGET_ARCH) -c $< $(OUTPUT_OPTION)

%.YACC.c %.YACC.h : %.y
	$(reportaction)
	@ $(BISONXX) -dvy $(BISONFLAGS) $^ -o $*.YACC.c -h $*.YACC.h

%.parse.cc %.parse.h : %.y
	$(reportaction)
	@ $(BISONXX) -dvy $(BISONFLAGS) $^ -o $*.parse.cc -h $*.parse.h

%.lex.cc %.lex.h : %.l
	$(reportaction)
	@ $(FLEXXX) -8 $(FLEXFLAGS) -o$*.lex.cc -h$*.lex.h $<

.%.depend : %.cc
	$(reportaction)
	@ $(CXX) -MM $(CPPFLAGS) $^ > $@

.%.depend : %.c
	$(reportaction)
	@ $(CC) -MM $(CPPFLAGS) $^ > $@

man1/%.1 : 	doc/%.1.in
	cp $< $@
#	nroff -man $< > $@

%.ps :	%.sgml
	@sgml2latex -a -p $<

%.html:	%.sgml
	@sgml2html $<

%.info:	%.sgml
	@sgml2info $<

%.rtf:	%.sgml
	-@sgml2rtf  $<

%.txt :	%.sgml
	@sgml2txt  $<





