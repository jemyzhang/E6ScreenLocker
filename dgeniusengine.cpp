
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


QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

void DGeniusEngine :: run( )
{
    PM_setupLcdSleepTime(0);
    ishide = false;
    showScreenSaver();
    while(1)
    {
        struct tm *tm_ptr;
        time_t now;
        time(&now); 
        tm_ptr = localtime(&now);

        if (!ishide && !(view->isActiveWindow()) ) {    //check if screensaver is forced background
            view->show();
            view->setActiveWindow();
            printf("force active:%d\n",view->isActiveWindow());
        }else if( ishide && view->isActiveWindow() )
        {
            printf("force inactive\n");
            view->hide();
        }

        if (ishide) {
            timeout = 0;    //for backlight timeout
        }else
        {
            timeout ++;
            if(timeout > 3) {
                timeout = 0;
                if(true == backlightstatus()) backlightctrl(false);
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
        incomecheck(); // hide canvas while incoming call and show canvas after incoming call
        sleep(1);
    }
}

void DGeniusEngine :: QCopReceived(int message)
{
//    if (message == 6) {
//        backlightctrl(false);
//    }
}

void DGeniusEngine :: pointerPressed( int x, int y )
{
    std::cout << "Pointer pressed on " << x << "," << y << std::endl;
    if( x > 220 && y < 10 )
    {
        PM_setupLcdSleepTime(30);
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

    keypressed = true ;
    timecnt = 0;
    if (false == ishide) {
        timeout = 0;
        showScreenSaver();
        if(4145 == keycode)
        {
            hidepressed = true ;
            #if 0
            canvas->showString("\n  Unlock: Now Press Left Key.");
            #else
            canvas->showString(utf8->toUnicode("\n         解锁:请再按左方向键."));
            #endif
            canvas->showSpriteLock();
            canvas->update();
        }else if( 4116 == keycode && true == hidepressed )
        {
            hidepressed = false ;
            hideScreenSaver();
        }else
        {
            hidepressed = false;
            #if 0
            canvas->showString("\n  Unlock: First Press Hangup Key, \n   Then Press Left Key.");
            #else
            canvas->showString(utf8->toUnicode("\n         解锁:请先按挂机键,\n               再按左方向键."));
            #endif
            canvas->showSpriteLock();
            canvas->update();
        }
    }else
    {
        if (4145 == keycode) {
            showpressed = true;
        }else if(4116 == keycode && true == showpressed)
        {
            showpressed = false;
            showScreenSaver();
        }else
        {
            showpressed = false;
        }
    }
}

void DGeniusEngine :: setview(QCanvasView *canvasview,DApplication *app)
{
    view = canvasview;
    DApp = app;
}

void DGeniusEngine :: showScreenSaver()
{
    printf("canvas show\n");

    iconcheckBT();
    iconcheckNoti();
    canvas->updateScreenSprite( );
    canvas->update();
    if (ishide) {
        view->showFullScreen();
        view->show();
        ishide = false;
    }
    if(false == backlightstatus()) backlightctrl(true);
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

    printf("Set backlight %s\n",onoff == true ?  "ON" : "OFF");
    keylightctrl(onoff);
}

void DGeniusEngine :: keylightctrl(bool onoff)
{
    int keyd = open("/dev/keylight", O_RDWR);
    if(keyd<0){
    printf("can not open keylight device\n");
    return;
    }
    if(onoff){
        ioctl(keyd,0xf0,0);
    }else{
        ioctl(keyd,0xf2,0);
    }
    close(keyd);

    printf("Set keylight %s\n",onoff == true ?  "ON" : "OFF");
}

int DGeniusEngine :: backlightstatus( )
{

    int fbd = open("/dev/fb0", O_RDWR);
    int status = 0;
    if(fbd<0){
    printf("can not open fb0\n");
    return false ;
    }
 
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
    }
}

void DGeniusEngine :: incomecheck( )
{
    static bool fincomecall = false;
    static bool checktwice = false;
    bool incall_ = UTIL_GetIncomingCallStatus();
    if (incall_ &&  !ishide) {
        printf("Incoming Call, canvas hide...\n");
        hideScreenSaver();
        if(!backlightstatus()) backlightctrl(true);
        timeout = 0;
        fincomecall = incall_;
    }else if (fincomecall && !incall_ && ishide && !UTIL_GetCallConnectedStatus()) {
        if( checktwice)
        {
            showScreenSaver();
            fincomecall = incall_;
            checktwice = false ;
        }else
        {
            printf("Call rejected or missed, check again................\n");
            checktwice = true ;
        }
    }else if (fincomecall && !incall_ && ishide && UTIL_GetCallConnectedStatus())
    {
        printf("Call connected...\n");
        fincomecall = false;    //if the phone is picked up, do not lock the phone after hangup
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
    if (UTIL_GetTimingPhoneLock() || UTIL_GetCallConnectedStatus()) {   //disable autolock while calling
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

