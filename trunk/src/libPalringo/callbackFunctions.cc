#include "libPalringo.h"
#include "DataMap.h"
#include "crypto.h"

#ifdef DEBUG
#define DBGOUT std::cout
#else
#define DBGOUT if (0) std::cout
#endif

int
PalringoConnection::onPingReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Pong!" << std::endl;
  headers.clear();
  if (protocolVersion_ == 2)
  {
    headers["PS"] = toString(packetSeq_);
  }
  sendCmd("P", headers, "");

  return 1;
}

int
PalringoConnection::onAuthReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  AuthData authData;
  AuthData *authDataPtr =
    getDataPtr<PalringoConnection::AuthData>(
      data,
      &authData);

  authDataPtr->getData(headers, body);

  if (authDataPtr->wordSize_)
  {
    // Save the word-size, we need it for later
    wordSize_ = authDataPtr->wordSize_;
  }

  connectionStatus_ = CONN_AUTHENTICATION;

  // Let's send our login and password
  headers.clear();
  AuthData outAuthData;
  std::string newBody;
  if (authDataPtr->encryptionType_ > -1)
  {
    outAuthData.encryptionType_ = authDataPtr->encryptionType_;
    outAuthData.onlineStatus_ = ONLINE;

    // No Encryption
    if (authDataPtr->encryptionType_ == 0)
    {
      outAuthData.name_ = login_;
      newBody = password_;
      if (encryption_)
      {
	encryption_ = false;
      }
    }

    // Salsa20/MD5
    else if (authDataPtr->encryptionType_ == 1)
    {
      std::string challenge(body.substr(0, 16));
      std::cout << "Challenge:\n" << hexDump(challenge) << std::endl;
      std::string IV(body.substr(16, 8));
      std::cout << "IV:\n" << hexDump(IV) << std::endl;
      std::string keyBundle(passwordMD5_ + IV);
      std::string keyStr(crypto::md5(keyBundle));
      std::cout << "passwordMD5_:\n" << hexDump(passwordMD5_) << std::endl;
      std::cout << "passwordMD5_ + IV:\n" << hexDump(keyBundle) << std::endl;
      std::cout << "keyStr:\n" << hexDump(keyStr) << std::endl;

      crypto::SalsaCipher salsa(IV, keyStr);
      uint32_t randomness[4];
      randomness[0] = rand();
      randomness[1] = rand();
      randomness[2] = rand();
      randomness[3] = rand();
      std::string randStr(reinterpret_cast<const char*>(randomness), 16);
      std::string beforeEncrypt(challenge + randStr);
      salsa.encrypt(beforeEncrypt, newBody);

      std::cout << "Plain Body:\n" << hexDump(beforeEncrypt) << std::endl;
      std::cout << "Encrypted Body:\n" << hexDump(newBody) << std::endl;

      std::string newKey(crypto::md5(passwordMD5_ + randStr));
      std::cout << "newKey:\n" << hexDump(newKey) << std::endl;

      delete salsa_;
      salsa_ = new crypto::SalsaCipher(IV, newKey);
    }

    // Salsa20/oldpassword
    else if (authDataPtr->encryptionType_ == 2)
    {
      std::string challenge(body.substr(0, 16));
      std::cout << "Challenge:\n" << hexDump(challenge) << std::endl;
      std::string IV(body.substr(16, 8));
      std::cout << "IV:\n" << hexDump(IV) << std::endl;
      std::string oldPass(crypto::oldPassword(password_));
      std::string keyBundle(oldPass + IV);
      std::string keyStr(crypto::md5(keyBundle));
      std::cout << "oldPass:\n" << hexDump(oldPass) << std::endl;
      std::cout << "oldPass + IV:\n" << hexDump(keyBundle) << std::endl;
      std::cout << "keyStr:\n" << hexDump(keyStr) << std::endl;

      crypto::SalsaCipher salsa(IV, keyStr);
      uint32_t randomness[4];
      randomness[0] = rand();
      randomness[1] = rand();
      randomness[2] = rand();
      randomness[3] = rand();
      std::string randStr(reinterpret_cast<const char*>(randomness), 16);
      std::string beforeEncrypt(challenge +
			       randStr +
			       password_ +
			       std::string(50 - password_.size(), '\0'));
      salsa.encrypt(beforeEncrypt, newBody);
      std::cout << "Plain Body:\n" << hexDump(beforeEncrypt) << std::endl;
      std::cout << "Encrypted Body:\n" << hexDump(newBody) << std::endl;

      std::string newKey(crypto::md5(passwordMD5_ + randStr));
      std::cout << "newKey:\n" << hexDump(newKey) << std::endl;

      delete salsa_;
      salsa_ = new crypto::SalsaCipher(IV, newKey);
    }

    outAuthData.setData(headers, newBody);
  }

  // Reconnection
  else
  {
    std::string reconnectChallenge(RK_);
    reconnectChallenge.append(body);
    newBody = crypto::md5(reconnectChallenge);
    char tmp[64];
    sprintf(tmp, "%lu", --packetSeq_);
    headers["PS"] = tmp;
    loggedOn_ = true;
    receivedData_ = 0;
  }


  sendCmd ("AUTH", headers, newBody);
  return 1;
}

