#ifndef USER_H
#define USER_H

struct User
{
    quint64 userID;
    QString nickname;
    QString status;
    QString lastOnline;
    QString email;
    QString password;
};

#endif // USER_H
