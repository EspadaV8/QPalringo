/*
 * libPalringo/libPalringo.h
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



#include <string.h>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <queue>
#include <stdlib.h>

// Define DEBUG to show debug messages
//#define DEBUG


// If you need bigger buffer edit this constants
#define IN_BUFFER_SIZE 8192
#define OUT_BUFFER_SIZE 8192

typedef std::map <std::string, std::string> headers_t;

typedef std::set <unsigned long> group_contacts_t;

typedef struct {
    std::string name_;
    group_contacts_t contacts_;
} group_t;

typedef std::map <unsigned long, group_t> groups_t;

typedef struct {
    std::string name_;
    int onlineStatus_;
    std::string status_;
    int isContact_;
    int deviceType_;
} contact_t;

typedef std::map <unsigned long, contact_t> contacts_t;

typedef std::map <unsigned long, std::string> mesgs_t;

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
    DELIVERED,
    NOT_DELIVERED,
    SFE_NOT_AVAILABLE,
    STATS_IF_NOT_AVAILABLE,
    END_OF_MESG_STORED,
    UNABLE_TO_STORE_OFFLINE_MESG,
    RESEND_CURRENT_MESG,
    GROUP_ALREADY_EXISTS,
    CONTACT_ALREADY_EXISTS,
    NOT_ALLOWED,
    BRIDGING_NOT_AVAILABLE
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
    BRIDGING
};


class PalringoConnection
{
    protected:

        /**
            * @brief User login.
        */
        const std::string login_;

        /**
            * @brief User password.
        */
        const std::string password_;

        /**
            * @brief Server hostname.
        */
        const std::string host_;

        /**
            * @brief Server port.
        */
        unsigned int port_;

        /**
            * @brief Source IP
            *
            * The IP from which it connects to the server
        */
        const std::string sourceIP_;

        /**
            * @brief Socket file descriptor.
        */
        int fd_;

        /**
            * @brief Word Size as returned by the server in the AUTH command.
        */
        int wordSize_;

        /**
            * @brief User ID.
            *
            * This connections User ID, it's 0 until it's logged on.
        */
        unsigned long userId_;

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
            * @brief Push a message in the output buffer.
            *
            * It pushes a message in the output buffer to send it later.
            * @param msg points to the message to be sent.
            * @param length is the length of the message.
            * @see popOutMessage()
            * @return True if successfully pushed the value in the buffer and false if not.
        */
        bool pushOutMessage ( char *msg, unsigned int length );

        /**
            * @brief Pop a message from the output buffer.
            *
            * It pops a message in the output buffer and copies it in msg.
            * @param msg is a buffer where the function copies the message.
            * @param length is the length of the message.
            * @see pushOutMessage()
            * @return True if successfully popped the value out from the buffer and false if not.
        */
        bool popOutMessage ( char *msg, unsigned int *length );

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
        bool sendCmd ( const std::string& cmd,
                                    const headers_t& headers,
                                    const char *body,
                                    unsigned int length );

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
        bool sendCmd ( const std::string& cmd,
                                    const headers_t& headers,
                                    const std::string& body );

        /**
            * @brief Read a command from the socket assuming body is a string
            *
            * It reads a command from the file descriptor.
            * Body is of type string.
            * @param cmd is the command name.
            * @param headers is a map that includes all the headers for the command.
            * @param body is a string where the body of the message is stored.
            * @see pollRead()
            * @return -1 if an error occurred, 0 if the command wasn't fully read and 1 if it read a full command.
            */
        int recvCmd ( std::string& cmd,
                                    headers_t& headers,
                                    std::string& body );


        /**
            * @brief Read a command from socket, body can be a binary
            *
            * It reads a command from the file descriptor.
            * Body can be of binary type.
            * @param cmd is the command name.
            * @param headers is a map that includes all the headers for the command.
            * @param body is a pointer to the buffer where the body of the message is going to be stored.
            * @param lenght is the length of the message.
            * @see pollRead()
            * @return -1 if an error occurred, 0 if the command wasn't fully read and 1 if it read a full command.
            */
        int recvCmd ( std::string& cmd,
                                    headers_t& headers,
                                    const char *body,
                                    unsigned int length );

        /**
            * @brief On Logon Failed Event callback function
            *
            * This function is called if the log on process fails.
            * @param reason is the reason why the log on has failed.
            * @see onLogonSuccessful()
            * @see poll()
            */
        virtual void onLogonFailed ( std::string& reason );

        /**
            * @brief On Logon Succesful Event callback function
            *
            * This function is called if the log on process is successful.
            * @see onLogonFailed()
            * @see poll()
            */
        virtual void onLogonSuccessful();

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
        virtual void onMessageReceived ( std::string& msg,
                                                                            unsigned long contact,
                                                                            unsigned long group,
                                                                            const std::string& contentType );

        /**
            * @brief On Message Received Event callback function, string message
            *
            * This function is called if a message is received and is of binary type.
            * @param msg is a pointer to the buffer were the message is stored.
            * @param length is the length of the message.
            * @param contact is the id of the group that sent the message.
            * @param group is the id of the group that sent the message.
            * @param contentType is the content type of the message.
            * @see recvCmd()
            * @see pollRead()
            * @see poll()
            */
        virtual void onMessageReceived ( char *msg,
                                                                            unsigned int length,
                                                                            unsigned long contact,
                                                                            unsigned long group,
                                                                            const std::string& contentType );


        /**
            * @brief On Contact Add Received Event callback function
            *
            * This function is called if a CONTACT ADD command is received.
            * @param msg is the message sent by the adder.
            * @param contact is the user ID of the adder
            * @see pollRead()
            * @see poll()
            */
        virtual void onContactAddReceived ( std::string& msg,
                                                                                unsigned long contact );

        /**
            *@brief  On Ghosted Received Event callback function
            *
            * This function is called if the a GHOSTED command is received.
            * @see pollRead()
            * @see poll()
            */
        virtual void onGhostedReceived();

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
        virtual void onResponseReceived ( What what,
                                                                            unsigned long mesgId,
                                                                            ErrorCode error );

        /**
            * @brief Received data buffer (Input Buffer)
            * @see recvCmd()
            */
        char inBuffer_[IN_BUFFER_SIZE];

        /**
            * @brief Sent data buffer (Output Buffer)
            * @see pushOutMessage()
            * @see popOutMessage()
            */
        char outBuffer_[OUT_BUFFER_SIZE];

        /**
            * @brief Output buffer pop pointer difference
            * @see popOutMessage()
            */
        unsigned int outBufferBegin_;

        /**
            * @brief Output buffer push pointer difference
            * @see pushOutMessage()
            */
        unsigned int outBufferCurrent_;

        /**
            * @brief Messages in Output buffer
            * @see getOutMessageCount()
            * @see pushOutMessage()
            * @see popOutMessage()
            */
        unsigned int outBufferCount_;

        /**
            * @brief Bytes read sofar
            * @see recvCmd()
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
        int mesg_id_;

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
            * @see pollwrite()
            */
        bool sendMessage ( const std::string& msg,
                                            const std::string& contentType,
                                            unsigned long id,
                                            int type /* 0 for contact, 1 for group */ );

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
            * @see pollwrite()
            */
        bool sendMessage ( const char* msg,
                                            unsigned int length,
                                            const std::string& contentType,
                                            unsigned long id,
                                            int type /* 0 for contact, 1 for group */ );

    public:

        /**
            * @brief Constructor, creates the connection.
            *
            * @param host is the hostname of the server.
            * @param port is the port of the server.
            * @param login
            * @param password
            * @param sourceIP is the IP from which the connection is made
            * @return
            */
        PalringoConnection ( const std::string& host,
                                                    unsigned int port,
                                                    const std::string& login,
                                                    const std::string& password,
                                                    const std::string& sourceIP = "" );

        /**
            * @brief Object destructor
            */
        virtual ~PalringoConnection()
        {
        }

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
            */
        void reConnect();


        /**
            * @brief Get the userId of the bot, 0 if it's still not logged on
            * @return userId of the connection, 0 if not logged on
            */
        unsigned long getUserId();


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
        unsigned int getOutMessageCount();

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
            * @return -1 if an error occurred, 0 if there wasn't any command in the
            * output buffer to send, 1 if a command was successfully written.
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
        static int checkFd ( int max, fd_set &fdSetRead, fd_set &fdSetWrite );

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
            * @see pollwrite()
            * @return True if successfully sent, false if not
            */
        bool sendToContact ( const std::string& msg,
                                                    unsigned long contact,
                                                    const std::string contentType = "text/plain" );

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
            * @see pollwrite()
            * @return True if successfully sent, false if not
            */
        bool sendToContact ( const char* msg,
                                                    unsigned int length,
                                                    unsigned long contact,
                                                    const std::string contentType = "text/plain" );

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
            * @see pollwrite()
            * @return True if successfully sent, false if not
            */
        bool sendToGroup ( const std::string& msg,
                                            unsigned long group,
                                            const std::string contentType = "text/plain" );

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
            * @see pollwrite()
            * @return True if successfully sent, false if not
            */
        bool sendToGroup ( const char* msg,
                                            unsigned int length,
                                            unsigned long group,
                                            const std::string contentType = "text/plain" );

        /**
            * @brief Gets contact details
            *
            * Gets contact details using the ID.
            * @param id is the ID of the contact.
            * @return An structure with contact details
            */
        const contact_t& getContact ( unsigned long id )
        {
                return contacts_[id];
        }

        /**
            * @brief Gets group details
            *
            * Gets group details using the ID.
            * @param id is the ID of the group.
            * @return An structure with group details
            */
        const group_t& getGroup ( unsigned long id )
        {
                return groups_[id];
        }

        /**
            * @brief Set the Auto Accept Contacts configuration
            *
            * If you set it to true, it auto accepts any contact add command received
            * @param accept
            */
        void setAutoAcceptContacts ( bool accept )
        {
                auto_accept_contacts_ = accept;
        }

};

