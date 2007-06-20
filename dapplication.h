
#ifndef _DAPPLICATION_H_
#define _DAPPLICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <ZApplication.h>
#include <ZMainWidget.h>
#include <unistd.h>
#include <qpushbutton.h>
#include <ZMultiLineEdit.h>
#include <ZScrollView.h>
#include <ZMessageBox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qcanvas.h>
#include <qthread.h>
#include <ZPushButton.h>
#include <qvbox.h>
#include <pthread.h>
#include <iostream>


class PointerListener
{
    public:
        PointerListener() {}
        virtual void pointerPressed( int x, int y) = 0;
        virtual void pointerDragged( int x, int y) = 0;
        virtual void pointerReleased( int x, int y) = 0;
	 virtual void keyPressed(int keycode) = 0;
     virtual void showScreenSaver() = 0;
};

class DApplication : public ZApplication {

    PointerListener *pointerListener;
    bool press;

    public:
        DApplication(int argc, char** argv):
        ZApplication(argc, argv)
        {
            pointerListener = 0;
        }
        virtual ~DApplication() {}
        void insertStr(QWSEvent*) {}
        virtual bool qwsEventFilter(QWSEvent* event) {
            ZApplication::qwsEventFilter(event);
            QWSMouseEvent* mouse=0;
            QWSKeyEvent* KeyEvent =0;
            QWSFocusEvent* focus;
            QWSPropertyNotifyEvent *propertyNotify;
            int x,y,z;
            if (!event)
                return false;
                
            switch(event->type) {
                case QWSEvent::NoEvent:
                    printf("no event\n");
                break;

                case QWSEvent::Connected:
                    printf("connected\n");
                break;
                
                case QWSEvent::Mouse:
                    printf("mouse\n");
                    mouse=event->asMouse();
                    if (mouse==0)
                        return false;
                    
                    /*
                    printf("mouse move at x=%d,y=%d,state=%d\n,
                        mouse->simpleData.x_root,
                        mouse->simpleData.y_root,
                        mouse->simpleData.state);
                     */
                     
                     x=mouse->simpleData.x_root;
                     y=mouse->simpleData.y_root;
                     z=mouse->simpleData.state;
                     
                     /*printf("mouse move at x=%d,y=%d,state=%d\n",
                     x,
                     y,
                     z);*/
                     printf("");
                     
                     if ( pointerListener != 0 )
                     {
                        switch( z )
                        {
                            case 0:
                                pointerListener->pointerReleased( x, y );
                            break;
                            case 1:
                                if ( not press )
                                    pointerListener->pointerPressed( x, y );
                            break;
                        }
                        
                        press = z;
                     }
                break;
                 
                case QWSEvent::Focus:
                    focus=(QWSFocusEvent*)event;
                    printf("focus %d\n",focus->simpleData.get_focus);
                    if (focus->simpleData.get_focus==0) {
                    /*
                        if (ignore == 0) {
                            ignore = 1;
                            pthread_create( &thr_event_handler, NULL, event_handler, (void*)KeyEvent);
                        }
                        */
                    }
                break;
                
                case QWSEvent::Key:
                
                    /*
                	int window;
                	ushort unicode;
                	ushort keycode;
                	int modifiers;
                	uint is_press:1;
                	uint is_auto_repeat:1;
                	*/
                
                    KeyEvent = (QWSKeyEvent*) event;
                    if (KeyEvent==0)
                        return 0;
                        
                    printf("key %d %d %d\n",
                        KeyEvent->simpleData.keycode,
                        KeyEvent->simpleData.is_press,
                        KeyEvent->simpleData.is_auto_repeat);
                    pointerListener->keyPressed(KeyEvent->simpleData.keycode);
                    //ZMessageBox::timerMessage(0, NULL,"teste",4);
                break;
                
                case QWSEvent::RegionModified:
                    printf("RegionModified\n");
                break;
                
                case QWSEvent::Creation:
                    printf("Creation\n");
                break;
                
                case QWSEvent::PropertyNotify:
                    printf("PropertyNotify\n");
                    if (propertyNotify==0)
                        return 0;
                        
                    printf("propertyNotify %d %d %d\n",
                        propertyNotify->simpleData.window,
                        propertyNotify->simpleData.property,
                        propertyNotify->simpleData.state);
                break;
                
                case QWSEvent::PropertyReply:
                    printf("PropertyReply\n");
                break;
                
                case QWSEvent::SelectionClear:
                    printf("SelectionClear\n");
                break;
                
                case QWSEvent::SelectionRequest:
                    printf("SelectionRequest\n");
                break;
                
                case QWSEvent::SelectionNotify:
                    printf("SelectionNotify\n");
                break;
                
                case QWSEvent::MaxWindowRect:
                    printf("MaxWindowRect\n");
                break;
                
                case QWSEvent::QCopMessage:
                    pointerListener->showScreenSaver();
                    printf("QCopMessage\n");
                break;
                
                case QWSEvent::WindowOperation:
                    printf("WindowOperation\n");
                break;
                
                case QWSEvent::IMEvent:
                    printf("IMEvent\n");
                break;
                
                case QWSEvent::NEvent:
                    printf("NEvent\n");
                break;
           }
           return false;
      }
      
      void setPointerListener( PointerListener *listener )
      {
        pointerListener = listener;
      }
      
 };


#endif
