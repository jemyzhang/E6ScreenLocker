
#include <qcanvas.h>

#include "screenlock.h"
#include "screenlockcanvas.h"
#include "dapplication.h"
#include "screenlockengine.h"
#include <qtextcodec.h>


int main( int argc, char **argv )
{
    printf("ScreenLocker ver.1.0beta(20070629-01.12) by Jemyzhang\n");
    DApplication app( argc, argv );
    ScreenLockCanvas *canvas = new ScreenLockCanvas( 236, 316 );
    QCanvasView *view = new QCanvasView( canvas );
    ScreenLockEngine *engine = new ScreenLockEngine( canvas );
    view->showFullScreen( );
    view->show( );
    //app.setMainWidget( view );
    engine->setview(view,&app);
    if(argc > 1){
        engine->setAutolockInterval(atoi(argv[1]));
    }else
    {
        engine->setAutolockInterval(20);    //default 30s auto lock
    }
    engine->start( );
    app.setPointerListener( (PointerListener*) engine );

    return app.exec( );
}

