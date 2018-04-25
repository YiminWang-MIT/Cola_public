#! /bin/sh
#
# the guy to blame:
#
# Michael O. Distler                        mailto:distler@mit.edu
# MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
# Cambridge, MA 02139, USA                  fax    (617) 258-5440
#
# $Header: /tmp/cvsroot/Cola/Doc/commit.sh,v 1.2 1997-08-01 17:46:29 distler Exp $
#
# This script changes the original DOC++ 3.0 distribution
# and provides a GNU configure script + Makefile.in
#
# definitions
#
src=docxxsrc.tar.gz
doc=docxxdoc.tar.gz
mod=docxxmod.tar.gz
pat=docxxmod.patch.gz
#
# error and warning messages
#
errorNoTar()
{
    echo ERROR: Make sure the following files exist in your current directory:
    echo "      " $doc, $src and $mod
    exit 1
}
warningNoPatch()
{
    echo Warning: patch \'$pat\' not available
    echo "        " Do you want to continue "(y/N) ? "
    read yn
    test "x$yn" = "xy" || test "x$yn" = "xyes" || exit 2
}
#
# Test if the tar files exist
#
test -f $src || errorNoTar
test -f $doc || errorNoTar
test -f $mod || errorNoTar
test -f $pat || warningNoPatch
#
# unpack archives
#
gzip -cd $src | tar xf -
gzip -cd $doc | tar xf -
gzip -cd $mod | tar xf -
#
# move files
#
test -d classes || mkdir classes
test -d include || mkdir include
mv src/*.class classes
mv src/java.h include
#
# remove files and directories
#
rm -rf doc/CVS
rm -f doc/*.class doc/*.html doc/*.gif
rm -f doc/Makefile doc/doc.aux doc/doc.dvi doc/doc.log doc/doc.ps doc/doc.tex
rm -f doc/dxxgifs.tex doc/gifs.db src/Makefile src/cpp.cpp src/depend
rm -f src/doc.cpp src/doc2html.cpp src/doc2tex.cpp src/docify.cpp src/java.cpp
rm -f src/lex.yy.c src/parser.y src/promote.c src/promote.l src/readfiles.cpp
#
# patch files
#
sed -e 's-/sgisoft/local/bin/perl-@PERL@-' -e 's/.$//'<src/binToC.pl >binToC.in
rm -f src/binToC.pl
chmod +x binToC.in
sed -e 's/macors/macros/' > doc/tmp.dxx < doc/reference.dxx
mv doc/tmp.dxx doc/reference.dxx
sed -e 's/Z&oumlckler/Z\&ouml;ckler/' > src/tmp.cpp < src/html.cpp
mv src/tmp.cpp src/html.cpp
test -f $pat && (cd src;gzip -cd ../$pat|patch -p)
#
echo done.
