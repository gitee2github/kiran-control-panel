#ifndef PASSWDHELPER_H
#define PASSWDHELPER_H

class QString;
namespace PasswdHelper{
    bool encryptPassword( const QString &pwd, QString &encrypted);
    bool checkUserPassword( const QString &user, const QString &pwd);
}

#endif // PASSWDHELPER_H
