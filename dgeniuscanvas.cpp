
#include <qcanvas.h>
#include <stdio.h>
#include "dgeniuscanvas.h"

// 2126-8423 - Ricardo


void DGeniusCanvas :: init( )
{
        for( int i = 0; i < 12; i++)
        {
            Screen_sprite[i] = NULL;
            digi_backup[i] = 0xff;
        }
        for( int i = 0; i < 4; i++) {
            QCanvasPixmap *img = new QCanvasPixmap(static_screen_xpm[i],QPoint(static_screen_pos[i].x , static_screen_pos[i].y));
            QCanvasPixmapArray *imgarray = new QCanvasPixmapArray( );
            imgarray->setImage(0,img);
            QCanvasSprite *static_Screen_sprite = new QCanvasSprite(imgarray,this);
            static_Screen_sprite->show();
        }

}

void DGeniusCanvas :: get_time_ch( )
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

void DGeniusCanvas :: showSpriteLock( )
{
    Lock_sprite->show( );
}

void DGeniusCanvas :: hideSpriteLock( )
{
    Lock_sprite->hide( );
}

void DGeniusCanvas :: showSpritesms( )
{
    sms_sprite->show( );
}

void DGeniusCanvas :: hideSpritesms( )
{
    sms_sprite->hide( );
}

void DGeniusCanvas :: showSpriteCall( )
{
    Call_sprite->show( );
}

void DGeniusCanvas :: hideSpriteCall( )
{
    Call_sprite->hide( );
}

void DGeniusCanvas :: updateScreenSprite( )
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
                }
            QCanvasPixmap *img = new QCanvasPixmap((const char**)screen_xpm[i],QPoint(screen_pos[i].x , screen_pos[i].y));
            QCanvasPixmapArray *imgarray = new QCanvasPixmapArray( );
            imgarray->setImage(0,img);
            Screen_sprite[i] = new QCanvasSprite(imgarray,this);
            Screen_sprite[i]->show();
            digi_backup[i] = digi_buf[i];
            }
    }
    printf("canvas updated\n");
}


void DGeniusCanvas :: showString( const QString& info)
{
    presskeyMsg->setText(info);
    presskeyMsg->show( );
}

void DGeniusCanvas :: hideString( )
{
    presskeyMsg->hide();
}

