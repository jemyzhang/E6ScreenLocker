
#ifndef _SCREENLOCKCANVAS_H_
#define _SCREENLOCKCANVAS_H_

#include <qcanvas.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <stdio.h>
#include <time.h>
#include "dapplication.h"

#include <qstring.h>
#include <qtextstream.h>
#include <qfile.h>

typedef struct SKIN_CONFIG_st {
    int width;
    int height;
    int px;
    int py;
    int cx;
    int cy;
};

typedef struct SKIN_CONFIGS {
    char *name;
    SKIN_CONFIG_st *skin;
};

class ScreenLockCanvas : public QCanvas
{
    QPixmap *skin;
    QPixmap *mainscreen;
    QCanvasPixmap *mainCanvas;
    QCanvasPixmapArray *main_array;
    QCanvasSprite *main_sprite;

    QFont *strfont;
    QString infomsg;

    char bgfile[1024];
    char digi_buf[12];
    bool ftextmsg;
    bool ficonlock;
    int ficonautolock;

    void init( );
    void get_time_ch( );
    void loadBGimg( );
  public:
    void loadSkinConfig(const QString& config);
    void loadSkin(const QString& png);
    void showSpriteLock( );
    void showSpritesms( );
    void showSpriteBT( );
    void showString( const QString& );
    void hideSpriteLock( );
    void hideSpritesms( );
    void hideSpriteBT( );
    void hideString( );
    void updateScreenSprite( );

    void textcheckmsg( );
    void iconcheckBT( );
    void iconcheckNoti( );
    void iconchecklock( );
    void iconcheckautolock( );
    void setAutoLockimg(bool status);   //status: true/false
    void hideAutoLockimg( );
    char* getBackgroundimg(){return bgfile;};
    void updates();

    ScreenLockCanvas( int w, int h ) : QCanvas( w, h )
    {
        init();
        //setup background color
        //setBackgroundColor( qRgb( 0xff, 0xff, 0xff) );
    }

    
    virtual ~ScreenLockCanvas( ) {
        dbg_printf("delete image source\n");
    		}

};

#endif

