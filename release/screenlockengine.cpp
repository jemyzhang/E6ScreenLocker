#include "screenlockengine.h"

extern "C" int  UTIL_GetIncomingCallStatus();
extern "C" int  UTIL_GetCallConnectedStatus(void);  
extern "C" int  UTIL_GetTimingPhoneLock();
extern "C" int  UTIL_GetPhoneInCall();
extern "C" int PM_setupLcdSleepTime(int sleepseconds);

extern SKIN_CONFIG_st Skin_CloseClick;
extern SKIN_CONFIG_st Skin_AutoLock;

QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

void ScreenLockEngine :: initial( )
{
    startup = true;
    ishide = false;
    req_LCDnonesleep = false;
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
    dbg_printf("initial over...\n");
}

void ScreenLockEngine :: checkprocess( )
{
    struct tm *tm_ptr;
    time_t now;
    time(&now); 
    tm_ptr = localtime(&now);
    if (tm_ptr->tm_sec == 0) {
        if(ishide || !req_lightoff ){   //while canvas hide or screen light off
            req_update = true;
            dbg_printf("Request screen update...%s",ctime(&now));
        }else
        {
            canvas->updates( );
        }
    }

    if (ishide) {
        timeout = 0;    //disable backlight timeout
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
        }
        timecnt ++;
    }
    autolock( ifautolock );        
    //incomecheck(); // hide canvas while incoming call and show canvas after incoming call
}

void ScreenLockEngine :: QCopReceived(int message)
{
    static bool flag = false;
    if (message == 5) {
        if (!ishide && !(view->isActiveWindow()) && false == flag && false == startup) {
            flag = true;
            backlightctrl(false);
            showScreenSaver();
            dbg_printf("force active:%d\n",view->isActiveWindow());
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
        dbg_printf("What's happend, let's see...\n");
        hideScreenSaver();
        if(!backlightstatus()) backlightctrl(true,lock_brightness);
        timeout = 0;
    }
}

void ScreenLockEngine :: pointerPressed( int x, int y )
{
    if( x > Skin_CloseClick.cx && 
        y > Skin_CloseClick.cy &&
        x < Skin_CloseClick.cx + Skin_CloseClick.width &&
        y < Skin_CloseClick.cy + Skin_CloseClick.height)
    {
        beforeterminate( );
        DApplication :: exit();
        ::exit( 0 );
    }else if (x > Skin_AutoLock.cx && 
        y > Skin_AutoLock.cy &&
        x < Skin_AutoLock.cx + Skin_AutoLock.width &&
        y < Skin_AutoLock.cy + Skin_AutoLock.height) {
        if(!ifshowdeskicon) return;
        if (ifautolock) {
            ifautolock = false;
        }else
        {
            ifautolock = true;
        }
        dbg_printf("auto lock mode: %d\n",ifautolock);
        canvas->setAutoLockimg(ifautolock);
    }
    
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
                    canvas->showString("Unlock: Now Press Right Key.");
                }else{
                    canvas->showString(utf8->toUnicode("解锁:请再按右方向键."));
                }
            }
            canvas->showSpriteLock();
        }else if( 4116 == keycode && true == hidepressed )
        {
            hidepressed = false ;
            hideScreenSaver();
        }else
        {
            hidepressed = false;
            if (ifshowinstruction) {
                if( 0 == LANGID ) {
                    canvas->showString("Unlock: First Press Hangup Key.");
                }else{
                    canvas->showString(utf8->toUnicode("解锁:请先按挂机键."));
                }
            }
            canvas->showSpriteLock();
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
    dbg_printf("canvas show\n");

    if(true == req_update) {
        canvas->updates( );
        req_update = false;
    }

    view->showFullScreen();
    view->show();
    ishide = false;
    canvas->advance();

    if(true == req_LCDnonesleep) {
        dbg_printf("Tune LCD Sleep Mode to None...\n");
        PM_setupLcdSleepTime(0);
        req_LCDnonesleep = false;
    }
    backlightctrl(true,lock_brightness);
}

void ScreenLockEngine :: hideScreenSaver()
{
    dbg_printf("canvas Minimized\n");
    view->hide();
    ishide = true;
    backlightctrl(true,sys_brightness);
    if (false == ifautolock) {
        dbg_printf("Recover LCD sleep time to %d\n",sys_lcdsleeptime);
        PM_setupLcdSleepTime(sys_lcdsleeptime);
        req_LCDnonesleep = true;
    }
}

void ScreenLockEngine :: backlightctrl(bool onoff,int brightness)
{
    int fbd = open("/dev/fb0", O_RDWR);
    if(fbd<0){
    dbg_printf("can not open fb0\n");
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

    dbg_printf("Set backlight %s\n",onoff == true ?  "ON" : "OFF");
    keylightctrl(onoff);
}

void ScreenLockEngine :: keylightctrl(bool onoff)
{
    int keyd = open("/dev/keylight", O_RDWR);
    if(keyd<0){
    dbg_printf("can not open keylight device\n");
    return;
    }
    if(onoff){
        ioctl(keyd,0xf0,0);
    }else{
        ioctl(keyd,0xf2,0);
    }
    close(keyd);

    dbg_printf("Set keylight %s\n",onoff == true ?  "ON" : "OFF");
}

int ScreenLockEngine :: backlightstatus( )
{

    int fbd = open("/dev/fb0", O_RDWR);
    int status = 0;
    if(fbd<0){
    dbg_printf("can not open fb0\n");
    return false ;
    }
 
    if(ioctl(fbd,FBIOGETBKLIGHT,&status) < 0)
    {
        dbg_printf("backlight status : get status error\n");
    }

    close(fbd);
    dbg_printf("Backlight status %d\n",status);
    return status;
}

void ScreenLockEngine :: autolock(bool ctrl )
{
    if (!ishide || !ctrl) {   //if autolock_interval equals 0, then autolock is disabled
        return;
    }

    static int cnt = 0;
    if (UTIL_GetTimingPhoneLock() || UTIL_GetPhoneInCall()) {   //disable autolock while calling
        cnt = 0;
    }else
    {
        cnt ++;
    }
    if (cnt > autolock_interval) {
            dbg_printf("Screen is autolocked...\n");
            cnt = 0;
            backlightctrl(false,lock_brightness);
            showScreenSaver();
    }
}

void ScreenLockEngine :: getSysDefine( )
{
    dbg_printf("Load system settings...");
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
                dbg_printf("brightness:%d ",sys_brightness);
            }else if(line.contains("LcdSleepTime = ")){
                sscanf(line.latin1(),"%s = %d",tmp,&sys_lcdsleeptime);
                sys_lcdsleeptime = lcdsleeptime_ref[sys_lcdsleeptime];
                dbg_printf("lcdsleeptime:%d\n",sys_lcdsleeptime);
            }
        }
        file.close();
    }else{
        dbg_printf("failed,use default value\n");
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
    for(unsigned int i = 0; i < sizeof(tmp); i++){
        tmp[i] = 0;
    }
    if(DApplication::LoadAppConfig("AutoLockONOFF",tmp)){
        ifautolock = atoi(tmp);
        for(unsigned int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("AutoLockPeriod",tmp)){
        autolock_interval = atoi(tmp);
        for(unsigned int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("LockOnBrightness",tmp)){
        lock_brightness = atoi(tmp);
        for(unsigned int i = 0; i < sizeof(tmp); i++){
            tmp[i] = 0;
        }
    }
    if(DApplication::LoadAppConfig("LockLightOnTimeout",tmp)){
        lock_light_timeout = atoi(tmp);
        for(unsigned int i = 0; i < sizeof(tmp); i++){
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
