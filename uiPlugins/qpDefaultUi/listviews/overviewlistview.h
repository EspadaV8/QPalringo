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
#ifndef OVERVIEWLISTVIEW_H
#define OVERVIEWLISTVIEW_H

#include <QList>
#include "services/service.h"
#include "message.h"
#include "services/bridge.h"
#include "palringolistview.h"
#include "targets/target.h"
#include "signinwindow.h"
#include "tools.h"

/**
        @author Andrew Smith <espadav8@gmail.com>
*/
class OverviewListView : public PalringoListView
{
    Q_OBJECT
    public:
        OverviewListView(QWidget *parent = 0, Tools* tools_ = 0 );
        ~OverviewListView();

        virtual void setupContainers();
        void newPendingMessage( Target* target );

    signals:
        void signinPalringo( QString emailAddress, QString password );

    private:
        QList<Target*> knownTargets;
        SigninWindow* signinWindow;

    private slots:
        void serviceReceived( Service *service );
        void handleServiceDoubleClick( Service* service );
};

#endif
