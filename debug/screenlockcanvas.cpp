
#include "screenlockcanvas.h"

// 2126-8423 - Ricardo

extern "C" int  UTIL_GetNotificationStatus();
extern "C" int  UTIL_GetBtStatus();


SKIN_CONFIG_st Skin_TimeDigi;
SKIN_CONFIG_st Skin_TimeColon;
SKIN_CONFIG_st Skin_DateDigi;
SKIN_CONFIG_st Skin_DateHori;
SKIN_CONFIG_st Skin_Weekday;
SKIN_CONFIG_st Skin_ampm;
SKIN_CONFIG_st Skin_Logo;
SKIN_CONFIG_st Skin_AutoLock;
SKIN_CONFIG_st Skin_MissCall;
SKIN_CONFIG_st Skin_MissMail;
SKIN_CONFIG_st Skin_LockIcon;
SKIN_CONFIG_st Skin_CloseClick;
SKIN_CONFIG_st Skin_Bluetooth;

const SKIN_CONFIGS skinconfigs[] = {
    {"TimeDigi",&Skin_TimeDigi},
    {"TimeColon",&Skin_TimeColon},
    {"DateDigi",&Skin_DateDigi},
    {"DateHori",&Skin_DateHori},
    {"Weekday",&Skin_Weekday},
    {"AMPM",&Skin_ampm},
    {"Logo",&Skin_Logo},
    {"AutoLock",&Skin_AutoLock},
    {"MissCall",&Skin_MissCall},
    {"MissMail",&Skin_MissMail},
    {"LockIcon",&Skin_LockIcon},
    {"CloseClick",&Skin_CloseClick},
    {"Bluetooth",&Skin_Bluetooth},
};

void ScreenLockCanvas :: init( )
{
    skin = NULL;
    mainscreen = NULL;
    main_array = NULL;
    main_sprite = NULL;
    mainCanvas = NULL;

    ficonlock = false;

    dbg_printf("load skin config.......\n");
    loadSkinConfig("skin/skin.cfg");
    dbg_printf("load skin png......\n");
    loadSkin("skin/skin.png");

    strfont = new QFont();
    strfont->setPixelSize(16);
    updates();
    return;
}

void ScreenLockCanvas :: loadSkin(const QString &png)
{
    QPixmap *tmp = new QPixmap(png);//, "PNG", QPixmap::OrderedDither | QPixmap::OrderedAlphaDither);
    if (tmp) {
        tmp->setOptimization( QPixmap::NoOptim );
        if( skin ) 
            delete skin;
        skin = tmp;
    }else {
        dbg_printf("skin image load error\n");
    }
}

void ScreenLockCanvas :: loadSkinConfig(const QString& config)
{
    char configvalue[50];
    for(unsigned int i = 0 ; i < sizeof(skinconfigs)/sizeof(SKIN_CONFIGS); i++) {
        DApplication::LoadConfig(config,skinconfigs[i].name,configvalue);
        
        sscanf(configvalue,"{%d,%d,%d,%d,%d,%d}",
               &skinconfigs[i].skin->width,
               &skinconfigs[i].skin->height,
               &skinconfigs[i].skin->px,
               &skinconfigs[i].skin->py,
               &skinconfigs[i].skin->cx,
               &skinconfigs[i].skin->cy);
    }
    return;
}

void ScreenLockCanvas :: get_time_ch( )
{
    struct tm *tm_ptr;
    time_t now;
    time(&now); 
    tm_ptr = localtime(&now);

    char tmp;
    tmp = tm_ptr->tm_hour;// + 8;
    digi_buf[4] = 0;
    if (tmp > 11) {
        digi_buf[4] = 1;
    }
    if(tmp > 12){
        tmp = tmp - 12;
    }
    digi_buf[0] = tmp / 10;
    digi_buf[1] = tmp % 10;
    tmp = tm_ptr->tm_min;
    digi_buf[2] = tmp / 10;
    digi_buf[3] = tmp % 10;
    tmp = tm_ptr->tm_year % 100;
    digi_buf[5] = tmp / 10;
    digi_buf[6] = tmp % 10;
    tmp = tm_ptr->tm_mon + 1;
    digi_buf[7] = tmp / 10;
    digi_buf[8] = tmp % 10;
    tmp = tm_ptr->tm_mday;
    digi_buf[9] = tmp / 10;
    digi_buf[10] = tmp % 10;
    digi_buf[11] = tm_ptr->tm_wday;
    dbg_printf("Ready for output date %s\n",ctime(&now));
}

