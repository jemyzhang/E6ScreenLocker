
#include <ZMessageBox.h>

#include "dgeniusengine.h"
#include "dapplication.h"
#include <iostream>
#include <time.h>
#include <stdio.h>

void DGeniusEngine :: run( )
{
    //std::cout << "DGeniusEngine running...\n";
    running = true;
    
    canvas->update( );
    while(1)
    {
        sleep(1);

        struct tm *tm_ptr;
        time_t now;
        time(&now); 
        tm_ptr = localtime(&now);

			//check timeout
        if (timeout > 10 && true == ishide) {
            printf("timeout,auto lock\n");
            timeout = 0;
				view->show();
            ishide = false;
        }
        if (ishide) {
	        timeout ++;
            view->hide();
        }else
        {
				timeout = 0;
            view->show();
        }
        if( tm_ptr->tm_sec == 0 && false == ishide)
        {
            char buf[11];
            char tmp;
            tmp = tm_ptr->tm_hour;// + 8;
            buf[4] = 0;
            if(tmp > 12){
                tmp = tmp - 12;
                buf[4] = 1;
            }
            buf[0] = tmp / 10;
            buf[1] = tmp % 10;
            tmp = tm_ptr->tm_min;
            buf[2] = tmp / 10;
            buf[3] = tmp % 10;
            tmp = tm_ptr->tm_year % 100;
            buf[5] = tmp / 10;
            buf[6] = tmp % 10;
            tmp = tm_ptr->tm_mon + 1;
            buf[7] = tmp / 10;
            buf[8] = tmp % 10;
            tmp = tm_ptr->tm_mday;
            buf[9] = tmp / 10;
            buf[10] = tmp % 10;
            buf[11] = tm_ptr->tm_wday;
            std::cout<<"Ready for output date "<< ctime(&now) <<std::endl;
           canvas->setScreenSprite(buf);
            canvas->update();
            std::cout << "canvas updated" << std::endl;

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
    keypressed = true ;
    if (false == ishide) {
    	if(4144 == keycode)
    	{
            std::cout << "Fire Key Pressed " << std::endl;
            hidepressed = true ;
            canvas->showString("\n  Unlock: Now Press Center Key.");
            canvas->showSpriteLock();
            canvas->update();
    	}else if( 4100 == keycode && true == hidepressed )
        {
            hidepressed = true ;
            canvas->hideString();
            canvas->hideSpriteLock();
           view->hide();
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
    /*
    canvas->hideString();
    showpressed = false;
    view->show();
    ishide = false;
    */
}
