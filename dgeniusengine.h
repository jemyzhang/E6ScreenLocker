
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
        bool running;
        bool hidepressed;
        bool showpressed;
        bool ishide;
        unsigned char timecnt;

    public:
        DGeniusEngine( DGeniusCanvas *canvas_ )
        {
            canvas = canvas_;
            running = false;
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
    void showScreenSaver();
};


#endif

