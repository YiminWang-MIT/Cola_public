#include <ctype.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <getopt.h>
#include "HMBook/hmbook.h"
#include "Cola/include/abcn/detector.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// Type definitions
struct offset_t {
  short *x1;
  short *s1;
  short *x2;
  short *s2;
};
typedef struct offset_t offset_t;

// Global variables
static bool bUseA = FALSE;
static bool bUseB = FALSE;
static bool bUseC = FALSE;
static int  nTdcOffset = 0;

// Generate Cola definitions for a single layer
void GenerateColLayer(FILE *file, char spec, char *layer, short size) {
  int i;
  int range = 1024;
  if (tolower(spec) == 'c')
    range = 4096;

  for (i = 0; i < size; i++) {
    fprintf(file, "Scalar %c_%s_%d(\"TDC\", \"Channel\") "
	    "= @.%c.det.vdc.%s[%d]\n", 
	    spec, layer, i, spec, layer, i);
  }
  for (i = 0; i < size; i++) {
    fprintf(file, "Histogram \"Spec. %c/%s-layer/TDC/wire_%d!\" "
	    "%c_%s_%d(-0.5, %.1f, %d)\n", 
	    toupper(spec), layer, i, spec, layer, i, range-0.5, range);
  }
}

// Generate a Cola file with definitions for all specified spectrometers
void GenerateColFile(char *colfile) {
  // Write file
  FILE *file;
  if ((file = fopen(colfile, "w"))) {
    // Spectrometer A
    if (bUseA) {
      GenerateColLayer(file, 'a', "x1", AX1SIZE);
      GenerateColLayer(file, 'a', "s1", AS1SIZE);
      GenerateColLayer(file, 'a', "x2", AX2SIZE);
      GenerateColLayer(file, 'a', "s2", AS2SIZE);
    }
    // Spectrometer B
    if (bUseB) {
      GenerateColLayer(file, 'b', "x1", BX1SIZE);
      GenerateColLayer(file, 'b', "s1", BS1SIZE);
      GenerateColLayer(file, 'b', "x2", BX2SIZE);
      GenerateColLayer(file, 'b', "s2", BS2SIZE);
    }
    // Spectrometer C
    if (bUseC) {
      GenerateColLayer(file, 'c', "x1", CX1SIZE);
      GenerateColLayer(file, 'c', "s1", CS1SIZE);
      GenerateColLayer(file, 'c', "x2", CX2SIZE);
      GenerateColLayer(file, 'c', "s2", CS2SIZE);
    }
    fclose(file);
  }
}

// Determine offset for a single wire
void ScanSingleWireOffset(HIST *hist, short &offset) {
  if (!hist)
    return;

  int found = 0, test = 0;
  double max = 0;
  offset = 0;

  // Get maximum
  for (int k = 0; k < hist->nx; k++)
    if (hist->data[k] > max)
      max = hist->data[k];

  // Get offset
  if (hist->entries > 8000)
    for (int j = hist->nx - 1; j >= 0; j--) {
      if ((j > 1016) && (hist->data[j] > max * 0.4))
	continue;
      if ((hist->data[j] > max * 0.1) && test == 0)
	test = j;
      if ((hist->data[j] > max * 0.7) && 
	  j < test && abs(test - j) < 30) {
	offset = (int)((j*hist->data[test]-test*hist->data[j])/
		       (hist->data[test]-hist->data[j])) + nTdcOffset;
	found++;
	return;
      }
    }
}

void ScanHistFile(char *infile, 
		  offset_t *speca, offset_t *specb, offset_t *specc) {
  FILE *fp = HMOpenHisFile(infile);
  if (!fp)
    return;
  HIST *hist; 

  do {
    hist = HMReadIsolated(fp);
    if (!hist) break;
    char specname, layername[255];
    int wire;
    if (sscanf(hist->title, 
	       "Spec. %c/%[12xs]-layer/TDC/wire_%d!", 
	       &specname, layername, &wire) != 3) {
      HMDelete(hist);
      continue;
    }
    std::cout << "Scanning histogram " << hist->title << "  \r" << std::flush;
    
    offset_t *spec = NULL;
    switch(specname) {
    case 'A': spec = speca; break;
    case 'B': spec = specb; break;
    case 'C': spec = specc; break; 
    }
    if (!spec) {
      HMDelete(hist);
      continue;
    }
    short *layer = NULL;
    if (!strcmp(layername, "x1"))
      layer = spec->x1;
    else if (!strcmp(layername, "s1"))
      layer = spec->s1;
    else if (!strcmp(layername, "x2"))
      layer = spec->x2;
    else if (!strcmp(layername, "s2"))
      layer = spec->s2;

    // Scan single wire
    if (layer)
      ScanSingleWireOffset(hist, layer[wire]);

    HMDelete(hist);
  } while (-1);
  std::cout << std::endl;
  pclose(fp);
}

// Find corrupt wires
void PostProcessOffset(char *layername, short *layer, int size, int range) {
  std::cout << layername << ": ";
  int sum = 0, used = 0;
  for (int i = 0; i < size; i++) {
    int tdcoffset = layer[i];
    if (tdcoffset > 0) {
      // Corrupt wires
      if (tdcoffset <= 100 || tdcoffset >= range - 24) {
	std::cout << "*" << i << " ";
      } else {
	sum += tdcoffset;
	used++;
      }
    }
  }
  // Find strange wires
  int avg = sum / (used > 0 ? used : 1);
  for (int j = 0; j < size; j++) {
    int tdcoffset = layer[j];
    if (tdcoffset > 100 && tdcoffset < range - 24) {
      if (abs(tdcoffset - avg) > 25)
	std::cout << j << " ";
    }    
  }
  std::cout << std::endl;
}

