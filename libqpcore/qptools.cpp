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
#include "qptools.h"

namespace QPTools
{
    /*
     Taken from Konversation
     Copyright (C) 2004 Peter Simonsson <psn@linux.se>
     Copyright (C) 2006-2008 Eike Hein <hein@kde.org>

     Modified by Andrew Smith for Qt4
     Copyright (C) 2008 Andrew Smith <espadav8@gmail.com>
    */
    QString tagURLs( QString text )
    {
        QRegExp urlPattern("((www\\.(?!\\.)|((f|sf|ht)tp(|s))://)(\\.?[\\d\\w/,\\':~\\?=;#@\\-\\+\\%\\*\\{\\}\\!\\(\\)]|&)+)|([-.\\d\\w]+@[-.\\d\\w]{2,}\\.[\\w]{2,})", Qt::CaseInsensitive, QRegExp::RegExp);
        QString link = "<a href=\"%1%2\">%3</a>";
        QString filteredLine = text;
        QString insertText;
        QString href;
        QString protocol;
        int pos = 0;
        int urlLen = 0;

        while((pos = urlPattern.indexIn(filteredLine, pos)) >= 0)
        {
            QString append;

            // check if the matched text is already replaced as a channel
            if ( filteredLine.lastIndexOf( "<a", pos ) > filteredLine.lastIndexOf( "</a>", pos ) )
            {
                ++pos;
                continue;
            }

            protocol = "";
            urlLen = urlPattern.matchedLength();
            href = filteredLine.mid( pos, urlLen );

            // Don't consider trailing comma part of link.
            if (href.right(1) == ",")
            {
                href.truncate(href.length()-1);
                append = ',';
            }

            // Don't consider trailing semicolon part of link.
            if (href.right(1) == ";")
            {
                href.truncate(href.length()-1);
                append = ';';
            }

            // Don't consider trailing closing parenthesis part of link when
            // there's an opening parenthesis preceding the beginning of the
            // URL or there is no opening parenthesis in the URL at all.
            if (href.right(1) == ")" && (filteredLine.mid(pos-1,1) == "(" || !href.contains("(")))
            {
                href.truncate(href.length()-1);
                append.prepend(")");
            }

            // Qt doesn't support (?<=pattern) so we do it here
            if((pos > 0) && filteredLine[pos-1].isLetterOrNumber())
            {
                pos++;
                continue;
            }

            if (urlPattern.cap(1).startsWith("www.", Qt::CaseInsensitive))
            {
                protocol = "http://";
            }
            else if (urlPattern.cap(1).isEmpty())
            {
                protocol = "mailto:";
            }

            // Use \x0b as a placeholder for & so we can readd them after changing all & in the normal text to &amp;
            // HACK Replace % with \x03 in the url to keep Qt from doing stupid things
            //insertText = link.arg(protocol, QString(href).replace('&', "\x0b").replace('%', "\x03"), href) + append;
            insertText = link.arg(protocol, QString(href).replace('%', "\x03"), href) + append;
            filteredLine.replace(pos, urlLen, insertText);
            pos += insertText.length();
        }

        // Change & to &amp; to prevent html entities to do strange things to the text
        //filteredLine.replace('&', "&amp;");
        //filteredLine.replace("\x0b", "&");

        return filteredLine;
    }

    QString formatMessageText( QByteArray messagePayload )
    {
        QString message = QString::fromUtf8( messagePayload );

        message = Qt::escape( message );
        if( message.startsWith( "/me ") )
        {
            message.remove(0,4); // remove "/me "
            message.prepend("<span style=\"color:red;\">* ").append(" *</span>"); //add a coloured span
        }

        message.replace(QRegExp("(\r\n)|(\n)|(\r)"), "<br />");
        message = QPTools::tagURLs( message );

        return message;
    }

    QDateTime getMessageTimestamp( Message message )
    {
        return QDateTime::fromTime_t( message.seconds() );
    }

    QPixmap getPixmap( QString iconFilename, quint32 size )
    {
        QPixmap p;
        QString key = iconFilename + '_' + QString::number( size );
        if( !QPixmapCache::find( key, p ) )
        {
            p.load( iconFilename );
            if( size != 0 )
            {
                p = p.scaledToWidth( size, Qt::SmoothTransformation );
            }
            QPixmapCache::insert( key, p );
        }
        return p;
    }
}
