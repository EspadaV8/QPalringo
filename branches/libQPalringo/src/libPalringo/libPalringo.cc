/*
* libPalringo/libPalringo.cc
* Copyright (c) 2008, Palringo Ltd
*
* Original author: Eric Sellin
* Extended version author: Xabier Eizmendi
* Additions also done by: Andrew Smith <espadav8@gmail.com>
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

#ifdef Q_OS_WIN32
    #define O_NONBLOCK FIONBIO
    #define EADDRINUSE WSAEADDRINUSE
    #define EINPROGRESS WSAEINPROGRESS
    #define EADDRNOTAVAIL WSAEADDRNOTAVAIL
    #define ECONNREFUSED WSAECONNREFUSED
    #include <winsock.h>
    #include <time.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/ip.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <iostream>
#include <iomanip>

//#define DEBUG

#include "libPalringo.h"

#ifdef DEBUG
#define DBGOUT std::cout << __FILE__ << ":" << __LINE__ << ":"
#else
#define DBGOUT if (0) std::cout
#endif

namespace pCommand
  {
  const std::string PING("P");
  const std::string BYE("BYE");
  const std::string AUTH("AUTH");
  const std::string CONTACT_ADD("CONTACT ADD");
  const std::string CONTACT_ADD_RESP("CONTACT ADD RESP");
  const std::string CONTACT_DETAIL("CONTACT DETAIL");
  const std::string CONTACT_UPDATE("CONTACT UPDATE");
  const std::string GHOSTED("GHOSTED");
  const std::string GROUP_ADMIN("GROUP ADMIN");
  const std::string GROUP_CREATE("GROUP CREATE");
  const std::string GROUP_DETAIL("GROUP DETAIL");
  const std::string GROUP_INVITE("GROUP INVITE");
  const std::string GROUP_SUBSCRIBE("GROUP SUBSCRIBE");
  const std::string GROUP_UNSUB("GROUP UNSUB");
  const std::string GROUP_UPDATE("GROUP UPDATE");
  const std::string LOGON("LOGON");
  const std::string LOGON_FAILED("LOGON FAILED");
  const std::string LOGON_SUCCESSFUL("LOGON SUCCESSFUL");
  const std::string MESG("MESG");
  const std::string MESG_HIST("MESG HIST");
  const std::string MESG_STORED("MESG STORED");
  const std::string REG("REG");
  const std::string RESPONSE("RESPONSE");
  const std::string SESSION_END("SESSION END");
  const std::string SUB_PROFILE("SUB PROFILE");
}

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
  "NOT_AVAILABLE",
  "THROTTLE",
  "SUB_ALREADY_EXISTS"
};

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
  "BRIDGING",
  "BRIDGING_MESG",
  "BRI",
  "PLS",
  "GROUP_ADMIN"
};

const uint32_t PalringoConnection::DATA_LIMIT(100*1024);



PalringoConnection::PalringoConnection(const std::string& host,
                                       uint32_t port,
                                       const std::string& login,
                                       const std::string& password,
                                       const std::string& sourceIP,
                                       bool nonBlocking,
                                       int protocolVersion,
                                       bool encryption,
                                       int compression,
                                       const std::string& clientType) :
    login_(login), password_(password), salsa_(NULL), dh_(NULL),
    host_(host), port_(port),
    protocolVersion_(protocolVersion), encryption_(encryption),
    compression_(compression), clientType_(clientType),
    packetSeq_(0), receivedData_(0), sourceIP_(sourceIP), userId_(0),
    outMessageCount_(0), sofar_(0), ghosted_(false),
    mesg_id_(0), auto_accept_contacts_(true), loggedOn_(false),
    nonBlocking_(nonBlocking), connectionReady_(false),
    connectionStatus_(CONN_OFFLINE)
{
  initCallbackFunctions();
  passwordMD5_ = crypto::md5(password_);
} // end of constructor

PalringoConnection::~PalringoConnection()
{
  delete salsa_;
  delete dh_;
}

void
PalringoConnection::initCallbackFunctions()
{
  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::PING,
                                &PalringoConnection::onPingReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::AUTH,
                                &PalringoConnection::onAuthReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::LOGON_SUCCESSFUL,
                                &PalringoConnection::onLogonSuccessfulReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::LOGON_FAILED,
                                &PalringoConnection::onLogonFailedReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::GHOSTED,
                                &PalringoConnection::onGhostedReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::CONTACT_DETAIL,
                                &PalringoConnection::onContactDetailReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::CONTACT_ADD,
                                &PalringoConnection::onContactAddReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::GROUP_DETAIL,
                                &PalringoConnection::onGroupDetailReceived));

  if (protocolVersion_ == 1)
    {
      inCallbackFunctions_.insert(std::pair<std::string,
                                  CmdCallbackFunction>(
                                    pCommand::GROUP_UPDATE,
                                    &PalringoConnection::onGroupUpdateReceived));
    }

  else if (protocolVersion_ == 2)
    {
      inCallbackFunctions_.insert(std::pair<std::string,
                                  CmdCallbackFunction>(
                                    pCommand::GROUP_UPDATE,
                                    &PalringoConnection::onGroupUpdateReceivedV2));
    }

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::REG,
                                &PalringoConnection::onRegReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::RESPONSE,
                                &PalringoConnection::onResponseReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::MESG,
                                &PalringoConnection::onMesgReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::SESSION_END,
                                &PalringoConnection::onSessionEndReceived));

  inCallbackFunctions_.insert(std::pair<std::string,
                              CmdCallbackFunction>(
                                pCommand::SUB_PROFILE,
                                &PalringoConnection::onSubProfileReceived));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::PING,
                                 &PalringoConnection::onPingSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::LOGON,
                                 &PalringoConnection::onLogonSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::BYE,
                                 &PalringoConnection::onByeSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::AUTH,
                                 &PalringoConnection::onAuthSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::CONTACT_UPDATE,
                                 &PalringoConnection::onContactUpdateSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::CONTACT_ADD_RESP,
                                 &PalringoConnection::onContactAddRespSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::GROUP_SUBSCRIBE,
                                 &PalringoConnection::onGroupSubscribeSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::GROUP_UNSUB,
                                 &PalringoConnection::onGroupUnsubSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::GROUP_CREATE,
                                 &PalringoConnection::onGroupCreateSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::GROUP_INVITE,
                                 &PalringoConnection::onGroupInviteSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::GROUP_ADMIN,
                                 &PalringoConnection::onGroupAdminSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::MESG,
                                 &PalringoConnection::onMesgSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::MESG_STORED,
                                 &PalringoConnection::onMesgStoredSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::MESG_HIST,
                                 &PalringoConnection::onMesgHistSent));

  outCallbackFunctions_.insert(std::pair<std::string,
                               CmdCallbackFunction>(
                                 pCommand::REG,
                                 &PalringoConnection::onRegSent));
}

PalringoConnection::GenericData::GenericData() : mesgId_(0)
{
}

PalringoConnection::GenericData::~GenericData()
{
}

PalringoConnection::MsgData::MsgData() : GenericData(),
    sourceId_(0), targetId_(0), mesgTarget_(0), correlationId_(0),
    contentLength_(0), last_(false), totalLength_(0)
{
}

void
PalringoConnection::MsgData::getData(headers_t& headers,
                                     std::string& body __attribute__ ((unused)) )
{
  getAttribute<uint32_t>(headers, "MESG-ID", mesgId_);
  getAttribute<uint64_t, true>(headers, "SOURCE-ID", sourceId_);
  getAttribute<uint64_t, true>(headers, "TARGET-ID", targetId_);
  getAttribute<uint32_t, true>(headers, "MESG-TARGET", mesgTarget_);
  getAttribute<uint32_t, true>(headers, "CORRELATION-ID", correlationId_);
  getAttribute<std::string, true>(headers, "CONTENT-TYPE", contentType_);
  getAttribute<uint32_t, true>(headers, "CONTENT-LENGTH", contentLength_);
  getAttribute<std::string, true>(headers, "EMAIL", email_);
  getAttribute<std::string, true>(headers, "NAME", name_);
  getAttribute<bool>(headers, "LAST", last_);
  getAttribute<uint32_t, true>(headers, "TOTAL-LENGTH", totalLength_);
  getAttribute<std::string, true>(headers, "TIMESTAMP", timestamp_);
  getAttribute<bool>(headers, "HIST", hist_);
}

void
PalringoConnection::MsgData::setData(headers_t& headers,
                                     std::string& body __attribute__ ((unused)) )
{
  headers["MESG-ID"] = toString(mesgId_);

  if (targetId_)
    {
      headers["TARGET-ID"] = toString(targetId_);
    }

  headers["MESG-TARGET"] = toString(mesgTarget_);

  if (correlationId_)
    {
      headers["CORRELATION-ID"] = toString(correlationId_);
    }

  if (contentType_.size())
    {
      headers["CONTENT-TYPE"] = contentType_;
    }

  if (contentLength_)
    {
      headers["CONTENT-LENGTH"] = toString(contentLength_);
    }

  if (last_)
    {
      headers["LAST"] = "T";
    }
}

PalringoConnection::LogonData::LogonData() : GenericData(),
    subId_(0),
    dataMap_(NULL)
{
}

PalringoConnection::LogonData::~LogonData()
{
  delete dataMap_;
}

void
PalringoConnection::LogonData::getData(headers_t& headers,
                                       std::string& body)
{
  dataMap_ = new DataMap(body);
  getAttribute<uint64_t, true>(headers, "SUB-ID", subId_);
  getAttribute<std::string, true>(headers, "NAME", name_);
  getAttribute<std::string, true>(headers, "NICKNAME", nickname_);
  getAttribute<std::string, true>(headers, "STATUS", status_);
  getAttribute<std::string, true>(headers, "LAST-ONLINE", lastOnline_);
  getAttribute<std::string, true>(headers, "REASON", reason_);
  getAttribute<std::string, true>(headers, "TIMESTAMP", timestamp_);
}

void
PalringoConnection::LogonData::setData(headers_t& headers __attribute__ ((unused)),
                                       std::string& body __attribute__ ((unused)) )
{
}

PalringoConnection::AuthData::AuthData() : GenericData(),
    encryptionType_(-1), wordSize_(0),
    onlineStatus_(static_cast<OnlineStatus>(-1)),
    ghost_(-1), contentLength_(0)
{
}

void
PalringoConnection::AuthData::getData(headers_t& headers,
                                      std::string& body __attribute__ ((unused)) )
{
  getAttribute<uint32_t, true>(headers, "WORD-SIZE", wordSize_);
  getAttribute<int32_t, true>(headers, "ENCRYPTION-TYPE", encryptionType_);
}

void
PalringoConnection::AuthData::setData(headers_t& headers,
                                      std::string& body __attribute__ ((unused)) )
{
  if (encryptionType_ > -1)
    {
      headers["ENCRYPTION-TYPE"] = toString(encryptionType_);
    }

  if (name_.size())
    {
      headers["NAME"] = name_;
    }

  if (onlineStatus_ > -1)
    {
      headers["ONLINE-STATUS"] = toString(onlineStatus_);
    }
}

PalringoConnection::ContactData::ContactData() : GenericData(),
    contactId_(0), onlineStatus_(static_cast<OnlineStatus>(-1)),
    deviceType_(static_cast<DeviceType>(0)),
    remove_(false), block_(false), capabilities_(0),
    sourceId_(0), targetId_(0), contentLength_(0)
{
}

void
PalringoConnection::ContactData::getData(headers_t& headers,
                                         std::string& body __attribute__ ((unused)) )
{
  getAttribute<uint32_t, true>(headers, "MESG-ID", mesgId_);
  getAttribute<uint64_t, true>(headers, "CONTACT-ID", contactId_);
  getAttribute<OnlineStatus, true>(headers, "ONLINE-STATUS", onlineStatus_);
  getAttribute<std::string, true>(headers, "NICKNAME", nickname_);
  getAttribute<std::string, true>(headers, "STATUS", status_);
  getAttribute<DeviceType, true>(headers, "DEVICE-TYPE", deviceType_);
  getAttribute<bool>(headers, "REMOVE", remove_);
  getAttribute<bool>(headers, "BLOCK", block_);
  getAttribute<uint64_t, true>(headers, "CAPABILITIES", capabilities_);
  getAttribute<uint64_t, true>(headers, "SOURCE-ID", sourceId_);
  getAttribute<uint64_t, true>(headers, "TARGET-ID", targetId_);
  getAttribute<std::string, true>(headers, "NAME", name_);
  getAttribute<uint32_t, true>(headers, "CONTENT-LENGTH", contentLength_);
  getAttribute<bool>(headers, "CONTACT", isContact_);
}

void
PalringoConnection::ContactData::setData(headers_t& headers __attribute__ ((unused)) ,
                                         std::string& body __attribute__ ((unused)) )
{
}

PalringoConnection::GroupData::GroupData() : GenericData(),
    groupId_(0), contentLength_(0), contactId_(0),
    type_(1),
    targetId_(0), action_(static_cast<GroupStatus>(-1))
{
}

void
PalringoConnection::GroupData::getData(headers_t& headers,
                                       std::string& body __attribute__ ((unused)) )
{
  getAttribute<uint32_t, true>(headers, "MESG-ID", mesgId_);
  getAttribute<uint64_t, true>(headers, "GROUP-ID", groupId_);
  getAttribute<std::string, true>(headers, "NAME", name_);
  getAttribute<uint32_t, true>(headers, "CONTENT-LENGTH", contentLength_);
  getAttribute<std::string, true>(headers, "ADMIN", admin_);
  getAttribute<uint64_t, true>(headers, "CONTACT-ID", contactId_);
  getAttribute<int32_t, true>(headers, "TYPE", type_);
  getAttribute<std::string, true>(headers, "DESC", desc_);
  getAttribute<std::string, true>(headers, "MESSAGE", message_);
  getAttribute<uint64_t, true>(headers, "TARGET-ID", targetId_);
  getAttribute<GroupStatus, true>(headers, "ACTION", action_);
}

void
PalringoConnection::GroupData::setData(headers_t& headers __attribute__ ((unused)) ,
                                       std::string& body __attribute__ ((unused)) )
{
}

PalringoConnection::ResponseData::ResponseData() : GenericData(),
    what_(static_cast<What>(-1)),
    errorCode_(static_cast<ErrorCode>(-1)), type_(-1), contentLength_(-1)
{
}

void
PalringoConnection::ResponseData::getData(headers_t& headers,
    std::string& body)
{
  getAttribute<uint32_t>(headers, "MESG-ID", mesgId_);
  getAttribute<What>(headers, "WHAT", what_);
  getAttribute<int32_t>(headers, "TYPE", type_);
  getAttribute<uint32_t>(headers, "CONTENT_LENGTH", contentLength_);

  if (type_)
    {
      errorMessage_ = body;
    }
  else
    {
      errorCode_ = static_cast<ErrorCode>(ntohl(
                                            *reinterpret_cast<const int*>(body.data() + 4)));
    }
}

void
PalringoConnection::ResponseData::setData(headers_t& headers __attribute__ ((unused)),
                                          std::string& body __attribute__ ((unused)) )
{
}

void
PalringoConnection::createSocket()
{
  fd_ = socket(PF_INET, SOCK_STREAM, 0);
  if (fd_ < 0)
    {
      DBGOUT << "socket() failed with " << errno << ": "
      << strerror(errno) << std::endl;
      //throw errno;
    }

}

void
PalringoConnection::setSocketOptions()
{
  int reuseOptVal(1);
  if (setsockopt(fd_,
                 SOL_SOCKET,
                 SO_REUSEADDR,
                 static_cast<void*>(&reuseOptVal),
                 sizeof(reuseOptVal)) != 0)
    {
      DBGOUT << "setsockopt() failed with " << errno << ": "
      << strerror(errno) << std::endl;
      //throw errno;
    }

  if (nonBlocking_)
    {
      long arg;
      // Set non-blocking
      if ((arg = fcntl(fd_, F_GETFL, NULL)) < 0)
        {
          DBGOUT << "Error fcntl(..., F_GETFL) " << strerror(errno) << std::endl;
          //throw errno;
        }
      arg |= O_NONBLOCK;
      if (fcntl(fd_, F_SETFL, arg) < 0)
        {
          DBGOUT << "Error fcntl(..., F_SETFL) " << strerror(errno) << std::endl;
          //throw errno;
        }
      connectionReady_ = false;
    }

}

int
PalringoConnection::connectClient(bool soft)
{
  if (ghosted_)
    ghosted_ = false;
  while (1)
    {
      createSocket();
      setSocketOptions();

      if (sourceIP_.size() > 0)
        {
          struct sockaddr_in my_addr;
          my_addr.sin_family = AF_INET;
          my_addr.sin_port = htons(getRandPort(1025, 65535));
          DBGOUT << "Using IP:PORT - "
          << sourceIP_ << ":"
          << ntohs(my_addr.sin_port) << std::endl;
          //my_addr.sin_port = 0;
          if (!inet_aton(sourceIP_.c_str(), &my_addr.sin_addr))
            {
              std::cerr << "Wrong IP" << std::endl;
            }

          int32_t count = 60000;
          while (count)
            {
              if (bind (fd_, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
                {
                  if (errno == EADDRINUSE)
                    {
                      close(fd_);
                      createSocket();
                      setSocketOptions();

                      my_addr.sin_family = AF_INET;
                      my_addr.sin_port = htons(getRandPort(1025, 65535));
                      if (!inet_aton(sourceIP_.c_str(), &my_addr.sin_addr))
                        {
                          std::cerr << "Wrong IP" << std::endl;
                        }
                      count--;
                      continue;
                    }

                  else
                    {
                      DBGOUT << "bind() failed with " << errno << ": "
                      << strerror (errno) << std::endl;
                      close (fd_);
                      //throw errno;
                      return -1;
                    }
                }
              else
                {
                  break;
                }
            }
          if (!count)
            {
              errno = EADDRINUSE;
              return -1;
            }
        }


      const struct hostent* hp = gethostbyname(host_.c_str());
      if (hp == 0)
        {
          DBGOUT << "gethostbyname() failed with " << errno << ": "
          << strerror(errno) << std::endl;
          close(fd_);
          return -1;
          //throw errno;
        }

      struct sockaddr_in remote_addr;
      remote_addr.sin_family = AF_INET;
      remote_addr.sin_port = htons (port_);
      (void) memcpy(&remote_addr.sin_addr.s_addr,
                    hp->h_addr_list [0],
                    sizeof (remote_addr.sin_addr.s_addr));

      int32_t res(0);
      if ((res = connect(fd_, (struct sockaddr *)&remote_addr, sizeof(remote_addr))) < 0)
        {
          if ((errno == EINPROGRESS) && nonBlocking_)
            {
              connectionStatus_ = CONN_CONNECTING;
              DBGOUT << "Using non blocking connection" << std::endl;
              connectionReady_ = false;
              break;
            }
          if (errno == EADDRNOTAVAIL)
            {
              close(fd_);
              continue;
            }
          else
            {
              DBGOUT << "connect() failed with " << errno << ": "
              << strerror(errno) << std::endl;
              close(fd_);
              return -1;
            }
        }
      else
        {
          connectionStatus_ = CONN_CONNECTED;
        }
      break;
    }

  if (connectionStatus_ == CONN_CONNECTING)
    {
      DBGOUT << "Connection in progress..." << std::endl;
    }

  else if (connectionStatus_ == CONN_CONNECTED)
    {
      DBGOUT << "Object Connected" << std::endl;
    }

  // Send logon message to the queue;
  headers_t headers;

  if (protocolVersion_ == 1)
    {
      DBGOUT << "Using protocol v1" << std::endl;
      headers["Protocol-Version"] = "1.0";
    }

  else if (protocolVersion_ == 2)
    {
      DBGOUT << "Using protocol v2" << std::endl;
      headers["Protocol-Version"] = "2.0";
      if (encryption_)
        {
          headers["NAME"] = login_;
        }

      if (soft && RK_.size())
        {
          headers["SUB-ID"] = toString(userId_);
        }

      if (!soft || !RK_.size())
        {
          packetSeq_ = 0;
        }

      if (compression_)
        {
          headers["COMPRESSION"] = toString(compression_);
        }
    }


  headers["App-Type"] = clientType_;
  headers["Operator"] = "PC_CLIENT";
  sendCmd(pCommand::LOGON, headers, "");
  return 1;
}

void
PalringoConnection::disconnectClient()
{
  loggedOn_ = false;
  connectionReady_ = false;
  connectionStatus_ = CONN_OFFLINE;
  outMessageCount_ = 0;
  mesg_id_ = 0;
  outStream_.clear();
  inStream_.clear();
  close(fd_);
}

int
PalringoConnection::registerClient()
{
  std::string PRIME("164748038554095637522938371808770983492243597321859580617377449125246076821025950898947834119606129764595090932464011069562774123540090251430953321459148159891506614627039609572581174221921221794617071056528029088532363979628602711602438004905204761275916314232821929662996364486162908845917443626391697066639");

  DBGOUT << "Creating Diffie-Hellman object, will take a while" << std::endl;
  dh_ = new crypto::DiffieHellman(PRIME);
  DBGOUT << "Diffie-Hellman object created" << std::endl;

  createSocket();
  setSocketOptions();
  const struct hostent* hp = gethostbyname(host_.c_str());
  if (hp == 0)
    {
      DBGOUT << "gethostbyname() failed with " << errno << ": "
      << strerror(errno) << std::endl;
      close(fd_);
      return -1;
      //throw errno;
    }

  struct sockaddr_in remote_addr;
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons (port_);
  (void) memcpy(&remote_addr.sin_addr.s_addr,
                hp->h_addr_list [0],
                sizeof (remote_addr.sin_addr.s_addr));

  int32_t res(0);
  if ((res = connect(fd_, (struct sockaddr *)&remote_addr, sizeof(remote_addr))) < 0)
    {
      if ((errno == EINPROGRESS) && nonBlocking_)
        {
          connectionStatus_ = CONN_CONNECTING;
          DBGOUT << "Using non blocking connection" << std::endl;
          connectionReady_ = false;
        }
      else
        {
          DBGOUT << "connect() failed with " << errno << ": "
          << strerror(errno) << std::endl;
          close(fd_);
          return -1;
        }
    }
  else
    {
      connectionStatus_ = CONN_CONNECTED;
    }

  headers_t headers;
  std::string prime(dh_->getPrime());
  std::string A(dh_->getPublic());
  DBGOUT << "prime.size(): " << prime.size()
  << ", A.size(): " << A.size() << std::endl;
  headers["P"] = prime;
  headers["A"] = A;
  sendCmd(pCommand::REG, headers, "");
  return 1;
}

bool
PalringoConnection::connectionReady()
{
  if (connectionReady_)
    {
      return true;
    }
  else
    {
      fd_set fdSetWrite;
      FD_ZERO(&fdSetWrite);
      FD_SET(fd_,&fdSetWrite);
      timeval tv = {0, 1000};
      if (select(fd_+1, NULL, &fdSetWrite, NULL, &tv) > 0)
        {
          connectionReady_ = true;
          return true;
        }
      else
        {
          return false;
        }
    }
}


int
PalringoConnection::reConnect(bool soft)
{
  disconnectClient();
  usleep(1);
  return connectClient(soft);
}

uint64_t
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
PalringoConnection::sendCmd (const std::string& cmd,
                             headers_t& headers,
                             const char *body,
                             uint32_t length)
{
  std::string bodyStr;
  bodyStr.assign(body, length);
  return sendCmd(cmd, headers, bodyStr);
}

bool
PalringoConnection::sendCmd (const std::string& cmd,
                             headers_t& headers,
                             const std::string& body)
{

  DBGOUT << cmd << std::endl;
  for (headers_t::const_iterator it = headers.begin();
       it != headers.end (); ++it)
    DBGOUT << it->first << ": " << it->second << std::endl;

  std::string headerStr(cmd);
  headerStr.append("\n");

  // Headers
  for (headers_t::const_iterator it = headers.begin();
       it != headers.end (); ++it)
    {
      headerStr.append(it->first);
      headerStr.append(": ");
      headerStr.append(it->second);
      headerStr.append("\n");
    } // end for

  // Content-Length
  if (body.size() > 0)
    {
      headerStr.append("CONTENT-LENGTH: ");
      headerStr.append(toString(body.size()));
      headerStr.append("\n");
    }

  // End of headers
  headerStr.append("\n");

  DBGOUT << "Sending command to buffer: " << std::endl;
  DBGOUT << headerStr << std::endl;

  outStream_.append(headerStr);
  outStream_.append(body);
  outMessageCount_++;

  CmdCallbackFunctionsMap::iterator fit(outCallbackFunctions_.find(cmd));

  if (fit != outCallbackFunctions_.end())
    {
      (this->*fit->second)(headers, const_cast<std::string&>(body), NULL);
    }

  else
    {
      processUnknownOutgoing(cmd, headers, const_cast<std::string&>(body));
    }
  return true;
}

bool PalringoConnection::sendRawData(const std::string& data)
{
  DBGOUT << "Sending to buffer: \n" << data << std::endl;
  outStream_.append(data);
  outMessageCount_++;
  return true;
}

int
PalringoConnection::pollWrite()
{
  uint32_t length(0);
  if ((length = outStream_.size()))
    {
      DBGOUT << "Sending data in outStream_:\n"
      << hexDump(outStream_)
      << std::endl;
      const char *outBuf = outStream_.data();

      int32_t wBytes(0);
      uint32_t seekPos(0);

      //while(seekPos < length)
      //{
      //DBGOUT << "Writing " << length - seekPos
      //<< " bytes in fd_" << std::endl;
      wBytes=write(fd_,outBuf + seekPos,length-seekPos);
      // If something went wrong while writing return -1
      if (wBytes < 0)
        {
          if (errno == EAGAIN)
            {
              wBytes = 0;
            }
          else
            {
              std::cerr << "Error writing in fd_ "<< errno << " "
              << strerror(errno) << std::endl;
              return -1;
            }
        }
      DBGOUT << wBytes << " bytes written in fd_" << std::endl;
      seekPos += wBytes;
      //}

      //DBGOUT << "Sent " << outMessageCount_ << " messages" << std::endl;
      outMessageCount_ = 0;
      //outStream_.clear();
      if (wBytes)
        {
          outStream_.erase(0,wBytes);
        }
      return 1;
    }
  else
    {
      return 0;
    }
}

bool
PalringoConnection::parseCmd(std::string& cmd,
                             headers_t& headers,
                             std::string& body)
{
  const char* eom; // End Of Message
  const char *inBuf = inStream_.c_str() + sofar_;
  const char *endBuf = inBuf + inStream_.size() - sofar_;

  // See if we have a full message in the buffer, either with a content
  // length found in the headers or without
  const char* const crlfcrlf = strstr(inBuf, "\r\n\r\n");
  if (crlfcrlf != 0)
    {
      const char* const cl = strstr(inBuf, "CONTENT-LENGTH: ");
      if (cl != 0 && cl < crlfcrlf)
        {
          if (endBuf >= crlfcrlf + 4 + atoi(cl + 16))
            {
              eom = crlfcrlf + 4 + atoi(cl + 16);
            }
          else
            {
              return false;
            }
        }
      else
        {
          eom = crlfcrlf + 4;
        }
    }
    else
    {
        // if we get some user data, just skip over it
        // FIXME
        const char* const ud = strstr(inBuf, "USER_DATA");
        if( ud != 0 )
        {
            sofar_ += endBuf - inBuf;
            return true;
        }
        else
        {
            return false;
        }
    }
  // Extract command
  const char* const crlf = strstr(inBuf, "\r\n");
  cmd = std::string(inBuf, crlf - inBuf);

  // Extract headers
  headers.clear();
  const char* header = crlf + 2;
  while (true)
    {
      if (strncmp(header, "\r\n", 2) == 0)
        {
          header += 2;
          break;
        }
      const char* const cs = strstr(header, ": ");
      const char* const crlf = strstr(header, "\r\n");
      if (cs == 0 || crlf == 0)
        {
          std::cerr << "Should never happen!" << std::endl;
          return false; // Should never happen!
        }
      headers[std::string (header, cs - header)] =
        std::string (cs + 2, crlf - cs - 2);
      header = crlf + 2;
    }

  // And the rest is the body (if any)
  body = std::string(header, eom - header);
  sofar_ += eom - inBuf;

  return true;
}

int
PalringoConnection::readCmd()
{
  char readBuf[IN_BUFFER_SIZE];
  ssize_t nb = read(fd_, readBuf, IN_BUFFER_SIZE);
  if (nb == 0)
    {
      std::cerr << "LOST CONNECTION!" << std::endl;
      close(fd_);
      return -1;
    }

  else if (nb < 0)
    {
      if (errno == EAGAIN)
        {
          return 0;
        }
      else if (errno == ECONNREFUSED)
        {
          std::cerr << "Error while reading: "
          << errno << " " << strerror(errno)
          << std::endl;
          //throw(errno);

          return -1;
        }
      else
        {
          std::cerr << "Error while reading: "
          << errno << " " << strerror(errno)
          << std::endl;
          //throw(errno);

          return -1;
        }
    }
  DBGOUT << "Read bytes: " << nb << std::endl;
  inStream_.append(readBuf,nb);
  DBGOUT << "inStream_ Size: " << inStream_.size() << std::endl;
  DBGOUT << "inStream_ content:\n" << hexDump(inStream_) << std::endl;
  return nb;
}

void
PalringoConnection::setConnectionReady()
{
  connectionStatus_ = CONN_READY;
  loggedOn_ = true;
}

uint64_t
PalringoConnection::ntohll (uint64_t data)
{
  // Run-time test to determine our endianess
  static char endianTest[2] = {0, 1};
  static bool bigEndian = (*(short *)endianTest == 1);
  if (bigEndian)
    {
      return data;
    }

  uint32_t upperword = ntohl(data >> 32);
  uint32_t lowerword = ntohl(data & 0xffff);

  return upperword | lowerword << 32;
}

int
PalringoConnection::pollRead()
{

  std::string cmd, body;
  headers_t headers;
  int32_t res = readCmd();

  if (res < 1)
    {
      return res;
    }

  receivedData_ += res;

  while (parseCmd(cmd, headers, body))
    {
#ifdef DEBUG
      DBGOUT << "Received command " << cmd << std::endl;
      for (headers_t::const_iterator it = headers.begin();
           it != headers.end(); ++it)
        {
          DBGOUT << "  " << it->first << ": " << it->second << std::endl;
        }
#endif

      headers_t::iterator it(headers.find("MESG-ID"));
      if (it != headers.end())
        {
          int32_t mesg_id = atoi(it->second.c_str());
          if (mesg_id > 0)
            {
              mesg_id_ = mesg_id;
            }
        }

      packetSeq_++;

      CmdCallbackFunctionsMap::iterator fit(inCallbackFunctions_.find(cmd));

      if (fit != inCallbackFunctions_.end())
        {
          (this->*fit->second)(headers, body, NULL);
        }

      else
        {
          processUnknownIncoming(cmd, headers, body);
        }
    }

  if (receivedData_ > DATA_LIMIT)
    {
      headers_t pheaders;
      if (protocolVersion_ == 2)
        {
          pheaders["PS"] = toString(packetSeq_);
        }
      sendCmd(pCommand::PING, pheaders, "");
      receivedData_ = 0;
    }

  DBGOUT << "Erasing " << sofar_ << " bytes from buffer" << std::endl;
  inStream_.erase(0, sofar_);
  sofar_ = 0;
  return 1;
}


int
PalringoConnection::processUnknownIncoming(const std::string& cmd,
    headers_t& headers __attribute__ ((unused)),
    std::string& body __attribute__ ((unused)) )
{
  DBGOUT << "Unknown command received: " << cmd << std::endl;
  return 1;
}

int
PalringoConnection::processUnknownOutgoing(
  const std::string& cmd,
  headers_t& headers __attribute__ ((unused)),
  std::string& body __attribute__ ((unused)) )
{
  DBGOUT << "Unknown command received: " << cmd << std::endl;
  return 1;
}

int
PalringoConnection::poll()
{
  fd_set fdSetRead;
  fd_set fdSetWrite;

  FD_ZERO(&fdSetRead);
  FD_ZERO(&fdSetWrite);
  FD_SET(fd_,&fdSetRead);
  FD_SET(fd_,&fdSetWrite);

  int32_t selRes(0);

  if ((selRes = checkFd(fd_+1, fdSetRead, fdSetWrite)) > 0)
    {
      if (FD_ISSET(fd_, &fdSetWrite))
        {
          if (outStream_.size())
            {
              DBGOUT << "Sending Output" << std::endl;

              if (pollWrite() < 0)
                {
                  return -1;
                }
            }
        }
      if (FD_ISSET(fd_, &fdSetRead))
        {
          DBGOUT << "Reading command" << std::endl;
          return pollRead();
        }
    }
  else if (selRes < 0)
    {
      DBGOUT << "Error checking file descriptor status"
      << strerror(errno) << std::endl;
      //throw errno;
      return -1;
    }
  return 0;
}

int
PalringoConnection::checkFd(int32_t max, fd_set &fdSetRead, fd_set &fdSetWrite)
{
  struct timeval tv =
    {
      0, 1000
    };

  return select(max+1, &fdSetRead, &fdSetWrite, NULL, &tv);
}

uint32_t
PalringoConnection::getRandPort(uint32_t min, uint32_t max)
{
  return((rand() % (max - min)) + min);
}

bool
PalringoConnection::sendMessage(const std::string& msg,
                                std::string& contentType,
                                uint64_t id,
                                int32_t type)
{

  headers_t headers;
  MsgData data;
  data.targetId_ = id;
  data.mesgTarget_ = type;
  data.mesgId_ = ++mesg_id_;
  data.contentType_ = contentType;
  if (msg.size() > 512)
    {
      const char *cmsg = msg.c_str();
      data.setData(headers, const_cast<std::string&>(msg));
      sendCmd(pCommand::MESG, headers, std::string(cmsg, 512));

      data.correlationId_ = data.mesgId_;

      size_t i(512);

      for (i=512; i < (msg.size() - (msg.size() % 512)); i+=512)
        {
          data.mesgId_ = ++mesg_id_;
          data.setData(headers, const_cast<std::string&>(msg));
          sendCmd(pCommand::MESG, headers, std::string(cmsg + i, 512));
        }

      data.mesgId_ = ++mesg_id_;
      data.last_ = true;
      data.setData(headers, const_cast<std::string&>(msg));
      return sendCmd(pCommand::MESG, headers,std::string(cmsg + i, msg.size() - i));
    }
  else
    {
      data.last_ = true;

      data.setData(headers, const_cast<std::string&>(msg));

      return sendCmd(pCommand::MESG, headers, msg);
    }
}

bool
PalringoConnection::sendMessage(char* msg,
                                uint32_t length,
                                std::string& contentType,
                                uint64_t id,
                                int32_t type)
{
  std::string msgStr;
  msgStr.append(msg, length);

  return sendMessage(msgStr, contentType, id, type);

}

bool
PalringoConnection::sendToContact(const std::string& msg,
                                  uint64_t contact,
                                  std::string contentType)
{
  return sendMessage(msg, contentType, contact, 0 /* contact */);
}

