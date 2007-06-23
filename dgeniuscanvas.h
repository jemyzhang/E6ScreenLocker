
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
	QCanvasSprite *Screen_sprite[12];
    QCanvasSprite *Lock_sprite;
    QCanvasSprite *sms_sprite;
    QCanvasSprite *Call_sprite;
    QCanvasSprite *BT_sprite;
    QCanvasText *presskeyMsg;
    char digi_buf[12];
	char digi_backup[12];

    void init( );
    void get_time_ch();

  public:
        
     
        void showSpriteLock( );
        void showSpritesms( );
        void showSpriteBT( );
        void showSpriteCall( );
        void showString( const QString& );
        void hideSpriteLock( );
        void hideSpritesms( );
        void hideSpriteCall( );
        void hideSpriteBT( );
        void hideString( );
        void updateScreenSprite( );
        
    
        DGeniusCanvas( int w, int h ) : QCanvas( w, h )
        {
            init();
	//prepare infomation text
     		presskeyMsg = new QCanvasText("Unlock: Now Press Center Key",this);
    		QFont *strfont = new QFont();
    	 	strfont->setPixelSize(16);
    	 	presskeyMsg->setFont(*strfont);
    	 	presskeyMsg->setColor(qRgb( 180,64,64));

	//prepare lock image	 
		    QCanvasPixmap *Lock_img;
            QCanvasPixmapArray *Lockimg_array;

            Lock_img = new QCanvasPixmap(Lock_xpm,QPoint(-80,-240));
            Lockimg_array = new QCanvasPixmapArray( );
            Lockimg_array->setImage(0,Lock_img);
            Lock_sprite = new QCanvasSprite(Lockimg_array,this);


            Lock_img = new QCanvasPixmap(Mail_xpm,QPoint(-20,-120));
            Lockimg_array = new QCanvasPixmapArray( );
            Lockimg_array->setImage(0,Lock_img);
            sms_sprite = new QCanvasSprite(Lockimg_array,this);

            Lock_img = new QCanvasPixmap(Call_xpm,QPoint(-20,-120));
            Lockimg_array = new QCanvasPixmapArray( );
            Lockimg_array->setImage(0,Lock_img);
            Call_sprite = new QCanvasSprite(Lockimg_array,this);

            Lock_img = new QCanvasPixmap(bluetooth_xpm,QPoint(-22,-175));
            Lockimg_array = new QCanvasPixmapArray( );
            Lockimg_array->setImage(0,Lock_img);
            BT_sprite = new QCanvasSprite(Lockimg_array,this);
            //setup time array
            updateScreenSprite( );
	//setup background color
            setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
        }

        
        virtual ~DGeniusCanvas( ) {
            delete[] Screen_sprite;
            //delete[] Screen_img;
            //delete[] ScreenImg_array; 
				}

};

#endif

