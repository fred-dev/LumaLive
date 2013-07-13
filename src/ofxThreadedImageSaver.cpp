
#include "ofxThreadedImageSaver.h"

void ofxThreadedImageSaver::threadedFunction() {
    if(lock()) {
        saveImage(fileName);
        unlock();
    } else {
        printf("ofxThreadedImageSaver - cannot save %s cos I'm locked", fileName.c_str());
    }
    stopThread();
}


void ofxThreadedImageSaver::saveThreaded(string fileName) {
    this->fileName = fileName;
    startThread(false, false);   // blocking, verbose
}