#include <qstring.h>
#include <qtextstream.h>
#include <qfile.h>
#include "dapplication.h"

bool DApplication :: LoadAppConfig(char *name,char* value )
{
    QString configfile;
    configfile = "screenlocker.cfg";
    printf("Load %s configuration:",name);

    QFile file(configfile);
    if(file.exists () && file.open( IO_ReadOnly )) {
        QString line;
        QTextStream stream(&file);
        char tmp[256];
        while (!stream.eof()) {
            line = stream.readLine();
            if (line.contains (name)) {
                sscanf(line,"%s = %s",tmp,value);
                printf("%s\n",value);
                file.close();
                return true;
            }
        }
        file.close();
    }
    printf("\n");
    return false;
}

bool DApplication :: SaveAppConfig(const char *name,const char *value )
{
    QString configfile;
    configfile = "screenlocker.cfg";

    QString contents = "";
    QFile file(configfile);
    bool isexisted = false;
    if(file.exists() && file.open(IO_ReadOnly))
    {
        QTextStream stream(&file);
        QString line;
        while (!stream.eof()) {
            line = stream.readLine();
            if (line == "\n") {
                continue;
            }
            if (line.contains(name)) {
                contents = contents.append(name);
                contents = contents.append(" = ");
                contents = contents.append(value);
                contents = contents.append("\n");
                isexisted = true;
            }else
            {
                contents = contents.append(line);
                contents = contents.append('\n');
            }
        }
        if (!isexisted) {
            contents = contents.append(name);
            contents = contents.append(" = ");
            contents = contents.append(value);
            contents = contents.append("\n");
        }
        file.close();
    }else
    {
        contents = contents.append(name);
        contents = contents.append(" = ");
        contents = contents.append(value);
        contents = contents.append("\n");
    }

       QFile file2(configfile);
       if (file2.open(IO_WriteOnly)) {
           QTextStream stream(&file2);
           stream << contents.latin1();
           file2.close();
       }

    printf("Save configuration[%s]\n",name);
}

bool DApplication :: qwsEventFilter(QWSEvent *event)
 {
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
               focus=(QWSFocusEvent*)event;
               if (focus->simpleData.get_focus==0) {
               }
         break;
         
         case QWSEvent::Key:
         
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
