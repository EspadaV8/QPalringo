/*
 * libPalringo/libPalringo.h
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
#ifndef LIBPALRINGO_H
#define LIBPALRINGO_H



#include <string>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <queue>
#include <inttypes.h>
#include "DataMap.h"
#include "MiscFunctions.h"
#include "crypto.h"


#define IN_BUFFER_SIZE 4096
#define DUMP_ROW_SIZE 16

namespace pCommand
{
  extern const std::string PING;
  extern const std::string BYE;
  extern const std::string AUTH;
  extern const std::string CONTACT_ADD;
  extern const std::string CONTACT_ADD_RESP;
  extern const std::string CONTACT_DETAIL;
  extern const std::string CONTACT_UPDATE;
  extern const std::string GHOSTED;
  extern const std::string GROUP_ADMIN;
  extern const std::string GROUP_CREATE;
  extern const std::string GROUP_DETAIL;
  extern const std::string GROUP_INVITE;
  extern const std::string GROUP_SUBSCRIBE;
  extern const std::string GROUP_UNSUB;
  extern const std::string GROUP_UPDATE;
  extern const std::string LOGON;
  extern const std::string LOGON_FAILED;
  extern const std::string LOGON_SUCESSFUL;
  extern const std::string MESG;
  extern const std::string MESG_HIST;
  extern const std::string MESG_STORED;
  extern const std::string REG;
  extern const std::string RESPONSE;
  extern const std::string SESSION_END;
  extern const std::string SUB_PROFILE;
}

/**
 * @brief Error code list.
 *
 * This enum includes a list of all the error codes.
 * OK means there hasn't been any error.
 */
enum ErrorCode
{
  OK, /**< The command was executed with no error*/
  INTERNAL_ERROR,
  NO_SUCH_WHATEVER,
  NOT_A_MEMBER,
  DELIVERED, /**< Message delivered*/
  NOT_DELIVERED, /**< Message lost*/
  SFE_NOT_AVAILABLE,
  STATS_IF_NOT_AVAILABLE,
  END_OF_MESG_STORED,
  UNABLE_TO_STORE_OFFLINE_MESG,
  RESEND_CURRENT_MESG,
  GROUP_ALREADY_EXISTS,
  CONTACT_ALREADY_EXISTS,
  NOT_ALLOWED,
  NOT_AVAILABLE,
  THROTTLE,
  SUB_ALREADY_EXISTS
};

/**
 * @brief Response type.
 *
 * This enum includes a list of all the response types.
 */
enum What
{
  ADD_CONTACT,
  SUBSCRIBE_TO_GROUP,
  CREATE_GROUP,
  UPDATE_CONTACT,
  UNSUB_GROUP,
  UPDATE_PRESENCE,
  UPDATE_NICKNAME,
  UPDATE_STATUS_MESG,
  CONTACT_AUTHORISATION,
  DEST_QUERY,
  GROUP_INVITE,
  MESG,
  MESG_STORE,
  STATS_LOGGING,
  BRIDGING,
  BRIDGING_MESG,
  BRI,
  PLS,
  GROUP_ADMIN
};

extern const std::string whatStrings[];
extern const std::string errorCodes[];

enum OnlineStatus
{
  OFFLINE,
  ONLINE,
  AWAY,
  INVISIBLE,
  DND,
  BUSY,
  BRB,
  LUNCH,
  ON_PHONE,
  IDLE
};


enum GroupStatus
{
  GROUP_MEMBER_REGULAR       = 0x00,
  GROUP_MEMBER_ADMIN         = 0x01,
  GROUP_MEMBER_MOD           = 0x02,
  GROUP_MEMBER_BANNED        = 0x04,
  GROUP_MEMBER_SILENCED      = 0x08,
  GROUP_MEMBER_HIDDEN        = 0x10
};

enum DeviceType
{
  BOT = 1,
  PC,
  HANDHELD
};

enum ConnectionStatus
{
  CONN_OFFLINE,
  CONN_CONNECTING,
  CONN_CONNECTED,
  CONN_AUTHENTICATION,
  CONN_GETTING_DATA,
  CONN_READY
};

typedef std::map <std::string, std::string> headers_t;

typedef std::set <uint64_t> group_contacts_t;

typedef struct {
  char* msg;
  uint32_t length;
} msg_t;

typedef struct {
  std::string name_;
  std::string desc_;
  group_contacts_t contacts_;
} group_t;

typedef std::map <int, group_t> groups_t;


