
#include <ZMessageBox.h>

#include "dgeniusengine.h"
#include "dapplication.h"
#include <iostream>
#include <time.h>
#include <stdio.h>

void DGeniusEngine :: run( )
{
    canvas->update( );
    while(1)
    {
        sleep(1);

        struct tm *tm_ptr;
        time_t now;
        time(&now); 
        tm_ptr = localtime(&now);

        if (!ishide && !(view->isActiveWindow()) ) {    //check if screensaver is forced background
            showScreenSaver();
            view->setActiveWindow();
            printf("force active:%d\n",view->isActiveWindow());
        }else if( ishide && view->isActiveWindow() )
        {
            printf("force inactive\n");
            hideScreenSaver();
        }

        if (ishide) {
//            timeout ++;   //disable auto screen lock, can not receive mouse event while minimized
            //check timeout
            if (timeout > 10 ) {
                printf("timeout,auto lock\n");
                timeout = 0;
                canvas->updateScreenSprite( );
                canvas->update();
                showScreenSaver();
                }
        }else
        {
            timeout = 0;
        }

        if( tm_ptr->tm_sec == 0 && false == ishide)
        {
           canvas->updateScreenSprite( );
           canvas->update();
        }
        if( true == keypressed)
        {
            timecnt ++;
            timeout = 0;
            if ( (timecnt % 2 ) ==0 ){
                hidepressed = false;
                showpressed = false;
                keypressed = false;
                canvas->hideString();
                canvas->hideSpriteLock();
                canvas->update();
            }
        }
        
    }
}

void DGeniusEngine :: pointerPressed( int x, int y )
{
    std::cout << "Pointer pressed on " << x << "," << y << std::endl;
    
}

void DGeniusEngine :: pointerDragged( int x, int y )
{
    std::cout << "Pointer dragged on " << x << "," << y << std::endl;
}

void DGeniusEngine :: pointerReleased( int x, int y )
{
    std::cout << "Pointer released on " << x << "," << y << std::endl;
}

void DGeniusEngine :: keyPressed(int keycode)
{
    keypressed = true ;
    if (false == ishide) {
        if(4144 == keycode)
        {
            hidepressed = true ;
            canvas->showString("\n  Unlock: Now Press Center Key.");
            canvas->showSpriteLock();
            canvas->update();
        }else if( 4100 == keycode && true == hidepressed )
        {
            hidepressed = true ;
            canvas->hideString();
            canvas->hideSpriteLock();
           hideScreenSaver();
           ishide = true;
        }else
        {
            hidepressed = false;
            canvas->showString("\n  Unlock: First Press Green Key, \n   Then Press Center Key.");
            canvas->showSpriteLock();
            canvas->update();
        }
    }else
    {
        if (4171 == keycode) {
            showpressed = ~showpressed;
        }else if(4172 == keycode && true == showpressed)
        {
            showpressed = false;
            canvas->updateScreenSprite( );
            canvas->update();
            showScreenSaver();
        }else
        {
            showpressed = false;
        }
    }
}

void DGeniusEngine :: QcopAutoLock( )
{
    if (ishide) {
        showScreenSaver();
    }
}

void DGeniusEngine :: setview(QCanvasView *canvasview)
{
    view = canvasview;
}

void DGeniusEngine :: showScreenSaver()
{
    printf("canvas show\n");
    view->showFullScreen();
    view->show();
    ishide = false;
}

void DGeniusEngine :: hideScreenSaver()
{
    printf("canvas Minimized\n");
    view->showMinimized();
    ishide = true;
}

