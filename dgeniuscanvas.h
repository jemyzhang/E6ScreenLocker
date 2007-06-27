
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
    QCanvasPixmap *img[12];
    QCanvasPixmapArray imgarray[12];
    QCanvasPixmap *bg_img;
    QCanvasPixmapArray *bgimg_array;

    QCanvasSprite *Screen_sprite[12];
    QCanvasSprite *Lock_sprite;
    QCanvasSprite *sms_sprite;
    QCanvasSprite *Call_sprite;
    QCanvasSprite *BT_sprite;
    QCanvasSprite *BG_sprite;
    QCanvasText *presskeyMsg;
    char digi_buf[12];
	char digi_backup[12];

    void init( );
    void get_time_ch( );
    void loadBGimg( );

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

		    QCanvasPixmap *Lock_img;
            QCanvasPixmapArray *Lockimg_array;

            //prepare background image	 
            loadBGimg();

            for( int i = 0; i < 5; i++) {
                QCanvasPixmap *img = new QCanvasPixmap(static_screen_xpm[i],QPoint(static_screen_pos[i].x , static_screen_pos[i].y));
                QCanvasPixmapArray *imgarray = new QCanvasPixmapArray( );
                imgarray->setImage(0,img);
                QCanvasSprite *static_Screen_sprite = new QCanvasSprite(imgarray,this);
                static_Screen_sprite->show();
            }

            //prepare lock image	 
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

            //prepare infomation text
     		presskeyMsg = new QCanvasText("Unlock: Now Press Center Key",this);
    		QFont *strfont = new QFont();
    	 	strfont->setPixelSize(16);
    	 	presskeyMsg->setFont(*strfont);
    	 	presskeyMsg->setColor(qRgb( 180,64,64));

            //setup background color
            setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
        }

        
        virtual ~DGeniusCanvas( ) {
            printf("delete image source\n");
            delete[] Screen_sprite;
            delete[] img;
            delete bg_img;
            delete bgimg_array;
            printf("delete image source\n");
				}

};

#endif

