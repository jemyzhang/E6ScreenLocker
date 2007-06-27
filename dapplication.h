
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
        virtual void QCopReceived(int message) = 0;
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
        virtual ~DApplication() {printf("exit application\n");}
        void insertStr(QWSEvent*) {}
        virtual bool qwsEventFilter(QWSEvent* event) {
            ZApplication::qwsEventFilter(event);
            QWSMouseEvent* mouse=0;
            QWSKeyEvent* KeyEvent =0;
            QWSFocusEvent* focus;
            QWSQCopMessageEvent *QCop;
            QWSPropertyNotifyEvent *propertyNotify;
            
            int x,y,z;
            int kcode;
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
                    mouse=event->asMouse();
                    if (mouse==0)
                        return false;
                     x=mouse->simpleData.x_root;
                     y=mouse->simpleData.y_root;
                     z=mouse->simpleData.state;
                     
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
/*                    focus=(QWSFocusEvent*)event;
                    if (focus->simpleData.get_focus==0) {
                    }
*/                break;
                
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
                    kcode = KeyEvent->simpleData.keycode;
                    if(kcode == 4118 || kcode == 4119 || kcode == 4169 || kcode == 4171 || kcode == 4172) {
                        //
                    }

                    if (KeyEvent->simpleData.is_press && !KeyEvent->simpleData.is_auto_repeat) {
                        pointerListener->keyPressed(KeyEvent->simpleData.keycode);
                    }
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
                    QCop = (QWSQCopMessageEvent *)event;
                    printf("QCopMessage %d %d %d\n",
                        QCop->simpleData.lchannel,
                        QCop->simpleData.ldata,
                        QCop->simpleData.lmessage);
                    pointerListener->QCopReceived(QCop->simpleData.lmessage);
                    if(QCop->simpleData.lmessage == 6) {
                        return true;
                    }
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