typedef struct {
  std::string nickname_;
  int32_t onlineStatus_;
  std::string status_;
  DeviceType deviceType_;
  bool isContact_;
} contact_t;

typedef std::map <int, contact_t> contacts_t;

typedef std::map <int, std::string> mesgs_t;


template <class T,bool B>
inline void
getAttribute(headers_t& header,
    std::string& key,
    T& attribute)
{
  headers_t::iterator it(header.find(key));
  if (it != header.end())
  {
    attribute = static_cast<T>(strtoul(it->second.c_str(), NULL, 10));
  }
}

template <>
inline void
getAttribute<std::string, true>(headers_t& header,
    std::string& key,
    std::string& attribute)
{
  headers_t::iterator it(header.find(key));
  if (it != header.end())
  {
    attribute = it->second;
  }
}

template <class T>
inline void
getAttribute(headers_t& header,
    std::string& key,
    T& attribute)
{
  attribute = static_cast<T>(strtoul(header[key].c_str(), NULL, 10));
}

template <>
inline void
getAttribute<std::string>(headers_t& header,
    std::string& key,
    std::string& attribute)
{
  attribute = header[key];
}

template <>
inline void
getAttribute<bool>(headers_t& header,
    std::string& key,
    bool& attribute)
{
  attribute = header.count(key) ? true : false;
}

template <class T,bool B>
inline void
getAttribute(headers_t& header,
    const char *key,
    T& attribute)
{
  headers_t::iterator it(header.find(key));
  if (it != header.end())
  {
    attribute = static_cast<T>(strtoul(it->second.c_str(), NULL, 10));
  }
}

template <>
inline void
getAttribute<std::string, true>(headers_t& header,
    const char *key,
    std::string& attribute)
{
  headers_t::iterator it(header.find(key));
  if (it != header.end())
  {
    attribute = it->second;
  }
}

template <class T>
inline void
getAttribute(headers_t& header,
    const char *key,
    T& attribute)
{
  attribute = static_cast<T>(strtoul(header[key].c_str(), NULL, 10));
}

template <>
inline void
getAttribute<std::string>(headers_t& header,
    const char *key,
    std::string& attribute)
{
  attribute = header[key];
}

template <>
inline void
getAttribute<bool>(headers_t& header,
    const char *key,
    bool& attribute)
{
  attribute = header.count(key) ? true : false;
}

std::string hexDump(std::string &data);
std::string hexDump(const void *data, size_t size);

// Capabilities
const uint64_t SEND_VOICE (0x00000001);
const uint64_t SEND_PICTURE(0x00000002);
const uint64_t RECV_VOICE (0x00000004);
const uint64_t RECV_PICTURE(0x00000008);
const uint64_t ONLINE_MESG (0x00000010);
const uint64_t OFFLINE_MESG(0x00000020);
const uint64_t LOCATION (0x00000040);



class PalringoConnection
{
protected:

  static const uint32_t DATA_LIMIT;
  // In all the integer values 0 means unset, unless otherwise specified
  class GenericData
  {
    public:
      uint32_t mesgId_;

      GenericData();
      virtual ~GenericData();

      virtual void getData(headers_t& headers, std::string& body) =0;
      virtual void setData(headers_t& headers, std::string& body) =0;
  };

  typedef int(PalringoConnection::* CmdCallbackFunction) (headers_t& headers,
      std::string& body,
      GenericData *data);

  typedef std::map<std::string, CmdCallbackFunction> CmdCallbackFunctionsMap;


  class MsgData : public GenericData
  {
    public:
      MsgData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      // sender of the message
      uint64_t sourceId_;
      // if exists, the ID of the group it's going to
      uint64_t targetId_;
      // not used?
      uint32_t mesgTarget_;
      uint32_t correlationId_;
      // type of message
      std::string contentType_;
      // length of message
      uint32_t contentLength_;
      std::string email_;
      std::string name_;
      // last part of the message
      bool last_;
      uint32_t totalLength_;

      // time the message was sent
      std::string timestamp_;
      // is the message a requested history one?
      bool hist_;
  };

  class LogonData : public GenericData
  {
    public:
      LogonData();
      ~LogonData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      uint64_t subId_;
      std::string name_;
      std::string nickname_;
      std::string status_;
      std::string lastOnline_;
      std::string reason_;
      std::string timestamp_;
      DataMap *dataMap_;
  };

