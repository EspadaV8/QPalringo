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
#include <QtGui>
#include "message.h"
#include "services/service.h"
#include "targets/target.h"

namespace QPTools
{
    QString tagURLs( QString text );
    QString formatMessageText( QByteArray messagePayload );
    QDateTime getMessageTimestamp( Message message );
    QPixmap getPixmap( QString iconFilename, quint32 size = 0 );
    QString getServiceIcon( Service* service );
    QString getTargetIcon( Target* target );

    Message createImageMessage( QString filename );
    Message createTextMessage( QString text );
    Message createMessageOutline();
}
