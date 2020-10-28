#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //set up display window
    ofSetWindowPosition(0, 0);
    //Draw Settiing
    ofDisableSmoothing() ;   //turn off anti aliasing
    ofSetBackgroundAuto(false);
    ofBackground(0,0,0);

    
    
    ofColor ScreenLeftDraw(255,255,255);
    ofColor ScreenRightDraw(255,0,0);

    //WEBCAM Set up
    camWidth = 320;  // try to grab at this size.
    camHeight = 240;
    camCroppedWidth = 10;
    camCroppedHeight = 30;
    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    vidGrabber.setDeviceID(1);  //0 for faceTime <-- select camera source here
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoThreshold.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    videoThresholdTexture.allocate(videoThreshold);
    ofSetVerticalSync(true);
    
    webcamThreshold = 60;
    /*--- Web cam setting END ---*/
    
    /*----LEFT SCREEN---*/
    
    //Timer
    startTime1 = ofGetElapsedTimeMillis();
    startTime2 = ofGetElapsedTimeMillis();
    startTime3 = ofGetElapsedTimeMillis();
    startTimeDrawLine  = ofGetElapsedTimeMillis();
    LeftScreenCellDrawingCounter = 0;
    RightScreenCellDrawingCounter = 0;
    leftScreenRunCount = 0;
    rightScreenRunCount = 0;
    middleScreenRunCount = 0;
    drawLineCounter = 0;
    //initialize Left & Right Screen Cell
    cellSetL.resize(panelPixelNumber);
    cellSetR.resize(panelPixelNumber);
    for(int i = 0; i < cellSetL.size(); i++){
        cellSetL[i] = i; // set value using index
        cout << "Cell L#" << i << "Value is" << cellSetL[i] << endl;
    }
    for(int i = 0; i < cellSetR.size(); i++){
        cellSetR[i] = i; // set value using index
        cout << "Cell R#" << i << "Value is" << cellSetR[i] << endl;
    }
   
    shuffleCellSetL();
    shuffleCellSetR();
    
    
    
    //GUI Set up
    Gui.setup();
    Gui.setPosition(400,500);
    Gui.add(testslider.setup("camCroppedWidthOffset", 101, 10, 300));
    Gui.add(testslider2.setup("camCroppedHeightOffset", 56, 10, 300));
    Gui.add(testslider3.setup("camCroppedWidth", 137, 10, 320));
    Gui.add(testslider4.setup("camCroppedHeight", 196, 10, 240));
    Gui.add(testslider5.setup("Y Position", 547, 10, 1024));
    Gui.add(testslider6.setup("Webcam Threshold", 60, 0, 255));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    /*----Webcam update ----*/
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        ofPixels & pixels = vidGrabber.getPixels();
        for(size_t i = 0; i < pixels.size(); i++){
            //invert the color of the pixel
            videoInverted[i] = 255 - pixels[i];
            //threshold callabration
            if (pixels[i] >testslider6) {                videoThreshold[i] = 255;
            } else {
                videoThreshold[i] = 0;
            }
          
            //webcamThreshold
        }
        //load the inverted pixels
        videoTexture.loadData(videoInverted);
        videoThresholdTexture.loadData(videoThreshold);
        //videoTexture.loadData(videoThreshold);
    }
    /* ---Webcam update END ----*/

}

//--------------------------------------------------------------
void ofApp::draw(){
    /*---Draw Left and Right Screen ----*/
    drawLeftScreen(cellSetLSorted,7031,96,0+testslider5,511);   //7031ms per step
    drawRightScreen(cellSetRSorted,1000,96,32+testslider5,300);  //1000ms =
    drawMiddleScreen(1200000,64,16+testslider5);  //1200000ms =20mins
    
    
    //display the webcam image (no process_
//    camCroppedHeight = testslider;
//    camCroppedWidth = testslider2;
//    ofSetColor (ofColor(255));
//    vidGrabber.getTexture().drawSubsection(64, 16, panelWidth*2,panelHeight, testslider,testslider2, testslider3,testslider4);
    
    
    //display the webcam 2nd version
    ofSetColor(ofColor(255));
    videoTexture.draw(20 + camWidth/2, 20, camWidth/2, camHeight/2);
    videoThresholdTexture.draw(20 + camWidth, 20, camWidth/2, camHeight/2);
    
    //GUI display
    camCroppedHeight = testslider;
    camCroppedWidth = testslider2;
    Gui.draw();
}
//--------------------------------------------------------------