int
PalringoConnection::onLogonSuccessfulReceived(headers_t& headers,
  std::string& body,
  GenericData *data)
{
  LogonData logonData;
  LogonData *logonDataPtr =
    getDataPtr<PalringoConnection::LogonData>(data, &logonData);

  logonDataPtr->getData(headers, body);

  connectionStatus_ = CONN_READY;

  userId_ = logonDataPtr->subId_;
  nickname_ = logonDataPtr->nickname_;
  status_ = logonDataPtr->status_;
  lastOnline_ = logonDataPtr->lastOnline_;
  DBGOUT << "Logon successful" << std::endl;
  loggedOn_ = true;
  return 1;
}

int
PalringoConnection::onLogonFailedReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  LogonData logonData;
  LogonData *logonDataPtr = getDataPtr<PalringoConnection::LogonData>(data, &logonData);

  logonDataPtr->getData(headers, body);

  disconnectClient();

  DBGOUT << "Logon failed: " << logonDataPtr->reason_ << std::endl;
  return 1;
}

int
PalringoConnection::onGhostedReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Setting bot with Id " << userId_ << " as GHOSTED" << std::endl;
  ghosted_ = true;
  return 1;
}

int
PalringoConnection::onContactDetailReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  ContactData contactData;
  ContactData *contactDataPtr = getDataPtr<PalringoConnection::ContactData>(data, &contactData);

  contactDataPtr->getData(headers, body);

  if (connectionStatus_ == CONN_AUTHENTICATION)
  {
    connectionStatus_ = CONN_GETTING_DATA;
  }

  contact_t& contact = contacts_[contactDataPtr->contactId_];

  if (contactDataPtr->nickname_.size())
  {
    contact.nickname_ = contactDataPtr->nickname_;
  }

  if (contactDataPtr->onlineStatus_ > -1)
  {
    contact.onlineStatus_ = contactDataPtr->onlineStatus_;
  }

  if (contactDataPtr->status_.size())
  {
    contact.status_ = contactDataPtr->status_;
  }

  if (contactDataPtr->deviceType_)
  {
    contact.deviceType_ = contactDataPtr->deviceType_;
  }

  if (contactDataPtr->isContact_)
  {
      contact.isContact_ = contactDataPtr->isContact_;
  }
  return 1;
}

int
PalringoConnection::onContactAddReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  ContactData contactData;
  ContactData *contactDataPtr = getDataPtr<PalringoConnection::ContactData>(data, &contactData);

  contactDataPtr->getData(headers, body);

  headers.clear();
  headers["ACCEPTED"] = "1";
  char tmp1[32];
  sprintf(tmp1, "%lu", contactDataPtr->sourceId_);
  headers["SOURCE-ID"] = tmp1;
  char tmp2[32];
  sprintf(tmp2, "%d", ++mesg_id_);
  headers["MESG-ID"] = tmp2;

  if (auto_accept_contacts_)
  {
    sendCmd("CONTACT ADD RESP", headers, "");
  }

  DBGOUT << "Contact add request " << contactDataPtr->sourceId_
	 << " - " << body << std::endl;
  return 1;
}

int
PalringoConnection::onGroupDetailReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  GroupData groupData;
  GroupData *groupDataPtr = getDataPtr<PalringoConnection::GroupData>(data,
      &groupData);

  groupDataPtr->getData(headers, body);

  if (connectionStatus_ == CONN_AUTHENTICATION)
  {
    connectionStatus_ = CONN_GETTING_DATA;
  }

  group_t& group = groups_[groupDataPtr->groupId_];

  if (groupDataPtr->name_.size())
  {
    group.name_ = groupDataPtr->name_;
  }

  for (size_t i = 0; i<body.size()/8; ++i)
  {
    group.contacts_.insert( ntohll( *( ( uint64_t* ) ( body.c_str() + 8 * i) ) ) );
  }
  return 1;
}

