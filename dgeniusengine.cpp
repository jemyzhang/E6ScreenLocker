
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
extern "C" int  UTIL_GetIncomingCallStatus();
extern "C" int  UTIL_GetCallConnectedStatus(void);  
extern "C" int  UTIL_GetTimingPhoneLock();
extern "C" int PM_setupLcdSleepTime(int sleepseconds);
extern "C" int PM_setupKeylightSleepTime(int sleepseconds);
extern "C" int PM_setupBklightSleepTime(int sleepseconds);

void DGeniusEngine :: run( )
{

    canvas->update( );
    if(false == backlightstatus()) backlightctrl(true);
    while(1)
    {
        struct tm *tm_ptr;
        time_t now;
        time(&now); 
        tm_ptr = localtime(&now);

        incomecheck(); // hide canvas while incoming call and show canvas after incoming call

        if (!ishide) {
            iconcheckBT();
            iconcheckNoti();
        }

        if (!ishide && !(view->isActiveWindow()) ) {    //check if screensaver is forced background
//            showScreenSaver();
            view->showFullScreen();
            view->show();
            printf("force active:%d\n",view->isActiveWindow());
        }else if( ishide && view->isActiveWindow() )
        {
            printf("force inactive\n");
//            hideScreenSaver();
            view->showMinimized();
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
            bool lighton = false;;
            
            if(true == backlightstatus()){
                timeout ++;
                if(timeout > 3) {
                    timeout = 0;
                    backlightctrl(false);
                }else
                {
                    lighton = true;
                }
            }
            if( true == lighton &&  tm_ptr->tm_sec == 0 )
            {
               canvas->updateScreenSprite( );
               canvas->update();
            }
        }

        if( true == keypressed)
        {
            if ( timecnt > 1 ){
                timecnt = 0;
                hidepressed = false;
                showpressed = false;
                keypressed = false;
                canvas->hideString();
                canvas->hideSpriteLock();
                canvas->update();
            }
            timecnt ++;
        }
        autolock( );        
        sleep(1);
    }
}

void DGeniusEngine :: pointerPressed( int x, int y )
{
    std::cout << "Pointer pressed on " << x << "," << y << std::endl;
    if( x > 220 && y < 10 )
    {
        DApplication :: exit();
        ::exit( 0 );
    }
    
}

void DGeniusEngine :: pointerDragged( int x, int y )
{
    //std::cout << "Pointer dragged on " << x << "," << y << std::endl;
}

void DGeniusEngine :: pointerReleased( int x, int y )
{
    //std::cout << "Pointer released on " << x << "," << y << std::endl;
}

void DGeniusEngine :: keyPressed(int keycode)
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    keypressed = true ;
    timecnt = 0;
    if (false == ishide) {
        timeout = 0;
        if(false == backlightstatus()) {
            iconcheckBT();
            iconcheckNoti();
            canvas->updateScreenSprite();
            canvas->update();
            backlightctrl(true);
        }
        if(4145 == keycode)
        {
            hidepressed = true ;
            #if 0
            canvas->showString("\n  Unlock: Now Press Center Key.");
            #else
            canvas->showString(utf8->toUnicode("\n         解锁:请再按圆形确认键."));
            #endif
            canvas->showSpriteLock();
            canvas->update();
        }else if( 4100 == keycode && true == hidepressed )
        {
            hidepressed = false ;
            canvas->hideString();
            canvas->hideSpriteLock();
           hideScreenSaver();
        }else
        {
            hidepressed = false;
            #if 0
            canvas->showString("\n  Unlock: First Press Hangup Key, \n   Then Press Center Key.");
            #else
            canvas->showString(utf8->toUnicode("\n         解锁:请先按挂机键,\n               再按圆形确认键."));
            #endif
            canvas->showSpriteLock();
            canvas->update();
        }
    }else
    {
        if (4145 == keycode) {
            showpressed = true;
        }else if(4100 == keycode && true == showpressed)
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
    PM_setupLcdSleepTime(0);
}

void DGeniusEngine :: hideScreenSaver()
{
    printf("canvas Minimized\n");
    view->showMinimized();
    ishide = true;
    PM_setupLcdSleepTime(30);
}

void DGeniusEngine :: backlightctrl(bool onoff)
{
    int sleeptime = 0;
//    int keylighttime = 0;
    if (onoff) {
        sleeptime = 60;
        //keylighttime = 10;
    }

//    PM_setupLcdSleepTime(sleeptime);
//    PM_setupKeylightSleepTime(keylighttime);

    int fbd = open("/dev/fb0", O_RDWR);
    if(fbd<0){
    printf("can not open fb0\n");
    return;
    }
    if(onoff){
        ioctl(fbd,FBIOBLANK,0);
        ioctl(fbd,FBIOSETBRIGHTNESS,0x19);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
    }else{
        ioctl(fbd,FBIOSETBRIGHTNESS,0);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
        ioctl(fbd,FBIOBLANK,0x3);
    }
    close(fbd);

    printf("Set backlight %s[%d]\n",onoff == true ?  "ON" : "OFF",sleeptime);
}

int DGeniusEngine :: backlightstatus( )
{

    int fbd = open("/dev/fb0", O_RDWR);
    int status = 0;
    if(fbd<0){
    printf("can not open fb0\n");
    return false ;
    }
 
    //ioctl(fbd,FBIOBLANK,0);
    if(ioctl(fbd,FBIOGETBKLIGHT,&status) < 0)
    {
        printf("backlight status : get status error\n");
    }

    close(fbd);
    printf("Backlight status %d\n",status);
    return status;
}

void DGeniusEngine :: iconcheckBT( )
{
    static bool btstatus = false;
    bool bt_ = UTIL_GetBtStatus();
    if(btstatus != bt_){
        btstatus = bt_;
        if (btstatus) {
            canvas->showSpriteBT();
        }else
        {
            canvas->hideSpriteBT ();
        }
        canvas->update();
    }
}

void DGeniusEngine :: iconcheckNoti( )
{
    static bool fnotification = false;
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
}

void DGeniusEngine :: incomecheck( )
{
    static bool fincomecall = false;
    static bool checktwice = false;
    bool incall_ = UTIL_GetIncomingCallStatus();
    if (incall_ &&  !ishide) {
        hideScreenSaver();
        if(!backlightstatus()) backlightctrl(true);
        timeout = 0;
        fincomecall = incall_;
    }else if (fincomecall && !incall_ && ishide && !UTIL_GetCallConnectedStatus()) {
        if( checktwice)
        {
            showScreenSaver();
            if(backlightstatus()) backlightctrl(false);
            fincomecall = incall_;
            checktwice = false ;
        }else
        {
            printf("Call Connection,check again................\n");
            checktwice = true ;
        }
    }
}

void DGeniusEngine :: setAutolockInterval(int interval)
{
    autolock_interval = interval;
    printf("Set autolock interval: %d\n",interval);
}

void DGeniusEngine :: autolock( )
{
    if (0 == autolock_interval || !ishide) {   //if autolock_interval equals 0, then autolock is disabled
        return;
    }
    static int cnt = 0;
    if (UTIL_GetTimingPhoneLock()) {
        cnt = 0;
    }else
    {
        cnt ++;
    }
    if (cnt > autolock_interval) {
            printf("Screen is autolocked...\n");
            cnt = 0;
            showScreenSaver();
            backlightctrl(false);
    }
}
