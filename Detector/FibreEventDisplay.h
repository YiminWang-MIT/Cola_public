#ifndef FIBREEVENTDISPLAY_H
#define FIBREEVENTDISPLAY_H
#include <iostream>

#define FIBRE_X_MAX_CHANNEL 2304
#define FIBRE_X_MIN_TIME -500
#define FIBRE_X_MAX_TIME 600

#define FIBRE_THETA_MAX_CHANNEL 2304
#define FIBRE_THETA_MIN_TIME -500
#define FIBRE_THETA_MAX_TIME 600

// #define FED_MAXPAGENUM 200

class FibreEventDisplay
{
  public:
    FibreEventDisplay(int pages, double minTime, double maxTime, bool autoWriteHeader);
    ~FibreEventDisplay();

    void writeHeader();
    void drawPage();
    void closeFile();
    
    void addHit(int channelNumber, double channelTime, int plane, int type);
    void addTrack(int channelX, double timeX, int channelTh, double timeTh, double quality);
    void addCluster(int plane, int minChannel, int maxChannel, double minTime, double maxTime);
    void clusterInfoX(int multi, int leadingChannel, int minChannel, int maxChannel, double minTime, double maxTime);
    void clusterInfoTh(int multi, int leadingChannel, int minChannel, int maxChannel, double minTime, double maxTime);
    
  private:
    FILE *outputFile;
    bool fileOpen;
    bool pageUsed;
    int pageNumber;
    int pages;
    
    int typeNumberX[4], typeNumberTh[4];
} ;

#endif

