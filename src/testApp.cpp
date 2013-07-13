#include "testApp.h"

void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(25);
    camWidth=1920;
    camHeight=1080;
    videoDevices = myGrabber.listVideoDevices();
	myGrabber.initGrabber(camWidth,camHeight);
    myGrabber.setVideoCodec("QTCompressionOptionsLosslessAnimationVideo");
    imageCounter=0;
    xPos=0;
    yPos=0;
    objectWidth=myGrabber.getWidth()/9;
    objectHeight=myGrabber.getHeight()/9;
    spacing=50;
    videoCumul 	= new unsigned char[int(myGrabber.getWidth()*myGrabber.getHeight()*4)];
	videoInverted 	= new unsigned char[int(myGrabber.getWidth()*myGrabber.getHeight()*4)];
	videoTexture.allocate(myGrabber.getWidth(),myGrabber.getHeight(), GL_RGBA);
    ofEnableAlphaBlending();
	ofBackground(0,0,0);
	gFading = false;
	gFadeCounter= 0;
	gCulumate= true;
	picCounter=0;
    loadingImages=false;
    showHelp=false;
    playMode=false;
    drawRatio = float(ofGetScreenHeight())/camHeight;
    tempPixels.allocate(camWidth, camHeight, 4);
    for (int i=0; i<8; i++) {
        tempImage[i].allocate(camWidth, camHeight, OF_IMAGE_COLOR_ALPHA);
    }
    
    pageNumber=0;
    previewHeight=camHeight/4;
    previewWidth=camWidth/4;
    previewScreenWidth=1920;
    previewScreenHeight=1080;
    dir.open("");
    dir.allowExt("png");
    dir.listDir();
    dir.sort();
    threadSaver=0;
    for(int i = 0; i < dir.numFiles(); i++){
        ofLogNotice(dir.getPath(i));
    }
}


void testApp::update() {
    myGrabber.update();
	float fadingAmount = 0.99;
	if (myGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = myGrabber.getPixels();
		gFadeCounter++;
		if (gFadeCounter== 1){
            gFadeCounter= 0;
		}
		for (int j = 0; j < camHeight; j++){
			for (int i = 0; i < camWidth; i++){
				if (gFading == true && gFadeCounter==0 ){
					videoInverted[j* camWidth * 4+ i*4] = char(videoInverted[j* camWidth * 4+ i*4] * fadingAmount);
					videoInverted[j* camWidth * 4+i*4+1] = char(videoInverted[j* camWidth * 4+i*4+1] * fadingAmount);
					videoInverted[j* camWidth * 4+i*4+2] = char(videoInverted[j* camWidth * 4+i*4+2] * fadingAmount);
				}
                
				if (gCulumate == true){
                    videoInverted[j* camWidth * 4+ i*4] =MAX(videoInverted[j* camWidth * 4+ i*4], pixels[(j)* camWidth * 3 +(camWidth-i)*3]);
                    videoInverted[j* camWidth * 4+i*4+1] =MAX(videoInverted[j* camWidth * 4+i*4+1],  pixels[(j)* camWidth * 3 +(camWidth-i)*3 +1]);
                    videoInverted[j* camWidth * 4+i*4+2] =MAX(videoInverted[j* camWidth * 4+i*4+2],  pixels[(j)* camWidth * 3 +(camWidth-i)*3 +2]);
                    videoInverted[j* camWidth * 4+i*4+3] = 255;// MAX(0,MIN(255,mouseX));// pixels[(yy+j)* imageWidth * 4 +(xx+i)*4 +3];
				}else {
					videoCumul[j* camWidth * 4+ i*4] =MAX(videoInverted[j* camWidth * 4+ i*4], pixels[(j)* camWidth * 3 +(camWidth-i)*3]);
					videoCumul[j* camWidth * 4+i*4+1] =MAX(videoInverted[j* camWidth * 4+i*4+1],  pixels[(j)* camWidth * 3 +(camWidth-i)*3 +1]);
					videoCumul[j* camWidth * 4+i*4+2] =MAX(videoInverted[j* camWidth * 4+i*4+2],  pixels[(j)* camWidth * 3 +(camWidth-i)*3 +2]);
					videoCumul[j* camWidth * 4+i*4+3] = 255;// MAX(0,MIN(255,mouseX));// pixels[(yy+j)* imageWidth * 4 +(xx+i)*4 +3];
				}
			}
		}
        
		if (gCulumate == true){
            videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGBA);
		}else {
			videoTexture.loadData(videoCumul, camWidth,camHeight, GL_RGBA);
		}
	}
    
    if (gFading) {
        fadeString="On";
    }
    else fadeString="Off";
    
}

