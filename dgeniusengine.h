
#ifndef _DGENIUSENGINE_H_
#define _DGENIUSENGINE_H_

#include <qthread.h>
#include "dapplication.h"
#include "dgeniuscanvas.h"


class DGeniusEngine : public QThread, PointerListener
{
    private:
        DGeniusCanvas *canvas;
        QCanvasView *view;
        bool keypressed;
        bool hidepressed;
        bool showpressed;
        bool ishide;
        unsigned char timecnt;
        unsigned char timeout;
        void showScreenSaver();
        void hideScreenSaver();

    public:
        DGeniusEngine( DGeniusCanvas *canvas_ )
        {
            canvas = canvas_;
            keypressed = false;
            hidepressed = false;
            showpressed = false;
            ishide = false;
            timecnt = 0;
        }
        
        virtual void run( );
        void setview(QCanvasView *canvasview);
        void pointerPressed( int x, int y );
        void pointerDragged( int x, int y );
        void pointerReleased( int x, int y );
        void keyPressed(int keycode);
        void QcopAutoLock( );
};


#endif

