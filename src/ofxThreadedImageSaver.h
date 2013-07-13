#pragma once

#include "ofMain.h"
#include "ofThread.h"

class ofxThreadedImageSaver : public ofThread, public ofImage {
public:
    string fileName;
    
    void threadedFunction();
    void saveThreaded(string fileName);
    
};