#								 -*-makefile-*-
# Created 1996 by	Michael O. Distler, Harald Merkel,
#			Institut für Kernphysik,
#			Johannes Gutenberg-Universität Mainz
#
# $Header: /tmp/cvsroot/Cola/Makefile.inc.in,v 1.41 2008-02-27 10:46:15 patrick Exp $
#
# global makefile
#

topdir		= @srcdir@

SHELL		= /bin/sh
CC		= @CC@
CPP		= @CPP@
GNUCPP		= @GNUCPP@
CXX		= @CXX@
F77		= @F77@
FC		= @FC@
INSTALL		= @INSTALL@
INSTALL_PROGRAM	= @INSTALL_PROGRAM@
INSTALL_DATA	= @INSTALL_DATA@
RANLIB		= @RANLIB@

AQUACONFIG	= @AQUACONFIG@
BISONXX		= @BISONXX@
BISONFLAGS	= @BISONFLAGS@
CINDY		= ../Cindy/Cindy++
FLEXXX		= @FLEXXX@
FLEXFLAGS	= @FLEXFLAGS@
FIND		= @FIND@
GZIP		= @GZIP@
YACC		= @YACC@

DEFS		= @DEFS@
OPTS		= @OPTS@
LIBS		= @LIBS@
WARN		= @WARN@
ISSHARED	= @ISSHARED@
ISSTATIC	= @ISSTATIC@

prefix		= @prefix@
exec_prefix	= @exec_prefix@
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

PGLIBS		= @PG_LIBS@
PGINCL		= @PG_INCL@

GSLLIBS		= @GSL_LIBS@
GSLINCL		= @GSL_INCL@

ARCH		= @host@
CERNLIB         = @CERNLIB@
ROOTLIB		= @ROOTLIB@
ROOT_CFLAGS	= @ROOT_CFLAGS@
ROOT_LIBS	= @ROOT_LIBS@

FFLAGS		= @FFLAGS@
FLIBS		= @FLIBS@
FCFLAGS		= @FCFLAGS@

X_CFLAGS	= @X_CFLAGS@
X_LIBS		= @X_LIBS@
MOTIFLIBS	= $(X_LIBS) @LIBMOTIF@

FPIC		= -fPIC
PICDIR		= pic
SHARED_EXT	= @SHARED_EXT@
SHARED_LIB	= $(patsubst %.a,%.$(SHARED_EXT),$(library))
SHFLAGS		= @SHFLAGS@
BUILD_LIBS	= $(library)

ifeq ($(ISSHARED),1)
BUILD_LIBS     += $(shlib)
LDSHARE		= @LDSHARE@
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
LIBELASTICRADIATIVE = ../Simulation/ElasticRadiative/libElasticRadiative.a
LIBDETECT	= ../Detector/libdetector.a
LIBAQUA_SETUP	= $(libdir)/libaqua.$(SHARED_EXT) \
		  $(libdir)/libsetup.$(SHARED_EXT)
LIBAQUA_READER	= $(LIBAQUA_SETUP) $(libdir)/libreader.$(SHARED_EXT)
LDREADER	= @LDSHARE@ -ldl
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

WX_CFLAGS	= -I@WXINC@/base -I@WXINC@/x $(X_CFLAGS)
ifeq ($(ISSHARED),1)
WX_LIBS		= -L@WXLIB@ -lwx_motif
else
WX_LIBS		= @WXLIB@/libwx_motif.a
endif

DOCXX		= @DOCXX@
DOCXXSTY	= docxx.sty
DVIPS		= @DVIPS@
LATEX		= @LATEX@ -interaction batchmode
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