void ofApp::shuffleCellSetL(){
    std::random_shuffle(cellSetL.begin(),cellSetL.end());  // pass in range to be shuffled
    ofRandomize(cellSetL); // pass in just the  vector (all will be shuffled)
    cellSetLSorted.clear();
    cellSetLSorted.resize(512);
    
    //print the sorted result
    for(int i = 0; i < cellSetL.size(); i++){
        
        cout << "shuffled Cell L#" << i << " 's Value is :" << cellSetL[i] << endl;  // print out the value of element at position 0
        cellSetLSorted[i] = cellSetL[i];
        
    }
    
    
}
//--------------------------------------------------------------
void ofApp::drawLeftScreen(vector<int> cellSet , int timeInterval, int drawPositionX, int drawPositionY, int resetStep) {
    timer1 = ofGetElapsedTimeMillis() - startTime1;
    if (timer1 >= timeInterval) {
        
        
        if (LeftScreenCellDrawingCounter < resetStep) {
            //set points position
            int printXPosition = cellSet[LeftScreenCellDrawingCounter]/16;
            int printYPosition = cellSet[LeftScreenCellDrawingCounter]%16;
            cout << "CellSet # " << LeftScreenCellDrawingCounter << " | cell value = " << cellSet[LeftScreenCellDrawingCounter] <<  endl;

            //display the dot
            ofSetColor(255,255,255);  //white
            ofDrawRectangle(drawPositionX+printXPosition, drawPositionY+printYPosition, 1, 1);
            
            //increment
            LeftScreenCellDrawingCounter++;
            
        } else {
            //when reaching the reset steps
            shuffleCellSetL();
            LeftScreenCellDrawingCounter = 0;
            ofFill();
            ofSetColor(ofColor(0,0,0));  //Reset Black Background COLOR
            ofDrawRectangle(drawPositionX, drawPositionY, panelWidth, panelHeight);
            //ofNoFill();
        
            
        }
       
        startTime1 = ofGetElapsedTimeMillis();
    }
    
}

//--------------------------------------------------------------