int
PalringoConnection::onGroupUpdateReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  GroupData groupData;
  GroupData *groupDataPtr = getDataPtr<PalringoConnection::GroupData>(data,
      &groupData);

  groupDataPtr->getData(headers, body);

  group_t& group = groups_[groupDataPtr->groupId_];

  if (groupDataPtr->name_.size())
  {
    group.name_ = groupDataPtr->name_;
  }
  else
  {
    if (groupDataPtr->type_)
    {
      group.contacts_.erase(groupDataPtr->contactId_);
    }
    else
    {
      group.contacts_.insert(groupDataPtr->contactId_);
    }
  }
  return 1;
}

int
PalringoConnection::onGroupUpdateReceivedV2(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Group Update V2 received" << std::endl;

  DataMap dataMap(body);

  if(!dataMap.parse())
  {
    DBGOUT << "Parse error while processing DataMap" << std::endl;
    return 0;
  }
  else
  {
    DBGOUT << "Processing DataMap" << std::endl;
    unsigned long groupId(strtoul(dataMap["Group-Id"].c_str(), NULL, 10));
    if (dataMap.hasAttribute("Contact-Id"))
    {
      DBGOUT << "Contact Update for Group received" << std::endl;
      unsigned long contactId(strtoul(dataMap["Contact-Id"].c_str(), NULL, 10));
      unsigned int type(strtoul(dataMap["Type"].c_str(), NULL, 10));

      if(!type)
      {
	DBGOUT << "Contact " << contactId
	       << " has joined the group" << std::endl;
	contact_t& contact =  contacts_[contactId];

	contact.nickname_ = dataMap["Nickname"];

	contact.deviceType_ = static_cast<DeviceType>(
	    strtoul(dataMap["Device-Type"].c_str(), NULL, 10));

	contact.onlineStatus_ = static_cast<OnlineStatus>(
	    strtoul(dataMap["Online-Status"].c_str(), NULL, 10));

	contact.status_ = dataMap["Status"];
	group_t& group = groups_[groupId];
	group.contacts_.insert(contactId);
      }

      else
      {
	DBGOUT << "Contact " << contactId
	       << " has left the group" << std::endl;
	group_t& group = groups_[groupId];
	group.contacts_.erase(contactId);
      }
    }

    else if(dataMap.hasAttribute("Name"))
    {
      DBGOUT << "Group name change" << std::endl;
      group_t& group =  groups_[groupId];
      group.name_ = dataMap["Name"];
    }

    else if(dataMap.hasAttribute("Desc"))
    {
      DBGOUT << "Group description change" << std::endl;
      group_t& group =  groups_[groupId];
      group.desc_ = dataMap["Desc"];
    }

    return 1;
  }
}

int
PalringoConnection::onRegReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  headers_t newHeaders;
  newHeaders["email"] = login_;
  newHeaders["MESG-ID"] = "1";

  DataMap dataMap(body);
  dataMap.parse();
  DBGOUT << "dataMap: \n" << dataMap.toString() << std::endl;
  std::string IV(dataMap["IV"]);
  std::string B(dataMap["B"]);
  std::string s(dh_->getSecret(B));
  std::string sMD5(crypto::md5(s));
  crypto::SalsaCipher salsa(IV, sMD5);
  DBGOUT << "s: " << s << "\nsMD5: " << hexDump(sMD5) << std::endl;

  std::string newBody;
  std::string clearText(password_ + std::string(50 - password_.size(), '\0'));
  DBGOUT << "clearText:\n" << hexDump(clearText) << std::endl;

  salsa.encrypt(clearText, newBody);
  DBGOUT << "newBody:\n" << hexDump(newBody) << std::endl;

  sendCmd(pCommand::REG, newHeaders, newBody);

  delete dh_;
  dh_ = NULL;
  return 1;
}

int
PalringoConnection::onResponseReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  ResponseData responseData;
  ResponseData *responseDataPtr = getDataPtr<PalringoConnection::ResponseData>(data,
      &responseData);

  responseDataPtr->getData(headers, body);

  DBGOUT << "Response Message:" << std::endl
    << "\tWhat: " << whatStrings[responseDataPtr->what_] << std::endl
    << "\tMesg-Id: " << responseDataPtr->mesgId_ << std::endl
    << "\tType: " << responseDataPtr->type_ << std::endl;
  if(!responseDataPtr->type_)
  {
    DBGOUT << "\tError Code: "
      << errorCodes[responseDataPtr->errorCode_] << std::endl;
  }
  else
  {
    DBGOUT << "\tError Message: "
      << responseDataPtr->errorMessage_ << std::endl;
  }
  return 1;
}