void testApp::draw() {
    ofSetBackgroundColor(0, 0, 0);
    if (playMode) {
        for (int i =0; i<objects.size(); i++) {
            objects[i]->draw();
            if (objects[i]->isMouseOver()==true) {
                objects[i]->objectImage.draw(1920-previewWidth, 1080-previewHeight,previewWidth, previewHeight);
                if (objects[i]->isMousePressed()) {
                    objects[i]->enabled=false;
                    objects.erase(objects.begin()+i);
                    xPos=0;
                    yPos=0;
                    imageCounter=0;
                    for (int i=0; i<objects.size(); i++) {
                        xPos=imageCounter*objectWidth+(imageCounter*spacing);
                        if (1920-xPos<objectWidth) {
                            if (1080-yPos<objectHeight) {
                                pageNumberDraw++;
                            }
                            xPos=0;
                            imageCounter=0;
                            yPos+=objectHeight+spacing;
                        }
                        objects[i]->set(xPos, yPos, objectWidth, objectHeight);
                        objects[i]->pageNumber=pageNumberDraw;
                        imageCounter++;
                    }
                }
            }
        }
    }
    
    if (!playMode) {
        videoTexture.draw((ofGetScreenWidth()-(float(camWidth)*drawRatio))/2,0,(float(camWidth)*drawRatio),ofGetScreenHeight());
		ofSetColor(255,255,255);
    }
    
    if (showHelp) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(picCounter), 10,10);
        int startY;
        startY=80;
        ofDrawBitmapString("Key Commands", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press space bar to save your image", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'c' to clear the screen", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'p' to toggle play record mode", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'q' for fullscreen mode", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'f' to toggle fade mode: Fading is "+fadeString, 20,startY);
        startY+=20;
        ofDrawBitmapString("In playback mode keys 1 - 0 show recorded images", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'h' to show and hide these instructions", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'v' to change video input device - this will only work when you can see this message", 20,startY);
        startY+=50;
        ofDrawBitmapString("VIDEO DEVICE", 20, startY);
        startY+=20;
        for(int i = 0; i < videoDevices.size(); i++){
            if(i == myGrabber.getVideoDeviceID()){
                ofSetColor(255, 100, 100);
            }
            else{
                ofSetColor(255);
            }
            ofDrawBitmapString(videoDevices[i], 20, startY+i*20);
        }
    }
}

