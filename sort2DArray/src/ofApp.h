#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include "ofxOpenCv.h"

#define panelWidth 32
#define panelHeight 16
#define panelPixelNumber 512


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    //Webcam Setting
    ofVideoGrabber vidGrabber;
    ofPixels videoInverted;
    ofPixels videoThreshold;
    ofTexture videoTexture;
    ofTexture videoThresholdTexture;
    int camWidth;
    int camHeight;
    int camCroppedWidth;
    int camCroppedHeight;
    
    
    //Left Screen parameter
        vector<int> cellSetL ;
        vector<int> cellSetLSorted;
        vector<int> cellSetR ;
        vector<int> cellSetRSorted;
    //Left Screen Timer
        int timer1;
        int startTime1;
        bool timer1End;
    int leftScreenRunCount;
    //Right Screen Timer
        int timer2;
        int startTime2;
        bool timer2End;
    int rightScreenRunCount;
        ofColor ScreenLeftDraw;
        ofColor ScreenRightDraw;
        int LeftScreenCellDrawingCounter;
        int RightScreenCellDrawingCounter;
    
    
        void drawLeftScreen(vector<int> cellSet , int timeInterval, int drawPositionX, int drawPositionY, int resetStep);
        void drawRightScreen(vector<int> cellSet , int timeInterval, int drawPositionX, int drawPositionY, int resetStep);
    
        int ScreenLeftPosX = panelWidth * 3;
        int ScreenLeftPosY = panelHeight* 0;
    
    void shuffleCellSetL();
    void shuffleCellSetR();
    
    //middle Screen mode
    int middleScreenRunCount;
    void drawMiddleScreen(int timeInterval, int drawPositionX, int drawPositionY);
    void middleScreenDrawLines(int screenPosX, int screenPosY);
    int timer3;
    int startTime3;
    bool timer3End;
    
    //drawline mode timer
    int timerDrawLine;
    int startTimeDrawLine;
    int drawLineCounter;
    
    int webcamThreshold;
    
    //GUI
    ofxIntSlider testslider ;
    ofxIntSlider testslider2;
    ofxIntSlider testslider3 ;
    ofxIntSlider testslider4;
    ofxIntSlider testslider5;
    ofxIntSlider testslider6;

    ofxPanel Gui;
};