void ScreenLockCanvas :: loadBGimg( )
{
    DApplication::LoadAppConfig("BackgroundImage",bgfile);

    if (mainscreen) {
        delete mainscreen;
    }

    mainscreen = new QPixmap(bgfile);
    if (mainscreen->isNull()) {
        mainscreen = new QPixmap(236,316);
        mainscreen->fill();
    }
}

void ScreenLockCanvas :: showSpriteLock( )
{
    ficonlock = true;
    updates();
}

void ScreenLockCanvas :: hideSpriteLock( )
{
    ficonlock = false;
    updates();
}

void ScreenLockCanvas :: showSpritesms( )
{
    bitBlt(mainscreen,Skin_MissMail.cx,Skin_MissMail.cy,
           skin,Skin_MissMail.px,Skin_MissMail.py,
           Skin_MissMail.width,Skin_MissMail.height);
    bitBlt(mainscreen,Skin_MissCall.cx,Skin_MissCall.cy,
           skin,Skin_MissCall.px,Skin_MissCall.py,
           Skin_MissCall.width,Skin_MissCall.height);
}

void ScreenLockCanvas :: hideSpritesms( )
{
    return;
}

void ScreenLockCanvas :: showSpriteBT()
{
    bitBlt(mainscreen,Skin_Bluetooth.cx,Skin_Bluetooth.cy,
           skin,Skin_Bluetooth.px,Skin_Bluetooth.py,
           Skin_Bluetooth.width,Skin_Bluetooth.height);
}

void ScreenLockCanvas :: hideSpriteBT()
{
    return;
}

void ScreenLockCanvas :: updateScreenSprite( )
{
    loadBGimg();
    get_time_ch();

    dbg_printf("start to prepare image...\n");
    int offset[] = {0,0,1,1,0,0,0,1,1,2,2};
    int cx,cy,px,py;

    //prepare logo
    bitBlt(mainscreen,Skin_Logo.cx,Skin_Logo.cy,
           skin,Skin_Logo.px,Skin_Logo.py,
           Skin_Logo.width,Skin_Logo.height);
    //prepare time colon
    bitBlt(mainscreen,Skin_TimeColon.cx,Skin_TimeColon.cy,
           skin,Skin_TimeColon.px,Skin_TimeColon.py,
           Skin_TimeColon.width,Skin_TimeColon.height);
    //prepare date horizon
    bitBlt(mainscreen,Skin_DateHori.cx,Skin_DateHori.cy,
           skin,Skin_DateHori.px,Skin_DateHori.py,
           Skin_DateHori.width,Skin_DateHori.height);
    bitBlt(mainscreen,Skin_DateHori.cx + Skin_DateHori.width + Skin_DateDigi.width * 2 ,Skin_DateHori.cy,
           skin,Skin_DateHori.px,Skin_DateHori.py,
           Skin_DateHori.width,Skin_DateHori.height);
    //prepare close-click image  
    bitBlt(mainscreen,Skin_CloseClick.cx,Skin_CloseClick.cy,
           skin,Skin_CloseClick.px,Skin_CloseClick.py,
           Skin_CloseClick.width,Skin_CloseClick.height);
    /* update time */
    for(int i = 0; i < 4; i++)
    {
            cx = Skin_TimeDigi.cx + Skin_TimeDigi.width * i + offset[i] * (i - 1) * Skin_TimeColon.width;
            cy = Skin_TimeDigi.cy;
            px = Skin_TimeDigi.px + Skin_TimeDigi.width * digi_buf[i];
            py = Skin_TimeDigi.py;
            bitBlt(mainscreen,cx,cy,
                   skin,px,py,Skin_TimeDigi.width,Skin_TimeDigi.height,
                   Qt::SetROP, false);
    }
    /* update ampm */
        bitBlt(mainscreen,Skin_ampm.cx,Skin_ampm.cy,
               skin,Skin_ampm.px,Skin_ampm.py + Skin_ampm.height * digi_buf[4],
               Skin_ampm.width,Skin_ampm.height);//,
               //Qt::SetROP, false);
    /* update date */
    for(int i = 5; i < 11; i++) {
        bitBlt(mainscreen,Skin_DateDigi.cx +Skin_DateDigi.width * (i - 5) + offset[i] * Skin_DateHori.width ,Skin_DateDigi.cy,
               skin,Skin_DateDigi.px + Skin_DateDigi.width * digi_buf[i],Skin_DateDigi.py,
               Skin_DateDigi.width,Skin_DateDigi.height);//,
               //Qt::SetROP, false);
    }
    /* update weekday */
        bitBlt(mainscreen,Skin_Weekday.cx,Skin_Weekday.cy,
               skin,Skin_Weekday.px,Skin_Weekday.py + Skin_Weekday.height * digi_buf[11],
               Skin_Weekday.width,Skin_Weekday.height,
               Qt::SetROP, false);
        iconcheckBT();
        iconcheckNoti();
        iconchecklock();
        iconcheckautolock();
        textcheckmsg();
}


