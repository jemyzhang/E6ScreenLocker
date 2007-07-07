
#include <ZMessageBox.h>

#include "screenlockengine.h"
#include "dapplication.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>

extern "C" int  UTIL_GetIncomingCallStatus();
extern "C" int  UTIL_GetCallConnectedStatus(void);  
extern "C" int  UTIL_GetTimingPhoneLock();

extern "C" int PM_setupLcdSleepTime(int sleepseconds);

extern "C" int  UTIL_GetPhoneInCall();

QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

void ScreenLockEngine :: initial( )
{
    startup = true;
    ishide = false;
    getSysDefine( );
    LoadConfig();
    if(autolock_interval == 0){
        ifautolock = false;        
    }
    if (ifshowdeskicon) {
        canvas->setAutoLockimg(ifautolock);
    }else
    {
        canvas->hideAutoLockimg();
    }
    PM_setupLcdSleepTime(0);
    backlightctrl(true,lock_brightness);
    printf("initial over...\n");
}

void ScreenLockEngine :: checkprocess( )
{
    struct tm *tm_ptr;
    time_t now;
    time(&now); 
    tm_ptr = localtime(&now);
    if (tm_ptr->tm_sec == 0) {
        if(ishide){
            req_update = true;
        }else
        {
            canvas->updateScreenSprite( );
        }
    }

    if (ishide) {
        timeout = 0;    //for backlight timeout
    }else
    {
        if(timeout > lock_light_timeout) {
            timeout = 0;
            backlightctrl(false);
        }
        if(req_lightoff) timeout ++;
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
    autolock( ifautolock );        
    incomecheck(); // hide canvas while incoming call and show canvas after incoming call
}

void ScreenLockEngine :: QCopReceived(int message)
{
    static bool flag = false;
    if (message == 5) {
        if (!ishide && !(view->isActiveWindow()) && false == flag && false == startup) {
            flag = true;
            backlightctrl(false);
            showScreenSaver();
            printf("force active:%d\n",view->isActiveWindow());
        }else
        {
            flag = false;
            startup = false;
        }
        if(!ishide && false == startup ) {
            if(backlightstatus()) req_lightoff = true;
        }
    }
}

void ScreenLockEngine :: PropertyReceived( )
{
    if (!ishide && !startup && ifhide4sms) {
        printf("What's happend, let's see...\n");
        hideScreenSaver();
        if(!backlightstatus()) backlightctrl(true,lock_brightness);
        timeout = 0;
    }
}

void ScreenLockEngine :: pointerPressed( int x, int y )
{
//    std::cout << "Pointer pressed on " << x << "," << y << std::endl;
    if( x > 220 && y < 10 )
    {
        beforeterminate( );
        DApplication :: exit();
        ::exit( 0 );
    }else if (x > 110 && y > 300 && x < 130) {
        if(!ifshowdeskicon) return;
        if (ifautolock) {
            ifautolock = false;
        }else
        {
            ifautolock = true;
        }
        printf("auto lock mode: %d\n",ifautolock);
        canvas->setAutoLockimg(ifautolock);
    }
    
}

void ScreenLockEngine :: pointerDragged( int x, int y )
{
    //std::cout << "Pointer dragged on " << x << "," << y << std::endl;
}

void ScreenLockEngine :: pointerReleased( int x, int y )
{
    //std::cout << "Pointer released on " << x << "," << y << std::endl;
}

void ScreenLockEngine :: keyPressed(int keycode)
{
    keypressed = true ;
    timecnt = 0;
    if (false == ishide) {
        timeout = 0;
        showScreenSaver();
        if(4145 == keycode)
        {
            hidepressed = true ;
            if (ifshowinstruction) {
                if(0 == LANGID) {
                    canvas->showString("\n  Unlock: Now Press Right Key.");
                }else{
                    canvas->showString(utf8->toUnicode("\n         解锁:请再按右方向键."));
                }
            }
            canvas->showSpriteLock();
            canvas->update();
        }else if( 4116 == keycode && true == hidepressed )
        {
            hidepressed = false ;
            hideScreenSaver();
        }else
        {
            hidepressed = false;
            if (ifshowinstruction) {
                if( 0 == LANGID ) {
                    canvas->showString("\n  Unlock: First Press Hangup Key, \n   Then Press Right Key.");
                }else{
                    canvas->showString(utf8->toUnicode("\n         解锁:请先按挂机键,\n               再按右方向键."));
                }
            }
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

void ScreenLockEngine :: setview(QCanvasView *canvasview,DApplication *app)
{
    view = canvasview;
    DApp = app;
}

void ScreenLockEngine :: showScreenSaver()
{
    printf("canvas show\n");

    canvas->iconcheckBT();
    canvas->iconcheckNoti();

    if(true == req_update) {
        canvas->updateScreenSprite( );
        req_update = false;
    }

    view->showFullScreen();
    view->show();
    ishide = false;
    canvas->advance();

    if(false == backlightstatus()) {
        printf("Tune LCD Sleep Mode to None...\n");
        PM_setupLcdSleepTime(0);
    }
    backlightctrl(true,lock_brightness);
}

void ScreenLockEngine :: hideScreenSaver()
{
    printf("canvas Minimized\n");
    //view->showMinimized();
    view->hide();
    ishide = true;
    backlightctrl(true,sys_brightness);
    if (0 == autolock_interval) {
        PM_setupLcdSleepTime(sys_lcdsleeptime);
    }
}

void ScreenLockEngine :: backlightctrl(bool onoff,int brightness)
{
    int fbd = open("/dev/fb0", O_RDWR);
    if(fbd<0){
    printf("can not open fb0\n");
    return;
    }
    if(onoff){
        ioctl(fbd,FBIOBLANK,0);
        ioctl(fbd,FBIOSETBRIGHTNESS,brightness);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
        req_lightoff = true;
    }else{
        ioctl(fbd,FBIOSETBRIGHTNESS,0);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
        ioctl(fbd,FBIOBLANK,0x3);
        req_lightoff = false;
    }
    close(fbd);

    printf("Set backlight %s\n",onoff == true ?  "ON" : "OFF");
    keylightctrl(onoff);
}

void ScreenLockEngine :: keylightctrl(bool onoff)
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

int ScreenLockEngine :: backlightstatus( )
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

void ScreenLockEngine :: incomecheck( )
{
    static bool fincomecall = false;
    static bool checktwice = false;
    static bool fcalling = false;

    bool incall_ = UTIL_GetIncomingCallStatus();
    bool calling = UTIL_GetPhoneInCall();
    if (incall_ &&  !ishide) {
        printf("Incoming Call, canvas hide...\n");
        hideScreenSaver();
        if(!backlightstatus()) backlightctrl(true,lock_brightness);
        timeout = 0;
        fincomecall = incall_;
    }else if (fincomecall && !incall_ && ishide && !calling) {
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
    }else if (ishide && calling && fcalling != calling)
    {
        printf("Call connected...\n");
        fcalling = calling;
        PM_setupLcdSleepTime(sys_lcdsleeptime); //restore lcd sleep time
    }else if(!calling) fcalling = calling; //reset call status
}

void ScreenLockEngine :: setAutolockInterval(int interval)
{
    autolock_interval = interval;
    printf("Set autolock interval: %d\n",interval);
}

void ScreenLockEngine :: autolock(bool ctrl )
{
    static bool updatelcdsleep = false;

    if (!ishide) {   //if autolock_interval equals 0, then autolock is disabled
        updatelcdsleep = false;
        return;
    }

    if (false == ctrl) {
        if ( false == updatelcdsleep) {
            printf("Recover lcdsleeptime to:%d\n",sys_lcdsleeptime);
            PM_setupLcdSleepTime(sys_lcdsleeptime);
            updatelcdsleep = true;
        }
        return;
    }else
    {
        updatelcdsleep = false;
    }

    static int cnt = 0;
    if (UTIL_GetTimingPhoneLock() || UTIL_GetPhoneInCall()) {   //disable autolock while calling
        cnt = 0;
    }else
    {
        cnt ++;
    }
    if (cnt > autolock_interval) {
            printf("Screen is autolocked...\n");
            cnt = 0;
            backlightctrl(false,lock_brightness);
            showScreenSaver();
    }
}

void ScreenLockEngine :: getSysDefine( )
{
    printf("Load system settings...");
    QFile file("/ezx_user/download/appwrite/setup/ezx_system.cfg");

    sys_brightness = 0x19;
    sys_lcdsleeptime = 30;

    if(file.exists () && file.open( IO_ReadOnly )) {
        QString line;
        QTextStream stream(&file);
        char tmp[512];
        while(!stream.eof()) {
            line = stream.readLine();
            if(line.contains("Brightness = ")) {
                sscanf(line.latin1(),"%s = %d",tmp,&sys_brightness);
                printf("brightness:%d ",sys_brightness);
            }else if(line.contains("LcdSleepTime = ")){
                sscanf(line.latin1(),"%s = %d",tmp,&sys_lcdsleeptime);
                sys_lcdsleeptime = lcdsleeptime_ref[sys_lcdsleeptime];
                printf("lcdsleeptime:%d\n",sys_lcdsleeptime);
            }
        }
        file.close();
    }else{
        printf("failed,use default value\n");
    }

}

void ScreenLockEngine :: LoadConfig()
{
    ifautolock = true;
    ifhide4sms = true;
    ifshowdeskicon = true;
    LANGID = 1;

    autolock_interval = 20;
    lock_brightness = 10;
    lock_light_timeout = 3;
    ifshowinstruction = true;
    char tmp[10];
    for(int i = 0; i < sizeof(tmp); i++){
        tmp[i] = 0;
    }
    if(DApplication::LoadAppConfig("AutoLockONOFF",tmp)){
        ifautolock = atoi(tmp);
        for(int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("AutoLockPeriod",tmp)){
        autolock_interval = atoi(tmp);
        for(int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("LockOnBrightness",tmp)){
        lock_brightness = atoi(tmp);
        for(int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("LockLightOnTimeout",tmp)){
        lock_light_timeout = atoi(tmp);
        for(int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("ShowInstruction",tmp)){
        ifshowinstruction = atoi(tmp);
    }
    if(DApplication::LoadAppConfig("HideWhileSMSCome",tmp)){
        ifhide4sms = atoi(tmp);
    }
    if(DApplication::LoadAppConfig("ShowAutoLockIcon",tmp)){
        ifshowdeskicon = atoi(tmp);
    }
    if(DApplication::LoadAppConfig("LANGUAGE",tmp)){
        LANGID = atoi(tmp);
    }
}

void ScreenLockEngine :: SaveConfig( )
{
    DApplication::SaveAppConfig("BackgroundImage",canvas->getBackgroundimg());
    DApplication::SaveAppConfig("ShowAutoLockIcon",QString::number(ifshowdeskicon).latin1());
    DApplication::SaveAppConfig("AutoLockONOFF",QString::number(ifautolock).latin1());
    DApplication::SaveAppConfig("AutoLockPeriod",QString::number(autolock_interval).latin1());
    DApplication::SaveAppConfig("LockLightOnTimeout",QString::number(lock_light_timeout).latin1());
    DApplication::SaveAppConfig("LockOnBrightness",QString::number(lock_brightness).latin1());
    DApplication::SaveAppConfig("LockLightOnTimeout",QString::number(lock_light_timeout).latin1());
    DApplication::SaveAppConfig("ShowInstruction",QString::number(ifshowinstruction).latin1());
    DApplication::SaveAppConfig("HideWhileSMSCome",QString::number(ifhide4sms).latin1());
    DApplication::SaveAppConfig("LANGUAGE",QString::number(LANGID).latin1());
}
void ScreenLockEngine :: beforeterminate( )
{
    SaveConfig();
    PM_setupLcdSleepTime(sys_lcdsleeptime);
    backlightctrl(true,sys_brightness);
}
