#include "screenlock.h"


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
    case SIGILL:
    case SIGSEGV:
    case SIGABRT:
    case SIGTERM:
        dbg_printf("catch signal: SIGTERM\n");
        break;
    case SIGINT:
        dbg_printf("catch signal: SIGTERM\n");
        break;
    default:
        dbg_printf("catch signal: UNKNOWN[%d]\n",signo);
        return;
    }
    engine->beforeterminate();
    exit(0);
}

int main( int argc, char **argv )
{
    printf("ScreenLocker by Jemyzhang %s %s, %s \n",VERSION,__TIME__,__DATE__);
    DApplication app( argc, argv );
    ScreenLockCanvas *canvas = new ScreenLockCanvas( 236, 316 );
    QCanvasView *view = new QCanvasView( canvas );
    engine = new ScreenLockEngine( canvas );
    view->showFullScreen( );
    view->show( );
    engine->setview(view,&app);
    engine->initial( );
    app.setPointerListener( (PointerListener*) engine );

    signal(SIGALRM,mainloop);
    signal(SIGTERM,killhandle);
    signal(SIGINT,killhandle);
    signal(SIGSEGV,killhandle);
    struct itimerval value;
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0; 
    setitimer(ITIMER_REAL, &value,NULL);

    return app.exec( );
}