int
PalringoConnection::onMesgReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  MsgData msgData;
  MsgData *msgDataPtr = getDataPtr<PalringoConnection::MsgData>(data,
      &msgData);

  msgDataPtr->getData(headers, body);

  if (!msgDataPtr->last_)
  {
    DBGOUT << "Must wait for more..." << std::endl;
    if (msgDataPtr->correlationId_ > 0)
    {
      mesgs_[msgDataPtr->correlationId_] += body; // Continuation of
    }					         // existing message

    else
    {
      mesgs_[msgDataPtr->mesgId_] = body; // First part of new message
    }
    return 0;
  } // end if

  if ( msgDataPtr->correlationId_ > 0 )
  {
    mesgs_t::iterator it = mesgs_.find(msgDataPtr->correlationId_);
    if (it != mesgs_.end())
    {
      // Final part of existing message
      body = it->second + body;
      mesgs_.erase(it);
      DBGOUT << "Last in series" << std::endl;
    }
    else
    {
      DBGOUT << "Unknown correlation ID!" << std::endl;
    }
  }
/*
  DBGOUT << body << " "
    << msgDataPtr->sourceId_ << " "
    << msgDataPtr->targetId_ << std::endl;
    */

  return 1;
}

int
PalringoConnection::onSessionEndReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  return 1;
}

int
PalringoConnection::onSubProfileReceived(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "SUB-PROFILE received" << std::endl;

  if(body.size())
  {
    std::string encRK;
    std::string msgBody;
    std::string IV;
    if (encryption_)
    {
      std::cout << "Body:\n" << hexDump(body) << std::endl;
      size_t RKsize = strtoul(headers["RK"].c_str(), NULL, 10);
      size_t IVsize = strtoul(headers["IV"].c_str(), NULL, 10);
      std::cout << "IV = " << headers["IV"] << "\nIVsize: " << IVsize << std::endl;
      encRK.append(body.substr(body.size() - RKsize, RKsize));
      msgBody.append(body.substr(IVsize, body.size() - RKsize - IVsize));
      IV.append(body.substr(0, IVsize));
      std::cout << "IV:\n" << hexDump(IV) << std::endl;
      salsa_->setIV(IV);
      salsa_->encrypt(encRK, RK_);
      std::cout << "encRK:\n" << hexDump(encRK) << std::endl;
      std::cout << "RK:\n" << hexDump(RK_) << std::endl;
      std::string keyStr(salsa_->getKey());
      std::cout << "Key:\n" << hexDump(keyStr) << std::endl;
    }

    LogonData logonData;
    LogonData *logonDataPtr =
      getDataPtr<PalringoConnection::LogonData>(data, &logonData);

    logonDataPtr->getData(headers, encryption_ ? msgBody : body);

    if(!logonDataPtr->dataMap_->parse())
    {
      std::cout << "Wrong datamap!\n"
		<< hexDump(encryption_ ? msgBody : body)
		<< std::endl;
      return 0;
    }
    else
    {
      DBGOUT << "Processing DataMap" << std::endl;
      std::string dataMapStr(logonDataPtr->dataMap_->toString());
      DBGOUT << "dataMap: \n" << dataMapStr << std::endl;


      connectionStatus_ = CONN_READY;

      DataMap& dataMap = *(logonDataPtr->dataMap_);

      userId_ = strtoul(dataMap["Sub-Id"].c_str(), NULL, 10);
      nickname_ = dataMap["Nickname"];
      status_ = dataMap["Status"];
      lastOnline_ = logonDataPtr->lastOnline_;
      if (!encryption_)
      {
	headers_t::iterator rkIt(headers.find("RK"));

	if (rkIt != headers.end())
	{
	  RK_ = rkIt->second;
	}
      }
      DBGOUT << "Logon successful" << std::endl;
      loggedOn_ = true;

      DataMap contactAddDataMap(dataMap["CONTACT_ADD"]);
      contactAddDataMap.parse();

      for(DataMap::ValueMap::iterator it = contactAddDataMap.begin();
	  it != contactAddDataMap.end();
	  it++)
      {
	headers_t cHeaders;
	cHeaders["ACCEPTED"] = "1";
	cHeaders["SOURCE-ID"] = it->first;
	char tmp2[32];
	sprintf(tmp2, "%d", ++mesg_id_);
	cHeaders["MESG-ID"] = tmp2;

	if (auto_accept_contacts_)
	{
	  sendCmd("CONTACT ADD RESP", cHeaders, "");
	}

	DBGOUT << "Contact add request " << it->first << std::endl;
      }

      DataMap contactsDataMap(dataMap["contacts"]);
      contactsDataMap.parse();
      for (DataMap::ValueMap::iterator it = contactsDataMap.begin();
	  it != contactsDataMap.end();
	  it++)
      {
	DBGOUT << "Processing DataMap of contact: " << it->first << std::endl;
	contact_t& contact =  contacts_[strtoul(it->first.c_str(), NULL, 10)];
	DataMap contactDataMap(contactsDataMap[it]);
	contactDataMap.parse();

	if (contactDataMap.hasAttribute("Nickname"))
	{
	  contact.nickname_ = contactDataMap["Nickname"];
	}

	if (contactDataMap.hasAttribute("Online-Status"))
	{
	  contact.onlineStatus_ = strtoul(contactDataMap["Online-Status"].c_str(), NULL, 10);
	}

	if (contactDataMap.hasAttribute("Status"))
	{
	  contact.status_ = contactDataMap["Status"];
	}

	if (contactDataMap.hasAttribute("Status"))
	{
	  contact.deviceType_ = static_cast<DeviceType>(strtoul(contactDataMap["Device-Type"].c_str(), NULL, 10));
	}

	if (contactDataMap.hasAttribute("contact"))
	{
	  contact.isContact_ = strtoul(contactDataMap["contact"].c_str(), NULL, 10);
	}
      }

      DataMap groupsDataMap(dataMap["group_sub"]);
      groupsDataMap.parse();

      for (DataMap::ValueMap::iterator it = groupsDataMap.begin();
	  it != groupsDataMap.end();
	  it++)
      {
	group_t& group = groups_[strtoul(it->first.c_str(), NULL, 10)];
	DataMap groupDataMap(groupsDataMap[it]);
	groupDataMap.parse();

	group.name_ =  groupDataMap["name"];
	group.desc_ = groupDataMap["desc"];

	for (DataMap::ValueMap::iterator itB = groupDataMap.begin();
	     itB != groupDataMap.end();
	     itB++)
	{
	  unsigned long contactId(0);
	  if ((contactId = strtoul(itB->first.c_str(), NULL, 10)))
	  {
	    group.contacts_.insert(contactId);
	  }
	}
      }
      return 1;
    }
  }

  else
  {
    DBGOUT << "Empty SUB PROFILE, Logon Successful!" << std::endl;
    connectionStatus_ = CONN_READY;
  }
  return 0;
}

