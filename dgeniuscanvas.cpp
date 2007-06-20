
#include <qcanvas.h>
#include <iostream>
#include "dgeniuscanvas.h"

// 2126-8423 - Ricardo


void DGeniusCanvas :: init( )
{
}


void DGeniusCanvas :: showBackground( )
{
    background_sprite->show( );
}

void DGeniusCanvas :: showSpriteR( )
{
    //std::cout << "DGeniusCanvas::showSpriteR( )\n";
    R_sprite->show( );
}

void DGeniusCanvas :: showSpriteL( )
{
    //std::cout << "DGeniusCanvas::showSpriteL( )\n";
    L_sprite->show( );
}

void DGeniusCanvas :: showSpriteU( )
{
    //std::cout << "DGeniusCanvas::showSpriteU( )\n";
    U_sprite->show( );
}

void DGeniusCanvas :: showSpriteD( )
{
    //std::cout << "DGeniusCanvas::showSpriteD( )\n";
    D_sprite->show( );
}

void DGeniusCanvas :: hideSpriteR( )
{
    //std::cout << "DGeniusCanvas::hideSpriteR( )\n";
    R_sprite->hide( );
}

void DGeniusCanvas :: hideSpriteL( )
{
    //std::cout << "DGeniusCanvas::hideSpriteL( )\n";
    L_sprite->hide( );
}

void DGeniusCanvas :: hideSpriteU( )
{
    //std::cout << "DGeniusCanvas::hideSpriteU( )\n";
    U_sprite->hide( );
}

void DGeniusCanvas :: hideSpriteD( )
{
    //std::cout << "DGeniusCanvas::hideSpriteD( )\n";
    D_sprite->hide( );
}

void DGeniusCanvas :: showString( const QString& info)
{
    presskeyMsg->setText(info);
	presskeyMsg->show( );
}

void DGeniusCanvas :: hideString( )
{
	presskeyMsg->hide();
}
/*
void DGeniusCanvas :: advance()
{
    std::cout<<"period output"<<std::endl;
}
*/
