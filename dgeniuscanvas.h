
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
	QCanvasSprite * Time_sprite[4];
    QCanvasSprite *background_sprite;
    QCanvasSprite *R_sprite;
    QCanvasSprite *L_sprite;
    QCanvasSprite *U_sprite;
    QCanvasSprite *D_sprite;
    QCanvasSprite *C_sprite;
    QCanvasSprite *start_button_sprite;
    QCanvasSprite *exit_button_sprite;
    QCanvasText *presskeyMsg;

    void init( );
  public:
        
        /**
         * Shows the background picture.
         */
        void showBackground( );
        
        /**
         * Shows the Right sprite.
         */
        
        void showSpriteR( );
        void showSpriteL( );
        void showSpriteU( );
        void showSpriteD( );
        void showString( const QString& );
        void hideString( );
        void hideSpriteR( );
        void hideSpriteL( );
        void hideSpriteU( );
        void hideSpriteD( );

        
    
        DGeniusCanvas( int w, int h ) : QCanvas( w, h )
        {
       
     		presskeyMsg = new QCanvasText("Unlock: Now Press Center Key",this);
    		QFont *strfont = new QFont();
    	 	strfont->setPixelSize(16);
    	 	presskeyMsg->setFont(*strfont);
    	 	presskeyMsg->setColor(qRgb( 180,64,64));
	 
	//setup time array
		    QCanvasPixmap *Time_img[4];
            QCanvasPixmapArray *TimeImg_array[4];// = new QCanvasPixmapArray( );
        for(int i = 0; i < 4; i++)
		{
			Time_img[i] = new QCanvasPixmap(Cdigi_xpm[i],QPoint(-33 * i , -100));
			TimeImg_array[i] = new QCanvasPixmapArray( );
			TimeImg_array[i]->setImage(0, Time_img[i]);
            Time_sprite[i] = new QCanvasSprite(TimeImg_array[i],this);
            Time_sprite[i]->show();
		}
            setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
        }

        
        virtual ~DGeniusCanvas( ) { }

};

#endif

