
#include <qcanvas.h>
#include <stdio.h>
#include "screenlockcanvas.h"

// 2126-8423 - Ricardo

extern "C" int  UTIL_GetNotificationStatus();
extern "C" int  UTIL_GetBtStatus();


void ScreenLockCanvas :: init( )
{
        for( int i = 0; i < 12; i++)
        {
            Screen_sprite[i] = NULL;
            digi_backup[i] = 0xff;
        }
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
    if(tmp > 12){
        tmp = tmp - 12;
        digi_buf[4] = 1;
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
    printf("Ready for output date %s\n",ctime(&now));
}

void ScreenLockCanvas :: loadBGimg( )
{   
    bg_img = new QCanvasPixmap(QPixmap("screenlock.bg"),QPoint(0,0));
    if(bg_img->isNull())
    {
        delete bg_img;
    }else
    {
        printf("Loading background img..\n");
        bgimg_array = new QCanvasPixmapArray( );
        bgimg_array->setImage(0,bg_img);
        BG_sprite = new QCanvasSprite(bgimg_array,this);
        BG_sprite->show();
    }
}

void ScreenLockCanvas :: showSpriteLock( )
{
    Lock_sprite->show( );
}

void ScreenLockCanvas :: hideSpriteLock( )
{
    Lock_sprite->hide( );
}

void ScreenLockCanvas :: showSpritesms( )
{
    sms_sprite->show( );
}

void ScreenLockCanvas :: hideSpritesms( )
{
    sms_sprite->hide( );
}

void ScreenLockCanvas :: showSpriteCall( )
{
    Call_sprite->show( );
}

void ScreenLockCanvas :: hideSpriteCall( )
{
    Call_sprite->hide( );
}

void ScreenLockCanvas :: showSpriteBT()
{
    BT_sprite->show();
}

void ScreenLockCanvas :: hideSpriteBT()
{
    BT_sprite->hide();
}

void ScreenLockCanvas :: updateScreenSprite( )
{
    get_time_ch();
    char **screen_xpm[12];
    screen_xpm[0] = (char **)Cdigi_xpm[digi_buf[0]];
    screen_xpm[1] = (char **)Cdigi_xpm[digi_buf[1]];
    screen_xpm[2] = (char **)Cdigi_xpm[digi_buf[2]];
    screen_xpm[3] = (char **)Cdigi_xpm[digi_buf[3]];
    screen_xpm[4] = (char **)ampm_xpm[digi_buf[4]];

    screen_xpm[5] = (char **)Ddigi_xpm[digi_buf[5]];
    screen_xpm[6] = (char **)Ddigi_xpm[digi_buf[6]];
    screen_xpm[7] = (char **)Ddigi_xpm[digi_buf[7]];
    screen_xpm[8] = (char **)Ddigi_xpm[digi_buf[8]];
    screen_xpm[9] = (char **)Ddigi_xpm[digi_buf[9]];
    screen_xpm[10] = (char **)Ddigi_xpm[digi_buf[10]];

    screen_xpm[11] = (char **)Day_xpm[digi_buf[11]];

    printf("start to prepare image...\n");
    for(int i = 0; i < 12; i++)
    {
        if(digi_backup[i] != digi_buf[i]) {
            if(Screen_sprite[i])
                {
                removeItem(Screen_sprite[i]);
                delete Screen_sprite[i];
                Screen_sprite[i] = NULL;
                delete img[i];
                img[i] = NULL;
                }
            img[i] = new QCanvasPixmap((const char**)screen_xpm[i],QPoint(screen_pos[i].x , screen_pos[i].y));
            imgarray[i] = QCanvasPixmapArray( );
            imgarray[i].setImage(0,img[i]);
            Screen_sprite[i] = new QCanvasSprite(&imgarray[i],this);
            Screen_sprite[i]->show();
            digi_backup[i] = digi_buf[i];
            }else{
                Screen_sprite[i]->show();
            }            
    }
    printf("canvas updated\n");
}


void ScreenLockCanvas :: showString( const QString& info)
{
    presskeyMsg->setText(info);
    presskeyMsg->show( );
}

void ScreenLockCanvas :: hideString( )
{
    presskeyMsg->hide();
}

void ScreenLockCanvas :: iconcheckBT( )
{
    static bool btstatus = false;
    bool bt_ = UTIL_GetBtStatus();
    if(btstatus != bt_){
        btstatus = bt_;
        if (btstatus) {
            showSpriteBT();
        }else
        {
            hideSpriteBT ();
        }
    }
}

void ScreenLockCanvas :: iconcheckNoti( )
{
    static bool fnotification = false;
    bool fnotifi = UTIL_GetNotificationStatus();
    if(fnotification != fnotifi)
    {
        printf("Notification status: %d\n",fnotifi);
        fnotification = fnotifi;
        if(fnotification)
        {
            showSpritesms();
            showSpriteCall();
        }else
        {
            hideSpritesms();
            hideSpriteCall();
        }
    }
}

