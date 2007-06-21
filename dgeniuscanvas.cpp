
#include <qcanvas.h>
#include <iostream>
#include "dgeniuscanvas.h"

// 2126-8423 - Ricardo


void DGeniusCanvas :: init( )
{
    for (int i = 0; i< 16; i++) {
        Screen_img[i] = NULL;
    }
}


void DGeniusCanvas :: showSpriteLock( )
{
    //std::cout << "DGeniusCanvas::showSpriteR( )\n";
    Lock_sprite->show( );
}

void DGeniusCanvas :: hideSpriteLock( )
{
    //std::cout << "DGeniusCanvas::hideSpriteR( )\n";
    Lock_sprite->hide( );
}

void DGeniusCanvas :: setScreenSprite(char *digi )
{
    std::cout << "DGeniusCanvas::setScreenSprite( )\n";
/*    if (Screen_img[0] != NULL) {
        delete[] Screen_img;
    }
*/
    screen_xpm[0] = (char **)logo_xpm;
    screen_xpm[1] = (char **)Cdigi_xpm[*digi++];
    screen_xpm[2] = (char **)Cdigi_xpm[*digi++];
    screen_xpm[3] = (char **)Ccolon_xpm;
    screen_xpm[4] = (char **)Cdigi_xpm[*digi++];
    screen_xpm[5] = (char **)Cdigi_xpm[*digi++];
    screen_xpm[6] = (char **)ampm_xpm[*digi++];

    screen_xpm[7] = (char **)Ddigi_xpm[*digi++];
    screen_xpm[8] = (char **)Ddigi_xpm[*digi++];
    screen_xpm[9] = (char **)Dhori_xpm;
    screen_xpm[10] = (char **)Ddigi_xpm[*digi++];
    screen_xpm[11] = (char **)Ddigi_xpm[*digi++];
    screen_xpm[12] = (char **)Dhori_xpm;
    screen_xpm[13] = (char **)Ddigi_xpm[*digi++];
    screen_xpm[14] = (char **)Ddigi_xpm[*digi++];

    screen_xpm[15] = (char **)Day_xpm[*digi++];

    for(int i = 0; i < 16; i++)
    {
        Screen_sprite[i]->hide();
        Screen_img[i] = new QCanvasPixmap((const char**)screen_xpm[i],QPoint(screen_pos[i].x , screen_pos[i].y));
        ScreenImg_array[i]->setImage(0, Screen_img[i]);
        Screen_sprite[i]->show();
    }

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
/*
void DGeniusCanvas :: advance()
{
    std::cout<<"period output"<<std::endl;
}
*/
