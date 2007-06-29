
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

        static bool LoadAppConfig(char *name,char *value );
        static bool SaveAppConfig(const char *name,const char *value );

        virtual bool qwsEventFilter(QWSEvent* event);
     
      void setPointerListener( PointerListener *listener )
      {
        pointerListener = listener;
      }
      
 };

#endif
