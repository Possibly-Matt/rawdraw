
#include <stdio.h>
#include <stdlib.h>;
#include <math.h>
#include <string.h>
#include "../os_generic.h"

#define CNFG_IMPLEMENTATION
#include <time.h>
#include "../CNFG.h"

#define PI 3.14159265
// these are used for the starting window size
#define windowW 1920    // 1024
#define windowH 1008    //576

unsigned frames = 0;
unsigned long iframeno = 0;

int mouseX = 0; 
int mouseY = 0;
int mouseMask = 0;
int precision = 1; // i forgot what this is used for in handlebutton()


void HandleKey( int keycode, int bDown ){ // keyboard press
	if( keycode == 27 ) exit( 0 );
	printf( "Key: %d -> %d\n", keycode, bDown );
}
void HandleButton( int x, int y, int button, int bDown ){ // mouse buttons
    precision++;
	printf( "Button: %d,%d (%d) -> %d\n", x, y, button, bDown );
    
}
void HandleMotion( int x, int y, int mask ){ // mouse position
    mouseX = x;
    mouseY = y;
    mouseMask = mask;
}
void HandleDestroy(){ // call to end the program 

	printf( "Destroying\n" );
	exit(10);
}


// this could be optimised
// i mostly use this for circles but don't tell anyone that...  
void drawRegPoly( int x, int y, int sides, int radius){

    double seperation = (1/(double)sides)*(PI/0.5f);
    double currentAngle = 0.0;
    float currX;
    float currY;
    float prevX = 0.0f;
    float prevY = 1.0f*(float)radius;

    for(int i = 0; i <= sides; i++){
        currX = sin(currentAngle)*radius;
        currY = cos(currentAngle)*radius;;
        CNFGTackSegment(x+prevX, y+prevY, x+currX, y+currY);
        prevX = currX;
        prevY = currY;

        currentAngle += seperation;
    }
}



int main()
{
    
    int i;
    double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	double LastFrameTime = OGGetAbsoluteTime();
	double SecToWait;
    int LastFPS; // updates every second 
    int linesegs = 0;

    int paddingAmount = 10;

    short w, h; // window size

    // these are used to display fps and window width/height
    char str[12];
    char str2[100];

    // make random more random
    srand(time(NULL));

	CNFGSetup( "UiElements Rawdraw Demo", windowW, windowH );

    CNFGGetDimensions( &w, &h ); // updates the window size variables
    CNFGClearFrame();
	while(1)
	{
        iframeno++; // woo new frame! up the total frame number

		CNFGHandleInput();
        CNFGClearFrame(); // cleaning up last frame

		CNFGGetDimensions( &w, &h ); // updates the window size variables
		CNFGColor( 0xffffff ); // sets "pen" colour that CNFG functions use
		
        // outer border
        CNFGDrawBox(paddingAmount,paddingAmount, w - paddingAmount , h - paddingAmount );
        
        // grid
        CNFGColor( 0x333333 );
        int wLine = 0;
        int hLine = 0;
        int squareAmount = 100;
        do { // |||||||||||
            CNFGTackSegment(wLine, 0, wLine, h);
            wLine += squareAmount;
        } while(wLine <= w);

        do { // ========
            CNFGTackSegment(0, hLine, w, hLine);
            hLine += squareAmount;
        } while(hLine <= h);


        CNFGColor( 0xffffff ); // resets the colour back

        // all base stuff finished, drawing the rest of the ui elements

        // fps display
        sprintf(str, "%d", LastFPS);   //b g r
        if(LastFPS < 60) CNFGColor(0x0000ff); // if fps falls below 60

        CNFGPenX = 20.9; CNFGPenY = h -30;
        CNFGDrawText( str , 4 );
        CNFGColor(0xffffff);

        // window size display
        sprintf(str, "%d", w); // wxh
        strcpy(str2, str);
        strcat(str2, "x");
        sprintf(str, "%d", h);
        strcat(str2, str);
        CNFGPenX = 100; CNFGPenY = h -30;
        CNFGDrawText( str2 , 4 );


        // end of drawing 
        frames++;
		CNFGSwapBuffers();
        
        ThisTime = OGGetAbsoluteTime();
		if( ThisTime > LastFPSTime + 1 )
		{
			printf( "FPS: %d Mouse: %d,%d\n", frames, mouseX, mouseY);
            LastFPS = frames; // for display
			frames = 0;
			linesegs = 0;
			LastFPSTime+=1;
		}

        // FPS limiter
        SecToWait = .016 - ( ThisTime - LastFrameTime );
		LastFrameTime += .016;
		if( SecToWait > 0 )
			OGUSleep( (int)( SecToWait * 1000000 ) );
            
	}
}