void WriteTDCOffsets(FILE *file, char spec, char *layer, 
		     short size, short *tdc_offsets) {
  for (int i = 0; i < size; i++)
    fprintf(file, "%c.VDC.TDC.%s[%d] = %d\n", 
	    toupper(spec), layer, i, tdc_offsets[i]);
}

int main(int argc, char *argv[]) {
  char opt;
  char *infile = NULL;
  char *outfile = NULL;
  char *colfile = NULL;

  // Check arguments
  while ((opt = getopt(argc, argv, "t:c:o:f:ABC")) != EOF) {
    switch (opt) {
    case 'f': infile  = optarg;                       break;
    case 'o': outfile = optarg;                       break;
    case 'A': bUseA = TRUE;                           break;
    case 'B': bUseB = TRUE;                           break;
    case 'C': bUseC = TRUE;                           break;
    case 'c': colfile = optarg;                       break;
    case 't': if (optarg) nTdcOffset = atoi(optarg);  break;
    case '?':
    case ':':
      std::cout << "\nUSAGE:   " << argv[0] <<" [options]\n\n"
	"Options:\n" 
	"-f <file>          Input histogram file name.\n"
	"-o <file>          Output file name.\n"
	"-c <file>          Generate col file.\n"
	"-t <offset>        Additional tdc offset.\n"
	"-{A|B|C}           Spectrometer A/B/C\n" << std::endl;
      exit(0);
      break;
      default : std::cerr << "Invalid options." << std::endl; 
    }
  }

  // Generate col file
  if (colfile)
    GenerateColFile(colfile);

  // If no input file is specified, we quit
  if (!infile)
    return 0;

  // Allocate memory for offsets
  offset_t speca, specb, specc;
  speca.x1 = new short[AX1SIZE]; speca.s1 = new short[AS1SIZE];
  speca.x2 = new short[AX2SIZE]; speca.s2 = new short[AS2SIZE];
  specb.x1 = new short[BX1SIZE]; specb.s1 = new short[BS1SIZE];
  specb.x2 = new short[BX2SIZE]; specb.s2 = new short[BS2SIZE];
  specc.x1 = new short[CX1SIZE]; specc.s1 = new short[CS1SIZE];
  specc.x2 = new short[CX2SIZE]; specc.s2 = new short[CS2SIZE];

  // Scan histogram file
  ScanHistFile(infile, &speca, &specb, &specc);

  // Write offsets to a file
  if (outfile) {
    FILE *file = fopen(outfile, "w");
    if (file) {
      // Spectrometer A
      if (bUseA) {
	PostProcessOffset("a.x1", speca.x1, AX1SIZE, 1024);
	PostProcessOffset("a.s1", speca.s1, AS1SIZE, 1024);
	PostProcessOffset("a.x2", speca.x2, AX2SIZE, 1024);
	PostProcessOffset("a.s2", speca.s2, AS2SIZE, 1024);
	WriteTDCOffsets(file, 'a', "x1", AX1SIZE, speca.x1);
	WriteTDCOffsets(file, 'a', "s1", AS1SIZE, speca.s1);
	WriteTDCOffsets(file, 'a', "x2", AX2SIZE, speca.x2);
	WriteTDCOffsets(file, 'a', "s2", AS2SIZE, speca.s2);
      }
      // Spectrometer B
      if (bUseB) {
	PostProcessOffset("b.x1", specb.x1, BX1SIZE, 1024);
	PostProcessOffset("b.s1", specb.s1, BS1SIZE, 1024);
	PostProcessOffset("b.x2", specb.x2, BX2SIZE, 1024);
	PostProcessOffset("b.s2", specb.s2, BS2SIZE, 1024);
	WriteTDCOffsets(file, 'b', "x1", BX1SIZE, specb.x1);
	WriteTDCOffsets(file, 'b', "s1", BS1SIZE, specb.s1);
	WriteTDCOffsets(file, 'b', "x2", BX2SIZE, specb.x2);
	WriteTDCOffsets(file, 'b', "s2", BS2SIZE, specb.s2);
      }
      // Spectrometer C
      if (bUseC) {
	PostProcessOffset("c.x1", specc.x1, CX1SIZE, 4096);
	PostProcessOffset("c.s1", specc.s1, CS1SIZE, 4096);
	PostProcessOffset("c.x2", specc.x2, CX2SIZE, 4096);
	PostProcessOffset("c.s2", specc.s2, CS2SIZE, 4096);
	WriteTDCOffsets(file, 'c', "x1", CX1SIZE, specc.x1);
	WriteTDCOffsets(file, 'c', "s1", CS1SIZE, specc.s1);
	WriteTDCOffsets(file, 'c', "x2", CX2SIZE, specc.x2);
	WriteTDCOffsets(file, 'c', "s2", CS2SIZE, specc.s2);
      }
      fclose(file);
    }
  }

  // Free memory
  delete speca.x1; delete speca.s1; delete speca.x2; delete speca.s2;
  delete specb.x1; delete specb.s1; delete specb.x2; delete specb.s2;
  delete specc.x1; delete specc.s1; delete specc.x2; delete specc.s2;

  return 0;
}