bool
PalringoConnection::sendToContact(char* msg,
                                  uint32_t length,
                                  uint64_t contact,
                                  std::string contentType)
{
  std::string msgStr;
  msgStr.append(msg, length);
  return sendMessage(msgStr, contentType, contact, 0 /* contact */);
}

bool
PalringoConnection::sendToGroup(const std::string& msg,
                                uint64_t group,
                                std::string contentType)
{
  return sendMessage(msg, contentType, group, 1 /* group */);
}

bool
PalringoConnection::sendToGroup(char* msg,
                                uint32_t length,
                                uint64_t group,
                                std::string contentType)
{
  std::string msgStr;
  msgStr.append(msg, length);
  return sendMessage(msgStr, contentType, group, 1 /* group */);
}

bool
PalringoConnection::sendPls(std::string &data)
{
  headers_t headers;
  headers["MESG-ID"] = toString(getMesgId());
  return sendCmd("PLS SUBMIT", headers, data);
}

void
PalringoConnection::getMesgHist(int32_t count,
                                uint32_t timestamp,
                                uint64_t sourceId,
                                int32_t type)
{
  std::string timestampStr("");
  timestampStr.append(toString(timestamp));
  timestampStr.append(".0");

  getMesgHist( count, timestamp, sourceId, type );
}

