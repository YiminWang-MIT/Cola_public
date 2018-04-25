MAIN = tma.texinfo
SRC = $(MAIN) c.example f.example

%.ps : %.dvi
	dvips $<


all : tma.info tmanote.dvi tmanote.ps tma.ps

	
tma.dvi : $(SRC)
	texi2dvi  $(MAIN)

tma.info : $(SRC)
	makeinfo $(MAIN)

tmanote.dvi : tmanote.tex 
	latex tmanote

clean :
	rm *~


realclean : 
	-rm *~ *.info *.dvi *.aux *.fn *.fns *.ky *.vr *.ps *.tp *.toc *.log\
	   *.pg *.cp
