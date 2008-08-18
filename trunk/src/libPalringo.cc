/*
* libPalringo/libPalringo.cc
* Copyright (c) 2008, Palringo Ltd
*
* Original author: Eric Sellin
* Extended version author: Xabier Eizmendi
*
*
*
* This program is free software (with dual license);
* You can redistribute it and/or modify it under the terms of
* the GNU General Public License version 2 as published by the
* Free Software Foundation and in terms of Palringo Closed Source License
* If you want to use it in a closed source program you must contact
* Palringo Ltd.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Library General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <errno.h>

#include <iostream>
#include <iomanip>

#include "libPalringo.h"


#ifdef DEBUG
#define DBGOUT std::cout
#else
#define DBGOUT if (0) std::cout
#endif

namespace
{
    const std::string whatStrings[] =
    {
        "ADD_CONTACT",
        "SUBSCRIBE_TO_GROUP",
        "CREATE_GROUP",
        "UPDATE_CONTACT",
        "UNSUB_GROUP",
        "UPDATE_PRESENCE",
        "UPDATE_NICKNAME",
        "UPDATE_STATUS_MESG",
        "CONTACT_AUTHORISATION",
        "DEST_QUERY",
        "GROUP_INVITE",
        "MESG",
        "MESG_STORE",
        "STATS_LOGGING",
        "BRIDGING"
    };


    const std::string errorCodes[] =
    {
        "OK",
        "INTERNAL_ERROR",
        "NO_SUCH_WHATEVER",
        "NOT_A_MEMBER",
        "DELIVERED",
        "NOT_DELIVERED",
        "SFE_NOT_AVAILABLE",
        "STATS_IF_NOT_AVAILABLE",
        "END_OF_MESG_STORED",
        "UNABLE_TO_STORE_OFFLINE_MESG",
        "RESEND_CURRENT_MESG",
        "GROUP_ALREADY_EXISTS",
        "CONTACT_ALREADY_EXISTS",
        "NOT_ALLOWED",
        "BRIDGING_NOT_AVAILABLE"
    };
}

PalringoConnection::PalringoConnection ( const std::string& host,
		unsigned int port,
		const std::string& login,
		const std::string& password,
		const std::string& sourceIP ) :
		login_ ( login ), password_ ( password ), host_ ( host ), port_ ( port ), sourceIP_ ( sourceIP ), userId_ ( 0 ),
		outBufferBegin_ ( 0 ), outBufferCurrent_ ( 0 ), outBufferCount_ ( 0 ), sofar_ ( 0 ),
		ghosted_ ( false ), auto_accept_contacts_ ( true ),
		loggedOn_ ( false )
{
    memset ( inBuffer_, 0, sizeof ( inBuffer_ ) );
    memset ( outBuffer_, 0, sizeof ( outBuffer_ ) );

    fd_ = socket ( PF_INET, SOCK_STREAM, 0 );

    if ( fd_ < 0 )
            {
                    DBGOUT << "socket() failed with " << errno << ": "
                    << strerror ( errno ) << std::endl;
                    throw errno;
            }

    int reuseOptVal ( 1 );

    if ( setsockopt ( fd_,
                      SOL_SOCKET,
                      SO_REUSEADDR,
                      static_cast<void*> ( &reuseOptVal ),
                      sizeof ( reuseOptVal ) ) != 0 )
    {
        DBGOUT << "setsockopt() failed with " << errno << ": "
        << strerror ( errno ) << std::endl;
        throw errno;
    }


    if ( sourceIP_.size() > 0 )
    {
        struct sockaddr_in my_addr;
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = 0;

        const struct hostent* hname = gethostbyname ( sourceIP_.c_str() );

        if ( hname == 0 )
        {
            DBGOUT << "gethostbyname() failed with " << errno << ": "
            << strerror ( errno ) << std::endl;
            close ( fd_ );
            throw errno;
        }

        ( void ) memcpy ( &my_addr.sin_addr.s_addr,
                                            hname->h_addr_list [0],
                                            sizeof ( my_addr.sin_addr.s_addr ) );

        if ( bind ( fd_, ( struct sockaddr * ) &my_addr, sizeof ( my_addr ) ) < 0 )
        {
            DBGOUT << "bind() failed with " << errno << ": "
            << strerror ( errno ) << std::endl;
            close ( fd_ );
            throw errno;
        }
    }


    const struct hostent* hp = gethostbyname ( host.c_str() );

    if ( hp == 0 )
    {
        DBGOUT << "gethostbyname() failed with " << errno << ": "
        << strerror ( errno ) << std::endl;
        close ( fd_ );
        throw errno;
    }

    struct sockaddr_in remote_addr;

    remote_addr.sin_family = AF_INET;

    remote_addr.sin_port = htons ( port );

    ( void ) memcpy ( &remote_addr.sin_addr.s_addr,
                                        hp->h_addr_list [0],
                                        sizeof ( remote_addr.sin_addr.s_addr ) );


    if ( connect ( fd_, ( struct sockaddr * ) &remote_addr, sizeof ( remote_addr ) ) < 0 )
    {
        DBGOUT << "connect() failed with " << errno << ": "
        << strerror ( errno ) << std::endl;
        close ( fd_ );
        throw errno;
    }
    DBGOUT << "Object Already Connected" << std::endl;
    headers_t headers;
    headers["Protocol-Version"] = "1.0";
    headers["App-Type"] = "Linux x86";
    headers["Operator"] = "PC_CLIENT";
    sendCmd ( "LOGON", headers, "" );
    
} // end of constructor

void PalringoConnection::reConnect()
{
	memset ( inBuffer_, 0, sizeof ( inBuffer_ ) );
	sofar_ = 0;

	fd_ = socket ( PF_INET, SOCK_STREAM, 0 );

	if ( fd_ < 0 )
		{
			DBGOUT << "socket() failed with " << errno << ": "
			<< strerror ( errno ) << std::endl;
			throw errno;
		}

	if ( sourceIP_.size() > 0 )
		{

			struct sockaddr_in my_addr;
			my_addr.sin_family = AF_INET;
			my_addr.sin_port = 0;

			const struct hostent* hname = gethostbyname ( sourceIP_.c_str() );

			if ( hname == 0 )
				{
					DBGOUT << "gethostbyname() failed with " << errno << ": "
					<< strerror ( errno ) << std::endl;
					close ( fd_ );
					throw errno;
				}

			( void ) memcpy ( &my_addr.sin_addr.s_addr,
							  hname->h_addr_list [0],
							  sizeof ( my_addr.sin_addr.s_addr ) );

			if ( bind ( fd_, ( struct sockaddr * ) &my_addr, sizeof ( my_addr ) ) < 0 )
				{
					DBGOUT << "bind() failed with " << errno << ": "
					<< strerror ( errno ) << std::endl;
					close ( fd_ );
					throw errno;
				}
		}

	const struct hostent* hp = gethostbyname ( host_.c_str() );

	if ( hp == 0 )
		{
			DBGOUT << "gethostbyname() failed with " << errno << ": "
			<< strerror ( errno ) << std::endl;
			close ( fd_ );
			throw errno;
		}

	struct sockaddr_in remote_addr;

	remote_addr.sin_family = AF_INET;

	remote_addr.sin_port = htons ( port_ );

	( void ) memcpy ( &remote_addr.sin_addr.s_addr,
					  hp->h_addr_list[0],
					  sizeof ( remote_addr.sin_addr.s_addr ) );

	if ( connect ( fd_, ( struct sockaddr * ) &remote_addr, sizeof ( remote_addr ) ) < 0 )
		{
			DBGOUT << "connect() failed with " << errno << ": "
			<< strerror ( errno ) << std::endl;
			close ( fd_ );
			throw errno;
		}

	headers_t headers;

	headers ["Protocol-Version"] = "1.0";
	headers ["App-Type"] = "Bot";
	headers ["Operator"] = "PC_CLIENT";
	sendCmd ( "LOGON", headers, "" );
}

unsigned long
PalringoConnection::getUserId()
{
	return userId_;
}

int
PalringoConnection::getFd()
{
	return fd_;
}

bool
PalringoConnection::isGhosted()
{
	return ghosted_;
}

bool
PalringoConnection::isLoggedOn()
{
	return loggedOn_;
}

bool
PalringoConnection::pushOutMessage ( char *msg, unsigned int length )
{
	if ( outBufferCurrent_ + length + sizeof ( length ) > OUT_BUFFER_SIZE )
		{
			DBGOUT << "Buffer Overflow in Bot "
			<< getUserId()
			<< " - dischargin message" << std::endl;
			return false;
		}

	else
		{
			//DBGOUT << "outBufferCurrent_: " << outBufferCurrent_ << std::endl;
			memcpy ( outBuffer_ + outBufferCurrent_, &length, sizeof ( length ) );
			outBufferCurrent_ += sizeof ( length );
			memcpy ( outBuffer_ + outBufferCurrent_, msg, length * sizeof ( *msg ) );
			outBufferCurrent_ += length * sizeof ( *msg );
			outBufferCount_++;
			//DBGOUT << "outBufferCurrent_: " << outBufferCurrent_ << std::endl;
			//DBGOUT << "outBufferCount_: " << outBufferCount_ << std::endl;
			return true;
		}
}

bool
PalringoConnection::popOutMessage ( char *msg, unsigned int *length )
{
	if ( outBufferCount_ )
		{
			//DBGOUT << "Popping out message" << std::endl;
			*length = * ( unsigned int* ) ( outBuffer_ + outBufferBegin_ );
			//DBGOUT << "\tMessage length " << *length << std::endl;
			outBufferBegin_ += sizeof ( *length );
			//DBGOUT<< "outBufferBegin_: " << outBufferBegin_ << std::endl;
			memcpy ( msg, outBuffer_ + outBufferBegin_, *length );

			//DBGOUT << "\tMessage data:\n" << msg << std::endl;
			outBufferBegin_ += *length;
			outBufferCount_--;
			//DBGOUT << "outBufferCount_ : " << outBufferCount_ << std::endl;

			if ( !outBufferCount_ )
				{
					outBufferBegin_ = 0;
					outBufferCurrent_ = 0;
					//DBGOUT << "Reseting buffer" << std::endl;
				};

			return true;
		}

	else
		{
			DBGOUT << "Bot " << getUserId()
			<< ": trying to pop out from empty outBuffer" << std::endl;
			return false;
		}
}


bool
PalringoConnection::sendCmd ( const std::string& cmd,
							  const headers_t& headers,
							  const char *body,
							  unsigned int length )
{
	DBGOUT << cmd << std::endl;

	for ( headers_t::const_iterator it = headers.begin();
			it != headers.end (); ++it )
		DBGOUT << it->first << ": " << it->second << std::endl;

	std::ostringstream ost;

	// Start line
	ost << cmd << std::endl;

	// Headers
	for ( headers_t::const_iterator it = headers.begin();
			it != headers.end (); ++it )
		{
			ost << it->first << ": " << it->second << std::endl;
		} // end for

	// Content-Length
	if(length > 0) {
			ost << "Content-Length: " << length << std::endl;
		}

	// End of headers
	ost << std::endl;

	DBGOUT << "Sending command to buffer: " << std::endl;

	DBGOUT << ost.str() << std::endl;

	char *data = ( char* ) malloc ( ost.str().size() + length );

	// adding headers to message
	memcpy ( data, ost.str().c_str(), ost.str().size() );

	// Body (if any)
	memcpy ( data + ost.str().size(), body, length );

	pushOutMessage ( data, ost.str().size() + length );

	free ( data );

	return true;

}

bool
PalringoConnection::sendCmd ( const std::string& cmd,
							  const headers_t& headers,
							  const std::string& body )
{

	return sendCmd ( cmd, headers, body.c_str(), body.size() );
}

int
PalringoConnection::pollWrite()
{
	if ( outBufferCount_ )
		{
			char data[OUT_BUFFER_SIZE];
			unsigned int length ( 0 );
			popOutMessage ( data, &length );
			//DBGOUT << "Sending message of length "
			//  << length << std::endl;

			int wBytes ( 0 );
			unsigned int seekPos ( 0 );

			while ( seekPos < length )
				{
					//DBGOUT << "Writing " << length - seekPos << " bytes in fd_" << std::endl;
					wBytes = write ( fd_, data + seekPos, length - seekPos );
					//DBGOUT << wBytes << " bytes written in fd_" << std::endl;
					// If something went wrong while writing return -1

					if ( wBytes < 0 )
						{
							std::cout << "Error writing in fd_ " << strerror ( errno ) << std::endl;
							return -1;
						}

					seekPos += wBytes;
				}

			return 1;
		}

	else
		{
			return 0;
		}
}

#ifdef SHIT
static std::ostream&
hexDump ( std::ostream& os, const char* data, size_t length )
{
	for ( size_t i = 0; i < ( length + 15 ) / 16; ++i )
		{
			os << std::hex << std::right << std::setw ( 4 ) << std::setfill ( '0' )
			<< ( i*16 ) << "  ";

			for ( size_t j = i * 16; j < ( i + 1 ) *16; ++j )
				{
					if ( j < length )
						{
							os << std::hex << std::setw ( 2 ) << ( ( int ) data[j] & 255 ) << " ";
						}

					else
						{
							os << "   ";
						}
				}

			os << " ";

			for ( size_t j = i * 16; j < ( i + 1 ) *16 && j < length; ++j )
				{
					os << ( data[j] >= 32 ? data[j] : '.' );
				}
			os << std::endl;
		}
	os << std::setfill ( ' ' ) << std::dec << std::endl;
	return os;
}
#endif

int
PalringoConnection::recvCmd ( std::string& cmd,
                              headers_t& headers,
                              std::string& body )
{
    const char* eom; // End Of Message

  while (1) {
  
        // See if we have a full message in the buffer, either with a content
        // length found in the headers or without
        const char* const crlfcrlf = strstr ( inBuffer_, "\r\n\r\n" );

        if ( crlfcrlf != 0 )
        {
            const char* const cl = strstr ( inBuffer_, "CONTENT-LENGTH: " );

            if ( cl != 0 && cl < crlfcrlf )
            {
                if ( sofar_ >= crlfcrlf + 4 + atoi ( cl + 16 ) - inBuffer_ )
                {
                    eom = crlfcrlf + 4 + atoi ( cl + 16 );
                    break;
                }
            }

            else
            {
                eom = crlfcrlf + 4;
                break;
            }
        }

        // Not a full message in the buffer, let's read some more... Make sure
        // we keep at least one '\0' at the end of the buffer!
        ssize_t nb = read ( fd_, inBuffer_ + sofar_, sizeof ( inBuffer_ ) - sofar_ - 1 );

        if ( nb == 0 )
        {
            std::cout << "LOST CONNECTION!" << std::endl;
            close ( fd_ );
            return -1;
        }

        else if ( nb < 0 )
        {
            int err;
            socklen_t errlen ( sizeof ( err ) );
            if ( getsockopt ( fd_, SOL_SOCKET, SO_ERROR, &err, &errlen ) == -1 )
            {
                std::cout << "getsockopt(3SOCKET): " << strerror ( errno );
                throw ( errno );
            }
            return -1;
        }
        sofar_ += nb;
    } // end while


    // Extract command
    const char* const crlf = strstr ( inBuffer_, "\r\n" );
    cmd = std::string ( inBuffer_, crlf - inBuffer_ );

    // Extract headers
    headers.clear ();
    const char* header = crlf + 2;
    while ( true )
    {
        if ( strncmp ( header, "\r\n", 2 ) == 0 )
        {
            header += 2;
            break;
        }
        const char* const cs = strstr ( header, ": " );
        const char* const crlf = strstr ( header, "\r\n" );
        if ( cs == 0 || crlf == 0 )
            return -1; // Should never happen!
        headers[std::string ( header, cs - header ) ] =
                std::string ( cs + 2, crlf - cs - 2 );
        header = crlf + 2;
    }

    // And the rest is the body (if any)
    body = std::string ( header, eom - header );

    // Anything else in the buffer?
    memcpy ( inBuffer_, eom, sizeof ( inBuffer_ ) - ( eom - inBuffer_ ) );
    memset ( inBuffer_ + sizeof ( inBuffer_ ) - ( eom - inBuffer_ ), 0, eom - inBuffer_ );
    sofar_ -= eom - inBuffer_;

    return 1;
    
}

static unsigned long long ntohll ( unsigned long long data )
{
    // Run-time test to determine our endianess
    static char endianTest[2] = {0, 1};
    static bool bigEndian = ( * ( short * ) endianTest == 1 );
    if ( bigEndian )
    {
        return data;
    }
    return ( ( unsigned long long ) ( ntohl ( ( int ) ( ( data << 32 ) >> 32 ) ) ) << 32 )
      | ( unsigned int ) ntohl ( ( ( int ) ( data >> 32 ) ) );
}

unsigned int
PalringoConnection::getOutMessageCount()
{
    return outBufferCount_;
}

int
PalringoConnection::pollRead()
{

    std::string cmd, body;
    headers_t headers;
    int res = recvCmd ( cmd, headers, body );
    if ( res != 1 )
    {
        return res;
    }
    DBGOUT << "Received command " << cmd << std::endl;
    for(headers_t::const_iterator it = headers.begin();
        it != headers.end(); ++it)
    {
        DBGOUT << "  " << it->first << ": " << it->second << std::endl;
    }

    if ( cmd == "P" ) {
        DBGOUT << "Pong!" << std::endl;
        sendCmd ( "P", headers, "" );
    }

    int mesg_id = atoi ( headers ["MESG-ID"].c_str() );
    if ( mesg_id > 0 )
    {
        mesg_id_ = mesg_id;
    }

    if ( cmd == "AUTH" )
    {
    
        // Save the word-size, we need it for later
        wordSize_ = atoi ( headers ["WORD-SIZE"].c_str() );
        
        // Let's send our login and password
        headers.clear();
        headers ["Encryption-Type"] = "0";
        headers ["Name"] = login_;
        headers ["Online-Status"] = "1";
        sendCmd ( "AUTH", headers, password_ );
    }

    if ( cmd == "LOGON FAILED" )
    {
        onLogonFailed ( headers ["REASON"] );
    }

    else if ( cmd == "LOGON SUCCESSFUL" )
    {
        userId_ = strtoul ( headers["SUB-ID"].c_str(), NULL, 10 );
        onLogonSuccessful();
    }

    else if ( cmd == "GROUP DETAIL" )
    {
        group_t& group = groups_[atoi ( headers ["GROUP-ID"].c_str() ) ];
        
        group.name_ = headers["NAME"];
        
        for ( size_t i = 0; i < body.size () / 8; ++i )
        {
            group.contacts_.insert ( ntohll ( * ( ( unsigned long long* ) ( body.c_str() + 8 * i ) ) ) );
        }
        
    }

    else if ( cmd == "CONTACT DETAIL" )
    {
        contact_t& contact = contacts_[atoi ( headers["CONTACT-ID"].c_str() ) ];
        
        contact.name_ = headers["NICKNAME"];
        contact.onlineStatus_ = atoi ( headers["ONLINE-STATUS"].c_str() );
        contact.status_ = headers["STATUS"];
        contact.isContact_ = ( headers.find("CONTACT") == headers.end() ) ? 0 : atoi( headers["CONTACT"].c_str() );
        contact.deviceType_ = ( headers.find("DEVICE-TYPE") == headers.end() ) ? 0 : atoi( headers["DEVICE-TYPE"].c_str() );
        
    }

    else if ( cmd == "CONTACT ADD" )
    {
        std::string source_id = headers["SOURCE-ID"];

        headers.clear();
        headers["ACCEPTED"] = "1";
        headers["SOURCE-ID"] = source_id;
        char tmp[32];
        sprintf ( tmp, "%d", ++mesg_id_ );
        headers["MESG-ID"] = tmp;

        if ( auto_accept_contacts_ )
        {
            sendCmd ( "CONTACT ADD RESP", headers, "" );
        }

        onContactAddReceived ( body, atoi ( source_id.c_str() ) );
    }

    else if ( cmd == "MESG" )
    {

        const int correl_id = atoi ( headers["CORRELATION-ID"].c_str() );

        if ( headers["LAST"] == "" )
        {
            DBGOUT << "Must wait for more..." << std::endl;
            if ( correl_id > 0 )
            {
                mesgs_[correl_id] += body; // Continuation of existing message
            }
            else
            {
                mesgs_[mesg_id] = body; // First part of new message
            }
            return 1;
        } // end if

        if ( correl_id > 0 )
        {
            mesgs_t::iterator it = mesgs_.find ( correl_id );
            if ( it != mesgs_.end() )
            {
                // Final part of existing message
                body = it->second + body;
                mesgs_.erase ( it );
                DBGOUT << "Last in series" << std::endl;
            }
            else
            {
                DBGOUT << "Unknown correlation ID!" << std::endl;
            }
        }

        onMessageReceived ( body,
                            atoi ( headers["SOURCE-ID"].c_str() ),
                            atoi ( headers["TARGET-ID"].c_str() ),
                            headers["CONTENT-TYPE"] );
    }
    else if ( cmd == "GHOSTED" )
    {
        DBGOUT << "Setting bot with Id " << userId_ << " as GHOSTED" << std::endl;
        onGhostedReceived();
    }

    else if ( cmd == "RESPONSE" )
    {
        onResponseReceived ( What ( atoi ( headers["WHAT"].c_str() ) ),
                                            atoi ( headers["MESG-ID"].c_str() ),
                                            ErrorCode ( atoi ( headers["TYPE"].c_str() ) ) );
    }// end if else

    return 1;

}

int
PalringoConnection::poll()
{
	fd_set fdSetRead;
	fd_set fdSetWrite;

	FD_ZERO ( &fdSetRead );
	FD_ZERO ( &fdSetWrite );
	FD_SET ( fd_, &fdSetRead );
	FD_SET ( fd_, &fdSetWrite );

	int selRes ( 0 );

	if ( ( selRes = checkFd ( fd_ + 1, fdSetRead, fdSetWrite ) ) > 0 )
		{
			if ( FD_ISSET ( fd_, &fdSetWrite ) )
				{
					if ( outBufferCount_ )
						{
							DBGOUT << "Sending Output" << std::endl;

							if ( pollWrite() < 0 )
								{
									return -1;
								}
						}
				}
			if ( FD_ISSET ( fd_, &fdSetRead ) )
				{
					DBGOUT << "Reading command" << std::endl;
					return pollRead();
				}
		}
	else if ( selRes < 0 )
		{
			DBGOUT << "Error checking file descriptor status"
			<< strerror ( errno ) << std::endl;
			throw errno;
			return -1;
		}

	return 0;
}

int
PalringoConnection::checkFd ( int max, fd_set &fdSetRead, fd_set &fdSetWrite )
{
	struct timeval tv = {0, 0};
	return select ( max + 1, &fdSetRead, &fdSetWrite, NULL, &tv );
}

void
PalringoConnection::onLogonFailed ( std::string& reason )
{
	std::cout << "Logon failed: " << reason << std::endl;
}

void
PalringoConnection::onLogonSuccessful()
{
	std::cout << "Logon successful" << std::endl;
	loggedOn_ = true;
}

void
PalringoConnection::onMessageReceived ( std::string& msg,
										unsigned long contact,
										unsigned long group,
										const std::string& contentType )
{
	DBGOUT << msg << " " << contact << " " << group << std::endl;
}

void
PalringoConnection::onMessageReceived ( char *msg,
										unsigned int length,
										unsigned long contact,
										unsigned long group,
										const std::string& contentType )
{
	DBGOUT << "Message of length" << length << " " << contact << " " << group << std::endl;
}


void
PalringoConnection::onContactAddReceived ( std::string& msg,
		unsigned long contact )
{
	DBGOUT << "Contact add request " << contact << " - " << msg << std::endl;
}

void
PalringoConnection::onGhostedReceived()
{
	ghosted_ = true;
}

void
PalringoConnection::onResponseReceived ( What what,
		unsigned long mesgId,
		ErrorCode error )
{
	DBGOUT << "Response Message:" << std::endl
	<< "\tWhat: " << whatStrings[what] << std::endl
	<< "\tMesg-Id: " << mesgId << std::endl
	<< "\tType: " << errorCodes[error] << std::endl;
}

bool
PalringoConnection::sendMessage ( const std::string& msg,
								  const std::string& contentType,
								  unsigned long id,
								  int type )
{

	headers_t headers;
	char tmp[32];
	sprintf ( tmp, "%lu", id );
	headers["TARGET-ID"] = tmp;
	sprintf ( tmp, "%d", type );
	headers["MESG-TARGET"] = tmp;
	sprintf ( tmp, "%d", ++mesg_id_ );
	headers["MESG-ID"] = tmp;
	headers["CONTENT-TYPE"] = contentType;
	headers["LAST"] = "T";
	return sendCmd ( "MESG", headers, msg );

}

bool
PalringoConnection::sendMessage ( const char* msg,
								  unsigned int length,
								  const std::string& contentType,
								  unsigned long id,
								  int type )
{

	headers_t headers;
	char tmp[32];
	sprintf ( tmp, "%lu", id );
	headers["TARGET-ID"] = tmp;
	sprintf ( tmp, "%d", type );
	headers["MESG-TARGET"] = tmp;
	sprintf ( tmp, "%d", ++mesg_id_ );
	headers["MESG-ID"] = tmp;
	headers["CONTENT-TYPE"] = contentType;
	headers["LAST"] = "T";
	return sendCmd ( "MESG", headers, msg, length );

}

bool
PalringoConnection::sendToContact ( const std::string& msg,
									unsigned long contact,
									const std::string contentType )
{
	return sendMessage ( msg, contentType, contact, 0 /* contact */ );
}

bool
PalringoConnection::sendToContact ( const char* msg,
									unsigned int length,
									unsigned long contact,
									const std::string contentType )
{
  return sendMessage(msg, length, contentType, contact, 0 /* contact */);
}

bool
PalringoConnection::sendToGroup ( const std::string& msg,
								  unsigned long group,
								  const std::string contentType )
{
  return sendMessage(msg, contentType, group, 1 /* group */);
}

bool
PalringoConnection::sendToGroup ( const char* msg,
								  unsigned int length,
								  unsigned long group,
								  const std::string contentType )
{
  return sendMessage(msg, length, contentType, group, 1 /* group */);
}