  class AuthData : public GenericData
  {
    public:
      AuthData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      int32_t encryptionType_; // -1 means unset
      uint32_t wordSize_;
      std::string name_;
      OnlineStatus onlineStatus_; // -1 means unset
      int32_t ghost_; // -1 means unset
      uint32_t contentLength_;
  };

  class ContactData : public GenericData
  {
    public:
      ContactData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      uint64_t contactId_;
      OnlineStatus onlineStatus_; // -1 means unset
      std::string nickname_;
      std::string status_;
      DeviceType deviceType_;
      bool remove_;
      bool block_;
      uint64_t capabilities_;
      uint64_t sourceId_;
      uint64_t targetId_;
      std::string name_;
      uint32_t contentLength_;
      bool isContact_;
  };

  class GroupData : public GenericData
  {
    public:
      GroupData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      uint64_t groupId_;
      std::string name_;
      uint32_t contentLength_;
      std::string admin_;
      uint64_t contactId_;
      int32_t type_; // -1 means unset
      std::string desc_;
      std::string message_;
      uint64_t targetId_;
      GroupStatus action_; // -1 means unset
  };

  class ResponseData : public GenericData
  {
    public:
      ResponseData();

      void getData(headers_t& headers, std::string& body);
      void setData(headers_t& headers, std::string& body);

      What what_;
      ErrorCode errorCode_;
      std::string errorMessage_;
      int32_t type_;
      uint32_t contentLength_;
  };

  template <class T>
  inline T*
  getDataPtr(PalringoConnection::GenericData *data, T *defaultDataObj)
  {
    T *res(dynamic_cast<T *>(data));
    if(res)
    {
      return res;
    }
    else
    {
      return defaultDataObj;
    }
  }

  inline int32_t
  getMesgId()
  {
    return ++mesg_id_;
  }

  CmdCallbackFunctionsMap inCallbackFunctions_;
  CmdCallbackFunctionsMap outCallbackFunctions_;

  /**
   * @brief User login.
  */
  std::string login_;

  /**
   * @brief User password.
  */
  std::string password_;

  std::string passwordMD5_;

  crypto::SalsaCipher *salsa_;

  crypto::DiffieHellman *dh_;

  /**
   * @brief Server hostname.
  */
  const std::string host_;

  /**
   * @brief Server port.
  */
  uint32_t port_;

  const int protocolVersion_;

  bool encryption_;

  int compression_;

  uint64_t packetSeq_;

  uint32_t receivedData_;

  /**
   * @brief Source IP
   *
   * The IP from which it connects to the server
  */
  const std::string sourceIP_;

  /**
   * @brief Socket file descriptor.
  */
  int32_t fd_;

  /**
   * @brief Word Size as returned by the server in the AUTH command.
  */
  int32_t wordSize_;

  /**
   * @brief User ID.
   *
   * This connections User ID, it's 0 until it's logged on.
  */
  uint64_t userId_;

  /**
   * @brief Nickname.
   *
   * User nickname.
  */
  std::string nickname_;

  /**
   * @brief Status.
   *
   * Status of the user.
   */
  std::string status_;

  /**
   * @brief Last online timestamp
   */
  std::string lastOnline_;

  /**
   * @brief Servers timestamp
   */
  std::string serverTimestamp_;

  /**
   * @brief Active reconnection key
   *
   * It's the reconnection key used when the connections is lost.
   * If it's size is 0, it does a full connection.
   */
  std::string RK_;

  /**
   * @brief List of groups we subscribe to.
  */
  groups_t groups_;

  /**
   * @brief List of contacts we know about.
  */
  contacts_t contacts_;

  /**
   * @brief Parts of messages.
  */
  mesgs_t mesgs_;

  /**
   * @brief Initialize callback functions
   */
  virtual void initCallbackFunctions();

  /**
   * @brief Parse commands from received data.
   *
   * This is an internal function, used to parse commands from the received data
   * @param cmd is the command name.
   * @param headers is the headers of the command.
   * @param body is the body of the command.
   * @return True if a full command was parsed from the received data.
   * */
  bool parseCmd(std::string& cmd,
      headers_t& headers,
      std::string& body);

  /**
   * @brief Read data from the socket
   * @return number of bytes read
   */
  int readCmd();

// Incoming Callback Functions


   virtual int onPingReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onAuthReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

  /**
   * @brief On Logon Succesful Event callback function
   *
   * This function is called if the log on process is successful.
   * @see onLogonFailed()
   * @see poll()
   */
   virtual int onLogonSuccessfulReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

