
#include <qcanvas.h>

#include "screenlock.h"
#include "screenlockcanvas.h"
#include "dapplication.h"
#include "screenlockengine.h"
#include <qtextcodec.h>

#include <signal.h>

ScreenLockEngine *engine;

void mainloop(int signo)
{
    if (SIGALRM == signo) {
        engine->checkprocess();
        signal(SIGALRM,mainloop);
    }
}

void killhandle(int signo)
{
    switch(signo) {
    case SIGTERM:
        printf("catch signal: SIGTERM\n");
        break;
    case SIGINT:
        printf("catch signal: SIGTERM\n");
        break;
    default:
        printf("catch signal: UNKNOWN[%d]\n",signo);
        return;
    }
    engine->beforeterminate();
    exit(0);
}

int main( int argc, char **argv )
{
    printf("ScreenLocker ver 1.0(20070704-01.2) by Jemyzhang\n");
    DApplication app( argc, argv );
    ScreenLockCanvas *canvas = new ScreenLockCanvas( 236, 316 );
    QCanvasView *view = new QCanvasView( canvas );
    engine = new ScreenLockEngine( canvas );
    view->showFullScreen( );
    view->show( );
    //app.setMainWidget( view );
    engine->setview(view,&app);
    engine->initial( );
    app.setPointerListener( (PointerListener*) engine );

    signal(SIGALRM,mainloop);
    signal(SIGTERM,killhandle);
    signal(SIGINT,killhandle);
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0; 
    setitimer(ITIMER_REAL, &value,NULL);

    return app.exec( );
}

