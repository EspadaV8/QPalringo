//
// C++ Interface: palringoservice
//
// Description: 
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PALRINGOSERVICE_H
#define PALRINGOSERVICE_H

#include <service.h>

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class PalringoService : public Service
{
    Q_OBJECT
    public:
        PalringoService();
        ~PalringoService();
        
    signals:
        virtual void serviceUpdated();
        
    private:
        virtual void loggedOn();

};

#endif