void ofApp::shuffleCellSetR(){
    std::random_shuffle(cellSetR.begin(),cellSetR.end());  // pass in range to be shuffled
    ofRandomize(cellSetR); // pass in just the  vector (all will be shuffled)
    cellSetRSorted.clear();
    cellSetRSorted.resize(512);
    
    //print the sorted result
    for(int i = 0; i < cellSetR.size(); i++){
        
        cout << "shuffled Cell #" << i << " 's Value is :" << cellSetR[i] << endl;  // print out the value of element at position 0
        cellSetRSorted[i] = cellSetR[i];
        
    }
    
    
}
//--------------------------------------------------------------
void ofApp::drawRightScreen(vector<int> cellSet , int timeInterval, int drawPositionX, int drawPositionY, int resetStep) {
    timer2 = ofGetElapsedTimeMillis() - startTime2;
    if (timer2 >= timeInterval) {
        
        
       
        
        if (RightScreenCellDrawingCounter < resetStep) {
            //set points position
            int printXPosition = cellSet[RightScreenCellDrawingCounter]/16;
            int printYPosition = cellSet[RightScreenCellDrawingCounter]%16;
            cout << "CellSet # " << RightScreenCellDrawingCounter << " | cell value = " << cellSet[RightScreenCellDrawingCounter] <<  endl;
            
            //display the dot
            //ofSetColor(ofColor(255,0,0));
            ofSetColor(ScreenRightDraw);
            ofDrawRectangle(drawPositionX+printXPosition, drawPositionY+printYPosition, 1, 1);
            
            //increment
            RightScreenCellDrawingCounter++;
            
            
        } else {
            //when reaching the reset steps
            shuffleCellSetL();
            RightScreenCellDrawingCounter = 0;
            rightScreenRunCount++;
            
            //change color when resseting the screen
            //Mode 1:  Black Background, white dots
            if (rightScreenRunCount %3 == 0) {
                ofFill();
                ofSetColor(ofColor(0,0,0));
                ;//Reset Black Background COLOR
                ofDrawRectangle(drawPositionX, drawPositionY, panelWidth, panelHeight);
                //ofNoFill();
                ScreenRightDraw.r = 255;
                ScreenRightDraw.g = 255;
                ScreenRightDraw.b = 255;
                
            }
            //Mode 2:  WHITE Background, red dots
            if (rightScreenRunCount %3 == 1) {
                ofFill();
                ofSetColor(ofColor(255,255,255));
                ;//Reset Black Background COLOR
                ofDrawRectangle(drawPositionX, drawPositionY, panelWidth, panelHeight);
                //ofNoFill();
                ScreenRightDraw.r = 255;
                ScreenRightDraw.g = 0;
                ScreenRightDraw.b = 0;
                
            }
            //Mode 3:  red Background, black dots
            if (rightScreenRunCount %3 == 2) {
                ofFill();
                ofSetColor(ofColor(255,0,0));
                ;//Reset Black Background COLOR
                ofDrawRectangle(drawPositionX, drawPositionY, panelWidth, panelHeight);
                //ofNoFill();
                ScreenRightDraw.r = 0;
                ScreenRightDraw.g = 0;
                ScreenRightDraw.b = 0;
                
            }
 
        }
        
        startTime2 = ofGetElapsedTimeMillis();
    }
    
}


void ofApp::drawMiddleScreen(int timeInterval, int drawPositionX, int drawPositionY) {
    
    timer3 = ofGetElapsedTimeMillis() - startTime3;
    if (timer3 >= timeInterval) {
        middleScreenRunCount++ ; //increment flag
        cout << "middle Screen Count = " << middleScreenRunCount <<  endl;
        ofSetColor(ofColor(0));
        ofDrawRectangle(drawPositionX, drawPositionY, panelWidth*2, panelHeight);
        startTime3 = ofGetElapsedTimeMillis();
    }
    
        if (middleScreenRunCount %3 ==0) {
            

            ofSetColor (ofColor(255));
            vidGrabber.getTexture().drawSubsection(64, 16+testslider5, panelWidth*2,panelHeight, testslider,testslider2, testslider3,testslider4);
            
        }
        if (middleScreenRunCount %3 ==1) {

            middleScreenDrawLines(64,16+testslider5);
            
        }
    
        if (middleScreenRunCount %3 ==2) {
            // threshold mode
            ofSetColor(255,180,105);
            videoThresholdTexture.drawSubsection(64, 16+testslider5, panelWidth*2,panelHeight, testslider,testslider2, testslider3,testslider4);
        }
        

    }
    
    

//--------------------------------------------------------------

void ofApp::middleScreenDrawLines(int screenPosX, int screenPosY) {
    timerDrawLine = ofGetElapsedTimeMillis() - startTimeDrawLine;
    if (timerDrawLine >= 5000) {
        cout << "Drawline" << endl;
    
        int randomPoint1x = (int)ofRandom(64);
        int randomPoint2x = (int)ofRandom(64);
        int randomPoint1y = (int)ofRandom(16);
        int randomPoint2y = (int)ofRandom(16);
        ofSetColor(ofColor(255));
 ofDrawLine(randomPoint1x+screenPosX,randomPoint1y+screenPosY,randomPoint2x+screenPosX,randomPoint2y+screenPosY);
        drawLineCounter++;
        if (drawLineCounter >= 20) {
            ofSetColor(ofColor(0));
            ofDrawRectangle(screenPosX, screenPosY, panelWidth*2, panelHeight);
            drawLineCounter=0;
        }
            //startTime3 = ofGetElapsedTimeMillis();
        
        startTimeDrawLine = ofGetElapsedTimeMillis();

    }
       }

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