  /**
   * @brief On Logon Failed Event callback function
   *
   * This function is called if the log on process fails.
   * @param reason is the reason why the log on has failed.
   * @see onLogonSuccessful()
   * @see poll()
   */
   virtual int onLogonFailedReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

  /**
   *@brief  On Ghosted Received Event callback function
   *
   * This function is called if the a GHOSTED command is received.
   * @see pollRead()
   * @see poll()
   */
   virtual int onGhostedReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onContactDetailReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

  /**
   * @brief On Contact Add Received Event callback function
   *
   * This function is called if a CONTACT ADD command is received.
   * @param msg is the message sent by the adder.
   * @param contact is the user ID of the adder
   * @see pollRead()
   * @see poll()
   */
   virtual int onContactAddReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupDetailReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupUpdateReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupUpdateReceivedV2(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onRegReceived(headers_t& headers,
                             std::string& body,
			     GenericData *data);
  /**
   * @brief On Response Received Event callback function
   *
   * This function is called if RESPONSE command is received.
   * @param what is the type of command the response is referring to.
   * @param mesgId is the message ID the response is referring to.
   * @param error is the error code (if any) the response is referring to.
   * @see pollRead()
   * @see poll()
   */
   virtual int onResponseReceived(headers_t& headers,
       std::string& body,
       GenericData *data);


  /**
   * @brief On Message Received Event callback function, string message
   *
   * This function is called if a message is received and is of string type.
   * @param msg is the message received.
   * @param contact is the id of the group that sent the message.
   * @param group is the id of the group that sent the message.
   * @param contentType is the content type of the message.
   * @see recvCmd()
   * @see pollRead()
   * @see poll()
   */
   virtual int onMesgReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onSessionEndReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onSubProfileReceived(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int processUnknownIncoming(const std::string& cmd,
       headers_t& headers,
       std::string& body);

   virtual int onPingSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onLogonSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onByeSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onAuthSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onContactUpdateSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onContactAddRespSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupSubscribeSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupUnsubSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupCreateSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupInviteSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onGroupAdminSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onMesgSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onMesgStoredSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onMesgHistSent(headers_t& headers,
       std::string& body,
       GenericData *data);

   virtual int onRegSent(headers_t& headers,
                         std::string& body,
			 GenericData *data);

   virtual int processUnknownOutgoing(
       const std::string& cmd,
       headers_t& headers,
       std::string& body);

  /**
   * @brief Received data buffer (Input Buffer)
   * @see readCmd()
   */
  std::string inStream_;

  /**
   * @brief Sent data buffer (Output Buffer)
   */
  std::string outStream_;

  /**
   * @brief Messages in Output buffer
   */
  uint32_t outMessageCount_;

  /**
   * @brief Bytes read sofar
   * @see readCmd()
   */
  ssize_t sofar_;

  /**
   * @brief This is set to true if the connection is Ghosted
   * @see isGhosted()
   */
  bool ghosted_;

  /**
   * @brief The message ID used to send commands.
   *
   * This variable increments every time we send a command.
   */
  int32_t mesg_id_;

  /**
   * @brief If true accept contacts automatically.
   * @see setAutoAcceptContacts()
   */
  bool auto_accept_contacts_;

  /**
   * @brief If true it's logged on
   * @see isLoggedOn()
   */
  bool loggedOn_;

  /**
   * @brief If true the connection is non blocking
   */
  bool nonBlocking_;

  /**
   * @brief If true the connection is ready
   */
  bool connectionReady_;

  /**
   * @brief Contains the connection status
   * @see ConnectionStatus
   */
  ConnectionStatus connectionStatus_;

public:

  /**
   * @brief Constructor, creates the PalringoConnection object
   *
   * @param host is the hostname of the server.
   * @param port is the port of the server.
   * @param login
   * @param password
   * @param sourceIP is the IP from which the connection is made
   * @param nonBlocking use nonblocking connection?
   * @param protocolVersion is the protocol version used, valid values 1 or 2
   * @param encryption if true encryption is active
   * @return
   */
  PalringoConnection(const std::string& host,
		     uint32_t port,
		     const std::string& login,
		     const std::string& password,
		     const std::string& sourceIP = "",
		     bool nonBlocking = false,
		     int protocolVersion = 1,
		     bool encryption = false,
		     int compression = 0);


  /**
   * @brief Object destructor
   */
  virtual ~PalringoConnection();

  void createSocket();
  void setSocketOptions();

  /**
   * @brief Connect the client to the server
   * @param soft if true, it tries to connect using the reconnection key
   * @return the same as connect() function
   */
  virtual int connectClient(bool soft = false);

  /**
   * @brief Disconnect the client#
   */
  virtual void disconnectClient();

  /**
   * @brief Register a subscriber to Palringo
   */
  virtual int registerClient();

  bool connectionReady();

  /**
   * @brief Poll the connection (read and write)
   *
   * This function checks the file descriptor and reads and/or writes data.
   * @see pollRead()
   * @see pollWrite()
   * @return -1 if an error occurred, 0 if there wasn't any read or writing
   * and 1 if there was some read or writing
   */
  int poll();

  /**
   * @brief Reconnect the bot
   * @param soft if true, it tries to reconnect using the reconnection key
   */
  virtual int reConnect(bool soft = false);


  /**
   * @brief Get the userId of the bot, 0 if it's still not logged on
   * @return userId of the connection, 0 if not logged on
   */
  uint64_t getUserId();


  /**
   * @brief Get the file descriptor
   * @return file descriptor of the connection
   */
  int getFd();

  /**
   * @brief Tells if the connection has been ghosted
   * @return True if ghosted, false if not
   */
  bool isGhosted();

  /**
   * @brief Tells if the connection is ready to send messages
   * @return True if logged on, false if not
   */
  bool isLoggedOn();

  /**
   * @brief Get number of messages in output buffer
   * @return number of messages in output buffer
   */
  inline uint32_t getOutMessageCount()
  {
    return outMessageCount_;
  }

  inline std::string getLogin()
  {
    return login_;
  }
  inline std::string getPassword()
  {
    return password_;
  }

  inline std::string getHost()
  {
    return host_;
  }

  inline uint32_t getPort()
  {
    return port_;
  }

  inline void setPort(uint32_t port)
  {
    port_ = port;
  }

  /**
   * @brief Read next incoming command.
   *
   * Before using this command you have to check if the file descriptor is ready
   * to read, otherwise it can block the process.
   * @see poll()
   * @return -1 if an error occurred, 0 if there wasn't a complete command to
   * read, 1 if a command was successfully read.
   */
  int pollRead();

  /**
   * @brief Write next command in output buffer.
   *
   * Before using this command you have to check if the file descriptor is ready
   * to write, otherwise it can block the process.
   * @see poll()
   * @return number of messages sent, -1 if an error occurred,
   * 0 if there wasn't any command in the.
   */
  int pollWrite();

  /**
   * @brief Check if the fd set is ready for read and/or write.
   *
   * It uses de select function to test a set of file descriptors.
   * @param max is the biggest file descriptor of all the sets.
   * @param fdSetRead is a set of file descriptors to be checked if ready to read.
   * The function returns the set of fd ready to read.
   * @param fdSetWrite is a set of file descriptors to be checked if ready to write
   * The function returns the set of fd ready to write.
   * @return number of file descriptors ready, -1 if an error occurred
   */
  static int checkFd(int32_t max, fd_set &fdSetRead, fd_set &fdSetWrite);

  /**
   * @brief Get a random number between a range
   */
  static uint32_t getRandPort(uint32_t min, uint32_t max);

  /**
   * @brief Convert a 64 bit network integer to host type one
   */
  static uint64_t ntohll (uint64_t data);

  /**
   * @brief Send a command to the server, any type of body
   *
   * It sends a command to the server.
   * Body can be of binary type.
   * The message is pushed into the output buffer and sent later using pollWrite().
   * @param cmd is the command name.
   * @param headers is a map that includes all the headers for the command.
   * @param body is a pointer to the buffer where the body of the message is stored.
   * @param length is the length of the message.
   * @see pollWrite()
   * @see sendMessage()
   * @see sendToContact()
   * @see sendToGroup()
   * @return True if the message was successfully pushed in the buffer and false if not.
   */
  virtual bool sendCmd (const std::string& cmd,
      headers_t& headers,
      const char *body,
      uint32_t length);

  /**
   * @brief Send a command to the server, body is a string
   *
   * It sends a command to the server.
   * Body is of type string.
   * The message is pushed into the output buffer and sent later using pollWrite().
   * @param cmd is the command name.
   * @param headers is a map that includes all the headers for the command.
   * @param body is a string where the body of the message is stored.
   * @see pollWrite()
   * @see sendMessage()
   * @see sendToContact()
   * @see sendToGroup()
   * @return True if the message was successfully pushed in the buffer and false if not.
   */
  virtual bool sendCmd(const std::string& cmd,
      headers_t& headers,
      const std::string& body = "");

  /**
   * @brief Send a string type message
   *
   * The message is stored in a string.
   * @param msg is the string containing the message.
   * @param contentType is the content type of the message.
   * @param id is the ID of the contact or group.
   * @param type is the destination type (0 for contact and 1 for group).
   * @see sendCmd()
   * @see sendToContact()
   * @see sendToGroup()
   * @see pollWrite()
   */
  virtual bool sendMessage(const std::string& msg,
      std::string& contentType,
      uint64_t id,
      int32_t type /* 0 for contact, 1 for group */);

  /**
   * @brief Send any type of message
   *
   * The message is stored in a buffer called msg.
   * @param msg is the buffer containing the message.
   * @param length is the length of the message.
   * @param contentType is the content type of the message.
   * @param id is the ID of the contact or group.
   * @param type is the destination type (0 for contact and 1 for group).
   * @see sendCmd()
   * @see sendToContact()
   * @see sendToGroup()
   * @see pollWrite()
   */
  virtual bool sendMessage(char* msg,
      uint32_t length,
      std::string& contentType,
      uint64_t id,
      int32_t type /* 0 for contact, 1 for group */);

  /**
   * @brief Send a string type message to a contact
   *
   * The message is stored in a string.
   * @param msg is the string containing the message.
   * @param contact is the ID of the contact.
   * @param contentType is the content type of the message.
   * "text/plain" by default
   * @see sendCmd()
   * @see sendMessage()
   * @see pollWrite()
   * @return True if successfully sent, false if not
   */
  virtual bool sendToContact(const std::string& msg,
      uint64_t contact,
      std::string contentType = "text/plain");

  /**
   * @brief Send any type of message to a contact
   *
   * The message is stored in a buffer called msg.
   * @param msg is the buffer containing the message.
   * @param length is the length of the message.
   * @param contact is the ID of the contact.
   * @param contentType is the content type of the message.
   * "text/plain" by default.
   * @see sendCmd()
   * @see sendMessage()
   * @see pollWrite()
   * @return True if successfully sent, false if not
   */
  virtual bool sendToContact(char* msg,
      uint32_t length,
      uint64_t contact,
      std::string contentType = "text/plain");

  /**
   * @brief Send a string type message to a group
   *
   * The message is stored in a string.
   * @param msg is the string containing the message.
   * @param group is the ID of the group.
   * @param contentType is the content type of the message.
   * "text/plain" by default.
   * @see sendCmd()
   * @see sendMessage()
   * @see pollWrite()
   * @return True if successfully sent, false if not
   */
  virtual bool sendToGroup(const std::string& msg,
      uint64_t group,
      std::string contentType = "text/plain");

  /**
   * @brief Send any type of message to a group
   *
   * The message is stored in a buffer called msg.
   * @param msg is the buffer containing the message.
   * @param length is the length of the message.
   * @param group is the ID of the group.
   * @param contentType is the content type of the message.
   * "text/plain" by default.
   * @see sendMessage()
   * @see sendToGroup()
   * @see pollWrite()
   * @return True if successfully sent, false if not
   */
  virtual bool sendToGroup(char* msg,
      uint32_t length,
      uint64_t group,
      std::string contentType = "text/plain");

  virtual bool sendPls(DataMap &data);

  virtual void getMesgHist(int32_t count,
                           uint32_t timestamp,
			   uint64_t sourceId,
                           int32_t type);

  void setLogin(const std::string &login);
  void setPassword(const std::string &password);

  /**
   * @brief Gets contact details
   *
   * Gets contact details using the ID.
   * @param id is the ID of the contact.
   * @return An structure with contact details
   */
  int32_t getContact(uint64_t id, contact_t& contact);

  /**
   * @brief Gets group details
   *
   * Gets group details using the ID.
   * @param id is the ID of the group.
   * @return An structure with group details
   */
  int getGroup(uint64_t id, group_t& group);

  /**
   * @brief Get a copy of the list of groups
   */
  void getGroups(groups_t& groups);

  /**
   * @brief Set the Auto Accept Contacts configuration
   *
   * If you set it to true, it auto accepts any contact add command received
   * @param accept
   */
  void setAutoAcceptContacts(bool accept)
  {
    auto_accept_contacts_ = accept;
  }
  
  
  void groupSubscribe( std::string groupName );
  void groupCreate( std::string groupName, std::string groupDesc = "", std::string groupPassword = "" );
  void groupUnsubscribe( uint64_t groupID );

};
#endif
