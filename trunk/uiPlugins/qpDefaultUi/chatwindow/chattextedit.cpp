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
#include "chattextedit.h"

ChatTextEdit::ChatTextEdit( QWidget *parent )
    : QPlainTextEdit ( parent )
{
    QFont font = this->font();
    font.setPixelSize( 10 );
    this->setFont( font );

    this->setMinimumHeight( this->fontMetrics().height() * 4 );
    this->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum );
}

void ChatTextEdit::keyPressEvent( QKeyEvent* event )
{
#ifdef Q_WS_HILDON
    if( ( event->modifiers() == Qt::KeypadModifier ) && ( event->key() == Qt::Key_Enter ) )
#else
    if( ( event->modifiers() == Qt::NoModifier ) && ( event->key() == Qt::Key_Return  ) )
#endif
    {
        emit returnPressed();
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

/*
  Enter         = Enter +   0x20000000 ( Qt::KeypadModifier )
  Shift-Enter   =           0x02200000
*/
