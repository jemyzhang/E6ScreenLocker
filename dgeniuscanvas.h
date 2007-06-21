
#ifndef _DGENIUSCANVAS_H_
#define _DGENIUSCANVAS_H_

#include <qcanvas.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <stdio.h>
#include "pictures.h"

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

            for(int i = 0; i < 16; i++)
            {
                ScreenImg_array[i] = new QCanvasPixmapArray( );
                Screen_sprite[i] = new QCanvasSprite(ScreenImg_array[i],this);
            }
            setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
        }

        
        virtual ~DGeniusCanvas( ) {
            delete[] Screen_sprite;
            delete[] Screen_img;
            delete[] ScreenImg_array; }

};

#endif