void ScreenLockCanvas :: showString( const QString& info)
{
    infomsg = info;
    ftextmsg = true;
    updates();
}

void ScreenLockCanvas :: hideString( )
{
    ftextmsg = false;
    updates();
}

void ScreenLockCanvas :: textcheckmsg()
{
    if (ftextmsg) {
        QPainter p(mainscreen);
        p.drawText(40,20,infomsg);
    }
}

void ScreenLockCanvas :: iconcheckBT( )
{
    bool btstatus = UTIL_GetBtStatus();
    if (btstatus) {
        showSpriteBT();
    }else
    {
        hideSpriteBT ();
    }
}

void ScreenLockCanvas :: iconcheckNoti( )
{
    bool fnotifi = UTIL_GetNotificationStatus();
    dbg_printf("Notification status: %d\n",fnotifi);
    if(fnotifi)
    {
        showSpritesms();
    }else
    {
        hideSpritesms();
    }
}


void ScreenLockCanvas :: iconchecklock()
{
    if (ficonlock) {
        bitBlt(mainscreen,Skin_LockIcon.cx,Skin_LockIcon.cy,
               skin,Skin_LockIcon.px,Skin_LockIcon.py,
               Skin_LockIcon.width,Skin_LockIcon.height);
    }
}

void ScreenLockCanvas :: iconcheckautolock()
{
    switch (ficonautolock) {
    case 1:
        bitBlt(mainscreen,Skin_AutoLock.cx,Skin_AutoLock.cy,
               skin,Skin_AutoLock.px,Skin_AutoLock.py,
               Skin_AutoLock.width,Skin_AutoLock.height);
        break;
    case 2:
        bitBlt(mainscreen,Skin_AutoLock.cx,Skin_AutoLock.cy,
               skin,Skin_AutoLock.px,Skin_AutoLock.py + Skin_AutoLock.height,
               Skin_AutoLock.width,Skin_AutoLock.height);
        break;
    case 3:
        break;
    default:
        bitBlt(mainscreen,Skin_AutoLock.cx,Skin_AutoLock.cy,
               skin,Skin_AutoLock.px,Skin_AutoLock.py,
               Skin_AutoLock.width,Skin_AutoLock.height);
        break;
    }
}

void ScreenLockCanvas :: setAutoLockimg(bool status)
{
    if(status){
        ficonautolock = 1;
    }else
    {
        ficonautolock = 2;
    }
    updates();
}


void ScreenLockCanvas :: hideAutoLockimg()
{
    ficonautolock = 3;
    updates( );
}

void ScreenLockCanvas :: updates()
{
    updateScreenSprite();
    if (main_sprite) {
        removeItem(main_sprite);
        delete main_sprite;
    }
    if (NULL == main_array) {
        main_array = new QCanvasPixmapArray( );
    }
    mainCanvas = new QCanvasPixmap(*mainscreen,QPoint(0,0));
    
    main_array->setImage(0,mainCanvas);
    main_sprite = new QCanvasSprite(main_array,this);
    main_sprite->show();
    
    update();
    dbg_printf("canvas updated\n");
}
