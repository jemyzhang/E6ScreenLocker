
#ifndef _SCREENLOCKENGINE_H_
#define _SCREENLOCKENGINE_H_

#include <ZMessageBox.h>
#include <qthread.h>
#include <qfile.h>
#include <qtextstream.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>

#include "dapplication.h"
#include "screenlockcanvas.h"

/* added for backlight control */
#define FBIOSETBKLIGHT          0x4619
#define FBIOGETBKLIGHT          0x461A
#define FBIOSETBRIGHTNESS       0x461B 
#define FBIOGETBRIGHTNESS       0x461C 


class ScreenLockEngine : public PointerListener
{
    private:
        ScreenLockCanvas *canvas;
        QCanvasView *view;
        DApplication *DApp;
        bool startup;
        bool keypressed;
        bool hidepressed;
        bool showpressed;
        bool ishide;
        bool req_update;
        bool req_lightoff;
        bool req_LCDnonesleep;
        bool ifautolock;
        bool ifshowinstruction;
        bool ifhide4sms;
        bool ifshowdeskicon;
        int LANGID;

        unsigned char keytimeout;
        unsigned char lightimeout;
        int onSleepTimer;
        bool sleepmode;
        int autolock_interval;
        int lock_light_timeout;
        int lock_brightness;
        int sys_brightness;
        int sys_lcdsleeptime;
        void LoadConfig( );
        void SaveConfig( );
        void showScreenSaver( );
        void hideScreenSaver( );
        void backlightctrl(bool onoff,int brightness = 10);
        void keylightctrl(bool onoff);
        int backlightstatus( );
        void autolock(bool ctrl = true); //screen auto lock
        void getSysDefine( ); //get system defined brightness and lcd sleep time
        void setlcdsleeptime(int second);

    public:
        ScreenLockEngine( ScreenLockCanvas *canvas_ )
        {
            canvas = canvas_;
        }
        void initial( );
        void beforeterminate( );
        void checkprocess( );
        void setview(QCanvasView *canvasview,DApplication *app);
        void pointerPressed( int x, int y );
        void keyPressed(int keycode);
        void QCopReceived(int message);
        void PropertyReceived( );
};

static const int lcdsleeptime_ref[] = {
    30,60,60*5,60*10,
};
#endif

