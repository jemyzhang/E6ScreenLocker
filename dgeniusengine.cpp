
#include <ZMessageBox.h>

#include "dgeniusengine.h"
#include "dapplication.h"
#include <iostream>

void DGeniusEngine :: run( )
{
    //std::cout << "DGeniusEngine running...\n";
    running = true;
    
    canvas->update( );
    while(1)
    {
        sleep(1);
        timecnt++;
        if( ( timecnt % 2 ) == 0)
        {
        hidepressed = false;
        showpressed = false;
        canvas->hideString();
        canvas->update();
        }
        
    }
    running = false;
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
    if (false == ishide) {
    	if(4144 == keycode)
    	{
            std::cout << "Fire Key Pressed " << std::endl;
            hidepressed = true ;
            canvas->showString("\n  Unlock: Now Press Center Key.");
            canvas->update();
    	}else if( 4100 == keycode && true == hidepressed )
        {
            hidepressed = true ;
            canvas->hideString();
           view->hide();
           ishide = true;
        }else
        {
            hidepressed = false;
            canvas->showString("\n  Unlock: First Press Green Key, \n   Then Press Center Key.");
            canvas->update();
        }
    }else
    {
        if (4171 == keycode) {
            showpressed = ~showpressed;
        }else if(4172 == keycode && true == showpressed)
        {
            showpressed = false;
            view->show();
            ishide = false;
        }else
        {
            showpressed = false;
        }
    }
}

void DGeniusEngine :: setview(QCanvasView *canvasview)
{
    view = canvasview;
}

void DGeniusEngine :: showScreenSaver()
{
    canvas->hideString();
    showpressed = false;
    view->show();
    ishide = false;
}
