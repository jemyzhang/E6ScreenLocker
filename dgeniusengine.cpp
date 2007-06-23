
#include <ZMessageBox.h>

#include "dgeniusengine.h"
#include "dapplication.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>

extern "C" int  UTIL_GetNotificationStatus();
extern "C" int  UTIL_GetBtStatus();

void DGeniusEngine :: run( )
{
    static bool fnotification = false;
    static bool btstatus = false;

    canvas->update( );
    if(false == backlightstatus()) backlightctrl(true);
    while(1)
    {
        sleep(1);
        bool bt_ = UTIL_GetBtStatus();
        if(btstatus != bt_){
            btstatus = bt_;
            if (btstatus) {
                canvas->showSpriteBT();
            }else
            {
                canvas->hideSpriteBT ();
            }
        }
        bool fnotifi = UTIL_GetNotificationStatus();
        if(fnotification != fnotifi)
        {
            printf("Notification status: %d\n",fnotifi);
            fnotification = fnotifi;
            if(fnotification)
            {
                canvas->showSpritesms();
                canvas->showSpriteCall();
            }else
            {
                canvas->hideSpritesms();
                canvas->hideSpriteCall();
            }
            canvas->update();
        }

        struct tm *tm_ptr;
        time_t now;
        time(&now); 
        tm_ptr = localtime(&now);

        if (!ishide && !(view->isActiveWindow()) ) {    //check if screensaver is forced background
            showScreenSaver();
            printf("force active:%d\n",view->isActiveWindow());
        }else if( ishide && view->isActiveWindow() )
        {
            printf("force inactive\n");
            hideScreenSaver();
        }

        if (ishide) {
//            timeout ++;   //disable auto screen lock, can not receive mouse event while minimized
            timeout = 0;    //for backlight timeout
            //check timeout
            if (timeout > 5 ) {
                printf("timeout,auto lock\n");
                timeout = 0;
                canvas->updateScreenSprite( );
                canvas->update();
                showScreenSaver();
                }
        }else
        {
            //timeout = 0;
            timeout ++;
            if(timeout > 3){
                timeout = 0;
                if(true == backlightstatus()) backlightctrl(false);
            }
        }

        if( tm_ptr->tm_sec == 0 && false == ishide)
        {
           canvas->updateScreenSprite( );
           canvas->update();
        }
        if( true == keypressed)
        {
            timecnt ++;
            if ( (timecnt % 2 ) ==0 ){
                hidepressed = false;
                showpressed = false;
                keypressed = false;
                canvas->hideString();
                canvas->hideSpriteLock();
                canvas->update();
            }
        }
        
    }
}

void DGeniusEngine :: pointerPressed( int x, int y )
{
    std::cout << "Pointer pressed on " << x << "," << y << std::endl;
    
}

void DGeniusEngine :: pointerDragged( int x, int y )
{
    std::cout << "Pointer dragged on " << x << "," << y << std::endl;
}

void DGeniusEngine :: pointerReleased( int x, int y )
{
    std::cout << "Pointer released on " << x << "," << y << std::endl;
}

void DGeniusEngine :: keyPressed(int keycode)
{
    keypressed = true ;
    timeout = 0;
    if(false == backlightstatus()) backlightctrl(true);
    if (false == ishide) {
        if(4144 == keycode)
        {
            hidepressed = true ;
            canvas->showString("\n  Unlock: Now Press Center Key.");
            canvas->showSpriteLock();
            canvas->update();
        }else if( 4100 == keycode && true == hidepressed )
        {
            hidepressed = true ;
            canvas->hideString();
            canvas->hideSpriteLock();
           hideScreenSaver();
           ishide = true;
        }else
        {
            hidepressed = false;
            canvas->showString("\n  Unlock: First Press Green Key, \n   Then Press Center Key.");
            canvas->showSpriteLock();
            canvas->update();
        }
    }else
    {
        if (4171 == keycode) {
            showpressed = ~showpressed;
        }else if(4172 == keycode && true == showpressed)
        {
            showpressed = false;
            canvas->updateScreenSprite( );
            canvas->update();
            showScreenSaver();
        }else
        {
            showpressed = false;
        }
    }
}

void DGeniusEngine :: mousePressed( )
{
   // if (false == ishide && false == backlightstatus()) {
        //backlightctrl(false);
    //}
}

void DGeniusEngine :: QcopAutoLock( )
{
    if (ishide) {
        //showScreenSaver();
    }
}

void DGeniusEngine :: setview(QCanvasView *canvasview)
{
    view = canvasview;
}

void DGeniusEngine :: showScreenSaver()
{
    printf("canvas show\n");
    view->showFullScreen();
    view->show();
    ishide = false;
}

void DGeniusEngine :: hideScreenSaver()
{
    printf("canvas Minimized\n");
    view->showMinimized();
    ishide = true;
}

void DGeniusEngine :: backlightctrl(bool onoff)
{
    int fbd = open("/dev/fb0", O_RDWR);
    if(fbd<0){
    printf("can not open fb0\n");
    return;
    }
 
    ioctl(fbd,FBIOBLANK,0);
    ioctl(fbd,FBIOSETBKLIGHT,onoff);

    close(fbd);
    printf("Set backlight %s\n",onoff == true ?  "ON" : "OFF");
}

int DGeniusEngine :: backlightstatus( )
{

    int fbd = open("/dev/fb0", O_RDWR);
    int status = 0;
    if(fbd<0){
    printf("can not open fb0\n");
    return false ;
    }
 
    ioctl(fbd,FBIOBLANK,0);
    if(ioctl(fbd,FBIOGETBKLIGHT,&status) < 0)
    {
        printf("backlight status : get status error\n");
    }

    close(fbd);
    printf("Backlight status %d\n",status);
    return status;
}
