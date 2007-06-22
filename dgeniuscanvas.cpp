
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


void DGeniusCanvas :: showSpriteLock( )
{
    Lock_sprite->show( );
}

void DGeniusCanvas :: hideSpriteLock( )
{
    Lock_sprite->hide( );
}

void DGeniusCanvas :: setScreenSprite(char *digi )
{
    char **screen_xpm[12];
    screen_xpm[0] = (char **)Cdigi_xpm[digi[0]];
    screen_xpm[1] = (char **)Cdigi_xpm[digi[1]];
    screen_xpm[2] = (char **)Cdigi_xpm[digi[2]];
    screen_xpm[3] = (char **)Cdigi_xpm[digi[3]];
    screen_xpm[4] = (char **)ampm_xpm[digi[4]];

    screen_xpm[5] = (char **)Ddigi_xpm[digi[5]];
    screen_xpm[6] = (char **)Ddigi_xpm[digi[6]];
    screen_xpm[7] = (char **)Ddigi_xpm[digi[7]];
    screen_xpm[8] = (char **)Ddigi_xpm[digi[8]];
    screen_xpm[9] = (char **)Ddigi_xpm[digi[9]];
    screen_xpm[10] = (char **)Ddigi_xpm[digi[10]];

    screen_xpm[11] = (char **)Day_xpm[digi[11]];

	printf("start to prepare image...\n");
    for(int i = 0; i < 12; i++)
    {
        if(digi_backup[i] != digi[i]) {
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
				digi_backup[i] = digi[i];
		}
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

