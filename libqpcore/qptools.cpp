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
#include "tools.h"
#include "targets/contact.h"
#include "targets/bridgecontact.h"
#include "services/bridge.h"

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

    QString getServiceIcon( Service* service )
    {
        if( service->getOnlineStatus() == qpOnlineStatus::OFFLINE )
        {
            switch( service->getType() )
            {
                case qpBridgeType::PALRINGO:
                    return ":/svg/palringoService.svg";
                case qpBridgeType::MSN:
                    return ":/services/msn_offline.svg";
                case qpBridgeType::YAHOO:
                    return ":/services/yahoo_offline.svg";
                case qpBridgeType::AIM:
                    return ":/services/aim_offline.svg";
                case qpBridgeType::GADUGADU:
                    return ":/services/gaduGadu_offline.svg";
                case qpBridgeType::ICQ:
                    return ":/services/icq_offline.svg";
                case qpBridgeType::XMPP:
                    return ":/services/jabber_offline.svg";
                case qpBridgeType::QQ:
                    return ":/services/qq_offline.svg";
                case qpBridgeType::FACEBOOK:
                    return ":/services/facebook_offline.svg";
                default:
                    return ":/svg/logo_offline.svg";
            }
        }
        else
        {
            switch( service->getType() )
            {
                case qpBridgeType::PALRINGO:
                    return ":/svg/palringoService.svg";
                case qpBridgeType::MSN:
                    return ":/services/msn.svg";
                case qpBridgeType::YAHOO:
                    return ":/services/yahoo.svg";
                case qpBridgeType::AIM:
                    return ":/services/aim.svg";
                case qpBridgeType::GADUGADU:
                    return ":/services/gaduGadu.svg";
                case qpBridgeType::ICQ:
                    return ":/services/icq.svg";
                case qpBridgeType::XMPP:
                    return ":/services/jabber.svg";
                case qpBridgeType::QQ:
                    return ":/services/qq.svg";
                case qpBridgeType::FACEBOOK:
                    return ":/services/facebook.svg";
                default:
                    return ":/svg/logo.svg";
            }
        }
    }

    QString getTargetIcon( Target* target )
    {
        if( target->getType() == Target::GROUP )
        {
            return ":/svg/group.svg";
        }
        else if( target->getType() == Target::CONTACT )
        {
            Contact* contact = qobject_cast<Contact*>(target);
            if( contact )
            {
                QString iconName;
                if ( contact->getOnlineStatus() != qpOnlineStatus::OFFLINE )
                {
                    switch( contact->getDeviceType() )
                    {
                        //case 1:
                        //    iconName = ":/svg/botContact.svg";
                        //    break;
                        case 2:
                            iconName = ":/svg/pcContact.svg";
                            break;
                        //case 3:
                        //    iconName = ":/svg/mobileContact.svg";
                        //    break;
                        case 4:
                            iconName = ":/svg/macContact.svg";
                            break;
                        case 5:
                            iconName = ":/svg/iPhoneContact.svg";
                            break;
                        default:
                            iconName = ":/svg/onlineContact.svg";
                            break;
                    }
                }
                else
                {
                    iconName = ":/svg/offlineContact.svg";
                }
                return iconName;
            }
        }
        else if( target->getType() ==  Target::BRIDGECONTACT )
        {
            BridgeContact* contact = qobject_cast<BridgeContact*>(target);
            if( contact )
            {
                QString icon = ":/services/";
                Bridge* b = Tools::getBridge( contact->getBridgeId() );
                if( b != NULL )
                {
                    switch( b->getType() )
                    {
                        case qpBridgeType::AIM:
                            icon += "aim";
                            break;
                        case qpBridgeType::FACEBOOK:
                            icon += "facebook";
                            break;
                        case qpBridgeType::GADUGADU:
                            icon += "gaduGadu";
                            break;
                        case qpBridgeType::ICQ:
                            icon += "icq";
                            break;
                        case qpBridgeType::MSN:
                            icon += "msn";
                            break;
                        case qpBridgeType::QQ:
                            icon += "qq";
                            break;
                        case qpBridgeType::XMPP:
                            icon += "jabber";
                            break;
                        case qpBridgeType::YAHOO:
                            icon += "yahoo";
                            break;
                        default:
                            icon = ":/svg/logo";
                            break;
                    }
                }
                else
                {
                    icon = ":/svg/logo";
                }
                if( contact->getOnlineStatus() == qpOnlineStatus::OFFLINE )
                {
                    icon += "_offline";
                }
                icon += ".svg";
                return icon;
            }
        }

        return ":/svg/logo.svg";
    }


    Message createImageMessage( QString filename )
    {
        Message message = QPTools::createMessageOutline();
        message.setType( "image/jpeg" );

        QImage image( filename );
        if( image.isNull() )
        {
            QBuffer buffer;
            buffer.open( QIODevice::WriteOnly );
            image.save(&buffer, "jpg");
            message.setPayload( buffer.data() );
        }
        return message;
    }

    Message createTextMessage( QString text )
    {
        Message message = QPTools::createMessageOutline();
        message.setType( "text/plain" );
        message.setPayload( text.toUtf8() );

        return message;
    }

    Message createMessageOutline()
    {
        Message message;
        message.setSeconds( QDateTime::currentDateTime().toTime_t() );
        message.setUseconds( 0 );
        message.setHist( false );

        return message;
    }
}
