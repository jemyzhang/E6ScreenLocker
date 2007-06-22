
#include <qcanvas.h>

#include "dgenius.h"
#include "dgeniuscanvas.h"
#include "dapplication.h"
#include "dgeniusengine.h"
#include <qtextcodec.h>


int main( int argc, char **argv )
{
    printf("ScreenLocker ver.1.0beta(20070622-014.13) by Jemyzhang\n");
    DApplication app( argc, argv );
    DGeniusCanvas *canvas = new DGeniusCanvas( 236, 316 );
    QCanvasView *view = new QCanvasView( canvas );
    DGeniusEngine *engine = new DGeniusEngine( canvas );
    view->showFullScreen( );
    view->show( );  
    //app.setMainWidget( view );
    engine->setview(view);
    engine->start( );
    app.setPointerListener( (PointerListener*) engine );

    return app.exec( );
}

