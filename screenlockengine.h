
#ifndef _SCREENLOCKENGINE_H_
#define _SCREENLOCKENGINE_H_

#include <qthread.h>
#include "dapplication.h"
#include "screenlockcanvas.h"

/* added for backlight control */
#define FBIOSETBKLIGHT          0x4619
#define FBIOGETBKLIGHT          0x461A
#define FBIOSETBRIGHTNESS       0x461B 
#define FBIOGETBRIGHTNESS       0x461C 


class ScreenLockEngine : public QThread, PointerListener
{
    private:
        ScreenLockCanvas *canvas;
        QCanvasView *view;
        DApplication *DApp;
        bool keypressed;
        bool hidepressed;
        bool showpressed;
        bool ishide;
        bool req_update;
        unsigned char timecnt;
        unsigned char timeout;
        int autolock_interval;
        int sys_brightness;
        int sys_lcdsleeptime;
        void showScreenSaver( );
        void hideScreenSaver( );
        void backlightctrl(bool onoff,int brightness = 10);
        void keylightctrl(bool onoff);
        int backlightstatus( );
        void incomecheck( );
        void autolock( ); //screen auto lock
        void getSysDefine( ); //get system defined brightness and lcd sleep time

    public:
        ScreenLockEngine( ScreenLockCanvas *canvas_ )
        {
            canvas = canvas_;
            keypressed = false;
            hidepressed = false;
            showpressed = false;
            ishide = false;
            timecnt = 0;
        }
        
        virtual void run( );
        void setview(QCanvasView *canvasview,DApplication *app);
        void pointerPressed( int x, int y );
        void pointerDragged( int x, int y );
        void pointerReleased( int x, int y );
        void keyPressed(int keycode);
        void setAutolockInterval(int interval);
        void QCopReceived(int message);
};

#endif

