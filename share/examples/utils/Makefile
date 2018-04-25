BINARIES= VDC_TDC_off VDC_DRIFT data2ps

all:	$(BINARIES)

VDC_TDC_off:	VDC_TDC_off.o
	g++ -o $@ $^ /usr/local/a1/lib/libhmbook.a -lm

VDC_DRIFT:	VDC_DRIFT.o
	g++ -o $@ $^ /usr/local/a1/lib/libhmbook.a -lm

data2ps:	data2ps.o psclopt.o psplot.o
	$(CXX) -o $@ $^ -lg++

clean:
	rm -f *~ *.o $(BINARIES)
