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
#include <QDebug>
#include "speexdecoder.h"

SpeexDecoder::SpeexDecoder(QObject *parent)
 : QObject(parent)
{
    speex_bits_init( &this->bits );
    this->dec_state = speex_decoder_init( &speex_wb_mode );

    speex_decoder_ctl( this->dec_state, SPEEX_GET_FRAME_SIZE, &this->frameSize );

    short *newBuffer = new short[frameSize];

    speex_bits_init_buffer( &this->bits, (char*)newBuffer, this->frameSize*2 );
    //speex_decoder_ctl( this->dec_state, SPEEX_SET_ENH, 0 );
}

QByteArray SpeexDecoder::decode( char* data, int size )
{
    char* decodedData = NULL;
    int decodedDataLength = 0;
    int frameSizeL;

    const unsigned char *ptr;
    const unsigned char *ptrEnd;

    short *outputBuffer = new short[this->frameSize*2];

    // Start from the current position
    ptr = (unsigned char*) data;
    ptrEnd = (unsigned char*) data + size;

    // As long as there's data to read...
    while( ptr + 2 < ptrEnd )
    {
        frameSizeL = *( ptr );
        frameSizeL += *( ptr + 1 ) << 8;

        // Advance the pointer to our data
        ptr += 2;

        // If the entire frame is there, decode.
        if ( ptr + frameSizeL < ptrEnd)
        {
            // Yes it can, allocate buffer and push at the end of the array
            // Decode into that buffer
            speex_bits_read_from( &this->bits, (char*)ptr, frameSizeL );
            speex_decode_int( this->dec_state, &this->bits, outputBuffer );

            decodedData = (char*)realloc( decodedData, decodedDataLength + ( frameSize *2 ) );
            memcpy( decodedData + decodedDataLength, (char*)outputBuffer, ( frameSize *2 ) );
            decodedDataLength += ( frameSize *2 );

            // Move our pointer forward
            ptr += frameSizeL;
        }
        else
        {
            // No more data to decode
            break;
        }
    }

    return QByteArray( decodedData, decodedDataLength );
}


SpeexDecoder::~SpeexDecoder()
{
}