void
PalringoConnection::getMesgHist(int32_t count,
                                const std::string &timestampStr,
                                uint64_t sourceId,
                                int32_t type)
{
  headers_t headers;
  headers["MESG-ID"] = toString(++mesg_id_);
  headers["COUNT"] = toString(count);
  headers["SOURCE-ID"] = toString(sourceId);
  
  if (type == 0)
    {
      headers["FROM-PRIVATE"] = timestampStr;
    }

  else if (type == 1)
    {
      headers["FROM-GROUP"] = timestampStr;
    }

  sendCmd(pCommand::MESG_HIST, headers, "");
}

void
PalringoConnection::setLogin(const std::string &login)
{
  login_.assign(login);
}

void
PalringoConnection::setPassword(const std::string &password)
{
  password_.assign(password);
}

int
PalringoConnection::getContact(uint64_t id, contact_t& contact)
{
  contacts_t::iterator it(contacts_.find(id));
  if (it != contacts_.end())
    {
      contact_t res = it->second;
      contact.onlineStatus_ = res.onlineStatus_;
      contact.status_ = res.status_;
      contact.nickname_ = res.nickname_;
      contact.isContact_ = res.isContact_;
      return 1;
    }
  else
    {
      return 0;
    }
}


int
PalringoConnection::getGroup(uint64_t id, group_t& group)
{
  groups_t::iterator it(groups_.find(id));
  if (it != groups_.end())
    {
      group = it->second;
      return 1;
    }
  else
    {
      return 0;
    }
}

void
PalringoConnection::getGroups(groups_t& groups)
{
  groups = groups_;
}

void PalringoConnection::groupSubscribe( std::string groupName )
{
    headers_t headers;
    headers["MESG-ID"] = toString(++mesg_id_);
    headers["NAME"] = groupName;

    sendCmd(pCommand::GROUP_SUBSCRIBE, headers, "");
}

void PalringoConnection::groupCreate( std::string groupName,
                                      std::string groupDesc,
                                      std::string groupPassword )
{
    headers_t headers;
    headers["MESG-ID"] = toString(++mesg_id_);
    headers["NAME"] = groupName;
    
    if( groupDesc.size() )
    {
        headers["DESC"] = groupDesc;
    }
    if( groupPassword.size() )
    {
        headers["CONTENT-LENGTH"] = groupPassword.size();
    }

    sendCmd(pCommand::GROUP_CREATE, headers, groupPassword);
}

void PalringoConnection::groupUnsubscribe( uint64_t groupID )
{
    headers_t headers;
    headers["MESG-ID"] = toString(++mesg_id_);
    headers["GROUP-ID"] = toString( groupID );
    
    sendCmd(pCommand::GROUP_UNSUB, headers, "");
}
