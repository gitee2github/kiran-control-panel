//
// Created by lxh on 2020/11/5.
//

#ifndef KIRAN_ACCOUNT_MANAGER_HARD_WORKER_H
#define KIRAN_ACCOUNT_MANAGER_HARD_WORKER_H

#include <QObject>

class HardWorker : public QObject
{
    Q_OBJECT
public:
    explicit HardWorker();

    ~HardWorker();

public slots:

    /// 通过AccountServices创建用户
    /// \param account          账户
    /// \param uid              UID
    /// \param accountType      账户类型
    /// \param encryptedPasswd  加密之后的密码
    /// \param homeDir          用户家目录
    /// \param shell            shell
    /// \param iconFile         头像
    void doCreateUser(QString account,
                      int uid,
                      int accountType,
                      QString encryptedPasswd,
                      QString homeDir,
                      QString shell,
                      QString iconFile);

    /// 通过AccountServices更新密码
    /// \param account          账户名
    /// \param encryptedPasswd  加密之后的密码
    void doUpdatePasswd(QString objPath,
                        QString account,
                        QString encryptedPasswd);

    /// 通过AccountServices更新用户属性
    /// \param account      账户名
    /// \param iconfile     图标
    /// \param accountType  账户类型
    /// \param isLocked     是否锁定
    void doUpdateUserProperty(QString objPath,
                              QString account,
                              QString iconfile,
                              int accountType,
                              bool isLocked);

    /// 通过AccountService删除用户
    /// \param uid  删除用户的UID
    void doDeleteUser(int uid);

signals:

    /// 创建用户完成信号
    /// \param userPath 创建完成的用户DBus对象路径,若创建用户都失败，则为空
    /// \param errMsg   错误消息框，收到弹出提示框
    void sigCreateUserDnoe(QString userPath, QString errMsg);

    /// 更新用户密码完成信号
    /// \param errMsg 错误消息，不为空表示失败，弹出提示框
    void sigUpdatePasswdDone(QString errMsg);

    /// 更新用户属性完成信号
    /// \param errMsg　错误消息，不为空表示失败，弹出提示框
    void sigUpdateUserPropertyDone(QString errMsg);

    /// 删除用户完成信号
    /// \param errMsg 错误消息，不为空表示失败，弹出提示框
    void sigDeleteUserDone(QString errMsg);
};

#endif  //KIRAN_ACCOUNT_MANAGER_HARD_WORKER_H
