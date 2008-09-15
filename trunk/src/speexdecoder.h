//
// C++ Interface: speexdecoder
//
// Description:
//
//
// Author: Andrew Smith <espadav8@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SPEEXDECODER_H
#define SPEEXDECODER_H

#include <QObject>
#include <QQueue>
#include <speex/speex.h>

/**
	@author Andrew Smith <espadav8@gmail.com>
*/
class SpeexDecoder : public QObject
{
    Q_OBJECT
    public:
        SpeexDecoder(QObject *parent = 0);
        ~SpeexDecoder();

        QByteArray decode( char* data, int size );

    private:
        SpeexBits       bits;
        void*           dec_state;
        long            position;
        int             frameSize;

};

#endif
