
#include <qcanvas.h>

#include "dgenius.h"
#include "dgeniuscanvas.h"
#include "dapplication.h"
#include "dgeniusengine.h"
#include <qtextcodec.h>


int main( int argc, char **argv )
{
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

