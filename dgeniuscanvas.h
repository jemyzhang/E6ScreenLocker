
#ifndef _DGENIUSCANVAS_H_
#define _DGENIUSCANVAS_H_

#include <qcanvas.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <stdio.h>
#include "pictures.h"
#include <time.h>
#include "dgenius.h"

class DGeniusCanvas : public QCanvas
{
	QCanvasSprite *Screen_sprite[16];
    QCanvasSprite *Lock_sprite;
    QCanvasText *presskeyMsg;

    QCanvasPixmap *Screen_img[16];
    QCanvasPixmapArray *ScreenImg_array[16];// = new QCanvasPixmapArray( );

    char **screen_xpm[16];

    void init( );
  public:
        
        /**
         * Shows the Right sprite.
         */
        
        void showSpriteLock( );
        void showString( const QString& );
        void hideString( );
        void hideSpriteLock( );
        void setScreenSprite(char *digi);
        
    
        DGeniusCanvas( int w, int h ) : QCanvas( w, h )
        {
            init();
     		presskeyMsg = new QCanvasText("Unlock: Now Press Center Key",this);
    		QFont *strfont = new QFont();
    	 	strfont->setPixelSize(16);
    	 	presskeyMsg->setFont(*strfont);
    	 	presskeyMsg->setColor(qRgb( 180,64,64));
	 
	//setup time array
		    QCanvasPixmap *Lock_img;
            QCanvasPixmapArray *Lockimg_array;// = new QCanvasPixmapArray( );

            Lock_img = new QCanvasPixmap(Lock_xpm,QPoint(-80,-240));
            Lockimg_array = new QCanvasPixmapArray( );
            Lockimg_array->setImage(0,Lock_img);
            Lock_sprite = new QCanvasSprite(Lockimg_array,this);
/*
            for(int i = 0; i < 16; i++)
            {
                ScreenImg_array[i] = new QCanvasPixmapArray( );
                Screen_sprite[i] = new QCanvasSprite(ScreenImg_array[i],this);
            }
*/            struct tm *tm_ptr;
            time_t now;
            time(&now); 
            tm_ptr = localtime(&now);
            char buf[11];
            char tmp;
            tmp = tm_ptr->tm_hour;// + 8;
            buf[4] = 0;
            if(tmp > 12){
                tmp = tmp - 12;
                buf[4] = 1;
            }
            buf[0] = tmp / 10;
            buf[1] = tmp % 10;
            tmp = tm_ptr->tm_min;
            buf[2] = tmp / 10;
            buf[3] = tmp % 10;
            tmp = tm_ptr->tm_year % 100;
            buf[5] = tmp / 10;
            buf[6] = tmp % 10;
            tmp = tm_ptr->tm_mon + 1;
            buf[7] = tmp / 10;
            buf[8] = tmp % 10;
            tmp = tm_ptr->tm_mday;
            buf[9] = tmp / 10;
            buf[10] = tmp % 10;
            buf[11] = tm_ptr->tm_wday;
            setScreenSprite(buf);

            setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
        }

        
        virtual ~DGeniusCanvas( ) {
            delete[] Screen_sprite;
            delete[] Screen_img;
            delete[] ScreenImg_array; }

};

#endif

