
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

int main( int argc, char **argv )
{
    printf("ScreenLocker ver.1.0beta(20070630-01.6) by Jemyzhang\n");
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
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0; 
    setitimer(ITIMER_REAL, &value,NULL);

    return app.exec( );
}

