#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "interactiveImageObject.h"
#include "ofxQTKitVideoGrabber.h"
#include "ofxThreadedImageLoader.h"
#include "ofxThreadedImageSaver.h"

class testApp : public ofBaseApp {
	
public:
	

	void setup();
    void update();
    void draw();
     void keyPressed(int key);
    void    loadAllImages();
    vector<interactiveImageObject*> objects;
    ofxQTKitVideoGrabber myGrabber;
    int imageCounter, xPos, yPos,objectWidth, objectHeight, spacing;
    
    void saveLumaImage();
    void reDrawObjects();
    void getNumPicsPerPage();
    void resetPicsPosition();
    int picsPerPage;
    ofImage zaveImage;
    
    string fadeString;
    //ofxQTKitVideoGrabber 		myGrabber;
    unsigned char * 	videoInverted;
    unsigned char * 	videoCumul;
	bool gCulumate;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    float					drawRatio;
	int gFadeCounter;
	bool gFading;
	bool gRedCap, gGreenCap, gBlueCap;
    int savedNum;
	int picCounter;
	bool playMode;
    bool loadingImages;
	string picName;
	vector<string> videoDevices;
    vector<string*> fileNames;
    bool showHelp;
    
    ofPixels tempPixels;
    ofxThreadedImageSaver tempImage[8];
    
    int pageNumber, pageNumberDraw, pageInView, previewWidth, previewHeight, previewScreenWidth, previewScreenHeight, threadSaver;
    ofxThreadedImageLoader loader;
   
    ofDirectory dir;


};

#endif