void testApp::reDrawObjects(){
    xPos=0;
    yPos=0;
    imageCounter=0;
    for (int i=0; i<objects.size(); i++) {
        xPos=imageCounter*objectWidth+(imageCounter*spacing);
        if (previewScreenWidth-xPos<objectWidth) {
            if (previewScreenHeight-yPos<objectHeight) {
                pageNumberDraw++;
            }
            xPos=0;
            imageCounter=0;
            yPos+=objectHeight+spacing;
        }
        objects[i]->set(xPos, yPos, objectWidth, objectHeight);
        objects[i]->pageNumber=pageNumberDraw;
        imageCounter++;
    }
}
void testApp::keyPressed(int key){
    if (key == 'f' || key == 'F'){
		gFading=!gFading;
	}
	if (key == 'h' || key == 'H'){
		showHelp=!showHelp;
	}
    if (key == 'q'){
		ofToggleFullscreen();
    }
	if (key == 'p'){
		
        if (playMode) {
            for (int i=0; i<objects.size(); i++) {
                objects[i]->enabled=false;
            }
        }
        if (!playMode) {
            loadAllImages();
            for (int i=0; i<objects.size(); i++) {
                objects[i]->enabled=true;
            }
        }
        playMode = !playMode;
        
        
	}
    if (key==OF_KEY_RIGHT) {
        spacing+=1;
        reDrawObjects();
    }
    if (key==OF_KEY_LEFT) {
        spacing-=1;
        reDrawObjects();
    }
    if (key==OF_KEY_UP) {
        objectHeight+=1;
        objectWidth+=1;
        reDrawObjects();
    }
    if (key==OF_KEY_DOWN) {
        objectHeight-=1;
        objectWidth-=1;
        reDrawObjects();
    }
    if(key == 'v'){
        if (showHelp &&!playMode){
            myGrabber.setVideoDeviceID( (myGrabber.getVideoDeviceID()+1) % videoDevices.size() );
        }
    }
	if (key == ' '){
		saveLumaImage();
		for (int j = 0; j < camHeight; j++){
			for (int i = 0; i < camWidth; i++){
				videoInverted[j* camWidth * 4+ i*4] =0;
				videoInverted[j* camWidth * 4+i*4+1] =0;
				videoInverted[j* camWidth * 4+i*4+2] = 0;
				videoInverted[j* camWidth * 4+i*4+3] = 255;// MAX(0,MIN(255,mouseX));// pixels[(yy+j)* imageWidth * 4 +(xx+i)*4 +3];
			}
		}
	}
	if (key == 'c'){
		for (int j = 0; j < camHeight; j++){
			for (int i = 0; i < camWidth; i++){
				videoInverted[j* camWidth * 4+ i*4] =0;
				videoInverted[j* camWidth * 4+i*4+1] =0;
				videoInverted[j* camWidth * 4+i*4+2] = 0;
				videoInverted[j* camWidth * 4+i*4+3] = 255;// MAX(0,MIN(255,mouseX));// pixels[(yy+j)* imageWidth * 4 +(xx+i)*4 +3];
			}
		}
	}
    
    if (key=='l') {
        loadAllImages();
    }
}
void testApp::loadAllImages(){
    for (int i=0; i<objects.size(); i++) {
        loader.loadFromDisk(objects[i]->objectImage, objects[i]->path);
    }
    
}
void testApp::saveLumaImage(){
    if (threadSaver==8) {
        threadSaver=0;
    }
    
    interactiveImageObject *object=new interactiveImageObject;
    xPos=imageCounter*objectWidth+(imageCounter*spacing);
    if (1920-xPos<objectWidth) {
        if (1080-yPos<objectHeight) {
            pageNumber++;
        }
        xPos=0;
        imageCounter=0;
        yPos+=objectHeight+spacing;
    }
    object->setup();
    object->set(xPos, yPos, objectWidth, objectHeight);
    videoTexture.readToPixels(tempPixels);
    tempImage[threadSaver].setFromPixels(tempPixels);
    string tempName = ofToString(ofGetTimestampString())+".png";
    object->path=tempName;
    tempImage[threadSaver].saveThreaded(tempName);
    object->pageNumber=pageNumber;
    object->enabled=false;
    objects.push_back(object);
    imageCounter+=1;
    threadSaver+=1;
    
}
void testApp::getNumPicsPerPage(){
    int maxColumns,maxRows, shortRows, shortColumns;
    maxColumns=1920/(objectWidth+spacing);
    maxRows=1080-(previewHeight+spacing)/(objectHeight+spacing);
    shortRows=(previewHeight+spacing)/(objectHeight+spacing);
    shortColumns=1920-(previewWidth+spacing)/(objectHeight+spacing);
    picsPerPage=maxColumns*maxRows+shortColumns+shortRows;
    
}

void testApp::resetPicsPosition(){
    
}





