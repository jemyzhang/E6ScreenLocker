#include "dapplication.h"

void DApplication :: loopTimer(int seconds)
{
    struct itimerval value;
    value.it_value.tv_sec = seconds;
    value.it_value.tv_usec = 0;
    value.it_interval = value.it_value;
    setitimer(ITIMER_REAL, &value,NULL);
    if (0 == seconds) {
        dbg_printf("went to sleep mode...\n");
    }else
    {
        dbg_printf("wakeup,timer set to %d seconds...\n",seconds);
    }
}

bool DApplication :: LoadConfig(const QString& configfile,const char *name,char* value )
{
    dbg_printf("Load %s configuration:",name);

    QFile file(configfile);
    if(file.exists () && file.open( IO_ReadOnly )) {
        QString line;
        QTextStream stream(&file);
        char tmp[256];
        while (!stream.eof()) {
            line = stream.readLine();
            if (line.contains (name)) {
                sscanf(line,"%s = %s",tmp,value);
                dbg_printf("%s\n",value);
                file.close();
                return true;
            }
        }
        file.close();
    }
    dbg_printf("\n");
    return false;
}


bool DApplication :: LoadAppConfig(const char *name,char* value )
{
    QString configfile;
    configfile = "screenlocker.cfg";
    return LoadConfig(configfile,name,value);
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

    dbg_printf("Save configuration[%s]\n",name);
    return true;
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
             dbg_printf("no event\n");
         break;

         case QWSEvent::Connected:
             dbg_printf("connected\n");
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
                         //pointerListener->pointerReleased( x, y );
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
             dbg_printf("key %d %d %d\n",
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
             dbg_printf("RegionModified\n");
         break;
         
         case QWSEvent::Creation:
             dbg_printf("Creation\n");
         break;
         
         case QWSEvent::PropertyNotify:
             if (propertyNotify==0)
                 return 0;
             pointerListener->PropertyReceived();
/*                
             dbg_printf("propertyNotify %d %d %d\n",
                 propertyNotify->simpleData.window,
                 propertyNotify->simpleData.property,
                 propertyNotify->simpleData.state);
*/ 
         break;
         
         case QWSEvent::PropertyReply:
             dbg_printf("PropertyReply\n");
         break;
         
         case QWSEvent::SelectionClear:
             dbg_printf("SelectionClear\n");
         break;
         
         case QWSEvent::SelectionRequest:
             dbg_printf("SelectionRequest\n");
         break;
         
         case QWSEvent::SelectionNotify:
             dbg_printf("SelectionNotify\n");
         break;
         
         case QWSEvent::MaxWindowRect:
             dbg_printf("MaxWindowRect\n");
         break;
         
         case QWSEvent::QCopMessage:
             QCop = (QWSQCopMessageEvent *)event;
             dbg_printf("QCopMessage %d %d %d\n",
                 QCop->simpleData.lchannel,
                 QCop->simpleData.ldata,
                 QCop->simpleData.lmessage);
             pointerListener->QCopReceived(QCop->simpleData.lmessage);
             if(QCop->simpleData.lmessage == 6) {
                 return true;
             }
         break;
         
         case QWSEvent::WindowOperation:
             dbg_printf("WindowOperation\n");
         break;
         
         case QWSEvent::IMEvent:
             dbg_printf("IMEvent\n");
         break;
         
         case QWSEvent::NEvent:
             dbg_printf("NEvent\n");
         break;
    }
    return false;
}

