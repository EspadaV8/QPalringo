/***************************************************************************
 *   Copyright (C) 2008 by Andrew Smith                                    *
 *   espadav8@gmail.com                                                    *
 *                                                                         *
 *   This file is part of QPalringo                                        *
 *                                                                         *
 *  QPalringo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as         *
 *  published by the Free Software Foundation, either version 3 of the     *
 *  License, or (at your option) any later version.                        *
 *                                                                         *
 *  QPalringo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU Lesser General Public License for more details.                    *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with QPalringo. If not, see                              *
 *  <http://www.gnu.org/licenses/>                                         *
 *                                                                         *
 ***************************************************************************/
#include <QApplication>
#include <QFile>
#include "palringowindow.h"

int main ( int argc, char *argv[] )
{
    QApplication app ( argc, argv );
    app.setApplicationName( "QPalringo" );
    QFile sheet ( ":/styles/QPalringo.css" );

    if ( ! sheet.open ( QIODevice::ReadOnly ) )
    {
        qCritical( "failed to read the stylesheet resource: %s", qPrintable( sheet.fileName() ) );
        exit ( EXIT_FAILURE );
    }

    app.setStyleSheet ( sheet.readAll() );
    
    QApplication::setOrganizationName( "Palringo" );
    QApplication::setOrganizationDomain( "palringo.com" );
    QApplication::setApplicationName( "QPalringo" );

    PalringoWindow *mw = new PalringoWindow();

    mw->show();
    return app.exec();
}