int
PalringoConnection::onPingSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Ping sent" << std::endl;
  return 1;
}

int
PalringoConnection::onLogonSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Logon sent" << std::endl;
  return 1;
}

int
PalringoConnection::onByeSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Bye sent" << std::endl;
  RK_.clear();
  poll();
  return 1;
}

int
PalringoConnection::onAuthSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Auth sent" << std::endl;
  return 1;
}

int
PalringoConnection::onContactUpdateSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Contact Update sent" << std::endl;
  return 1;
}

int
PalringoConnection::onContactAddRespSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Contact Add sent" << std::endl;
  return 1;
}

int
PalringoConnection::onGroupSubscribeSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Group Subscribe sent" << std::endl;
  return 1;
}

int
PalringoConnection::onGroupUnsubSent(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Group Unsub sent" << std::endl;
  return 1;
}
int
PalringoConnection::onGroupCreateSent(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Group Create sent" << std::endl;
  return 1;
}

int
PalringoConnection::onGroupInviteSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Group Invite sent" << std::endl;
  return 1;
}

int
PalringoConnection::onGroupAdminSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Group Admin sent" << std::endl;
  return 1;
}

int
PalringoConnection::onMesgSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  MsgData msgData;
  MsgData *msgDataPtr = getDataPtr<PalringoConnection::MsgData>(data,
      &msgData);

  msgDataPtr->getData(headers, body);

  if (msgDataPtr->last_)
  {
    DBGOUT << "Message sent" << std::endl;
  }
  return 1;
}

int
PalringoConnection::onMesgStoredSent(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "Message Stored sent" << std::endl;
  return 1;
}

int
PalringoConnection::onMesgHistSent(headers_t& headers,
   std::string& body,
   GenericData *data)
{
  DBGOUT << "Message History sent" << std::endl;
  return 1;
}


int
PalringoConnection::onRegSent(headers_t& headers,
    std::string& body,
    GenericData *data)
{
  DBGOUT << "REG Sent" << std::endl;
  return 1;
}
