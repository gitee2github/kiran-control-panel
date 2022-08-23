/**
 * Copyright (c) 2022 KylinSec Co., Ltd.
 * kiran-cpanel-network is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     luoqing <luoqing@kylinos.com.cn>
 */

#include "ipv6-widget.h"
#include <kiran-message-box.h>
#include <qt5-log-i.h>
#include "ui_ipv6-widget.h"
using namespace NetworkManager;

Ipv6Widget::Ipv6Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Ipv6Widget)
{
    ui->setupUi(this);
    initUI();
    initConnection();
}

Ipv6Widget::~Ipv6Widget()
{
    delete ui;
}

void Ipv6Widget::initUI()
{
    ui->ipv6Method->addItem(tr("Auto"), Ipv6Setting::ConfigMethod::Automatic);
    ui->ipv6Method->addItem(tr("Manual"), Ipv6Setting::ConfigMethod::Manual);
    ui->ipv6Method->addItem(tr("Ignored"), Ipv6Setting::ConfigMethod::Ignored);
    ui->ipv6Manual->setVisible(false);
    ui->ipv6Address->setPlaceholderText(tr("Required"));
    ui->ipv6Prefix->setMaximum(128);
    ui->ipv6Prefix->setMinimum(1);
}

void Ipv6Widget::initConnection()
{
    connect(ui->ipv6Method, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index)
            { handleIpv6MethodChanged(ui->ipv6Method->currentData().value<NetworkManager::Ipv6Setting::ConfigMethod>()); });
}

void Ipv6Widget::setIpv6Setting(const Ipv6Setting::Ptr &ipv6Setting)
{
    m_ipv6Setting = ipv6Setting;
}

void Ipv6Widget::handleIpv6MethodChanged(NetworkManager::Ipv6Setting::ConfigMethod method)
{
    switch (method)
    {
    case Ipv6Setting::ConfigMethod::Automatic:
        ui->ipv6Manual->setVisible(false);
        KLOG_DEBUG() << "Automatic";
        break;
    case Ipv6Setting::ConfigMethod::Manual:
        ui->ipv6Manual->setVisible(true);
        KLOG_DEBUG() << "Manual";
        break;
    case Ipv6Setting::ConfigMethod::Ignored:
        ui->ipv6Manual->setVisible(false);
        KLOG_DEBUG() << "Ignored";
        break;
    default:
        break;
    }
}
void Ipv6Widget::saveSettings()
{
    if (m_ipv6Setting != nullptr)
    {
        Ipv6Setting::ConfigMethod method = ui->ipv6Method->currentData().value<NetworkManager::Ipv6Setting::ConfigMethod>();
        if (method == Ipv6Setting::ConfigMethod::Ignored)
        {
            m_ipv6Setting->setMethod(method);
            m_ipv6Setting->setAddresses(QList<NetworkManager::IpAddress>());
        }
        else if (method == Ipv6Setting::ConfigMethod::Automatic)
        {
            m_ipv6Setting->setMethod(method);

            NetworkManager::IpAddress ipAddressAuto;
            ipAddressAuto.setIp(QHostAddress(""));
            ipAddressAuto.setPrefixLength(0);
            ipAddressAuto.setGateway(QHostAddress(""));
            m_ipv6Setting->setAddresses(QList<NetworkManager::IpAddress>() << ipAddressAuto);
        }
        else if (method == Ipv6Setting::ConfigMethod::Manual)
        {
            m_ipv6Setting->setMethod(method);

            IpAddress address;
            address.setIp(QHostAddress(ui->ipv6Address->text()));
            address.setPrefixLength(ui->ipv6Prefix->value());
            address.setGateway(QHostAddress(ui->ipv6Gateway->text()));

            QList<IpAddress> addresseList;
            addresseList << address;
            m_ipv6Setting->setAddresses(addresseList);

            QList<QHostAddress> ipv6DNS;
            ipv6DNS << QHostAddress(ui->ipv6PreferredDNS->text()) << QHostAddress(ui->ipv6AlternateDNS->text());
            m_ipv6Setting->setDns(ipv6DNS);
        }
    }
}

void Ipv6Widget::showSettings()
{
    if (m_ipv6Setting != nullptr)
    {
        if (m_ipv6Setting->method() == Ipv6Setting::ConfigMethod::Ignored)
        {
            int ipv6MethodIndex = ui->ipv6Method->findData(Ipv6Setting::ConfigMethod::Ignored);
            ui->ipv6Method->setCurrentIndex(ipv6MethodIndex);
        }
        else if (m_ipv6Setting->method() == Ipv6Setting::ConfigMethod::Automatic)
        {
            int ipv6MethodIndex = ui->ipv6Method->findData(Ipv6Setting::ConfigMethod::Automatic);
            ui->ipv6Method->setCurrentIndex(ipv6MethodIndex);
        }
        else if (m_ipv6Setting->method() == Ipv6Setting::ConfigMethod::Manual)
        {
            int ipv6MethodIndex = ui->ipv6Method->findData(m_ipv6Setting->method());
            ui->ipv6Method->setCurrentIndex(ipv6MethodIndex);

            // xxx:取addresses的方式有待改进
            IpAddress ipv6Address = m_ipv6Setting->addresses().at(0);
            QString ip = ipv6Address.ip().toString();
            int prefix = ipv6Address.prefixLength();
            QString gateway = ipv6Address.gateway().toString();

            ui->ipv6Address->setText(ip);
            ui->ipv6Prefix->setValue(prefix);
            ui->ipv6Gateway->setText(gateway);
        }

        QString preferredDNS = "";
        QString alternateDNS = "";
        if (!m_ipv6Setting->dns().isEmpty())
        {
            preferredDNS = m_ipv6Setting->dns().at(0).toString();
            if (m_ipv6Setting->dns().count() >= 2)
            {
                alternateDNS = m_ipv6Setting->dns().at(1).toString();
            }
        }
        ui->ipv6PreferredDNS->setText(preferredDNS);
        ui->ipv6AlternateDNS->setText(alternateDNS);
    }
    else
        resetSettings();
}

void Ipv6Widget::resetSettings()
{
    int ipv6MethodIndex = ui->ipv6Method->findData(Ipv6Setting::ConfigMethod::Automatic);
    ui->ipv6Method->setCurrentIndex(ipv6MethodIndex);
    ui->ipv6Prefix->setValue(64);
    ui->ipv6Address->clear();
    ui->ipv6Gateway->clear();
    ui->ipv6PreferredDNS->clear();
    ui->ipv6AlternateDNS->clear();
}

void Ipv6Widget::clearPtr()
{
    m_ipv6Setting.clear();
}

// TODO:不完善，待优化
bool Ipv6Widget::isInputValid()
{
    Ipv6Setting::ConfigMethod configMethod = ui->ipv6Method->currentData().value<Ipv6Setting::ConfigMethod>();

    if (configMethod == Ipv6Setting::ConfigMethod::Ignored)
    {
    }
    else if (configMethod == Ipv6Setting::ConfigMethod::Automatic)
    {
    }
    else if (configMethod == Ipv6Setting::ConfigMethod::Manual)
    {
        QString ipv6 = ui->ipv6Address->text();
        if (ipv6.isEmpty())
        {
            QString error = QString(tr("Ipv6 address can not be empty"));
            KiranMessageBox::KiranStandardButton btn = KiranMessageBox::message(this, tr("Error"),
                                                                                error,
                                                                                KiranMessageBox::Yes | KiranMessageBox::No);

            KLOG_DEBUG() << "Ipv6 Address cannot be empty";
            return false;
        }
        else
        {
            if (!isIpv6AddressValid(ipv6))
            {
                QString error = QString(tr("Ipv6 address invalid"));
                KiranMessageBox::KiranStandardButton btn = KiranMessageBox::message(this, tr("Error"),
                                                                                    error,
                                                                                    KiranMessageBox::Yes | KiranMessageBox::No);
                KLOG_DEBUG() << "Ipv6Address invalid";
                return false;
            }
        }

        QString ipv6Gateway = ui->ipv6Gateway->text();
        if (!ipv6Gateway.isEmpty())
        {
            if (!isIpv6AddressValid(ipv6Gateway))
            {
                QString error = QString(tr("Ipv6 Gateway invalid"));
                KiranMessageBox::KiranStandardButton btn = KiranMessageBox::message(this, tr("Error"),
                                                                                    error,
                                                                                    KiranMessageBox::Yes | KiranMessageBox::No);
                KLOG_DEBUG() << "Ipv6 Netmask invalid";
                return false;
            }
        }
    }

    QString preferredDNS = ui->ipv6PreferredDNS->text();
    if (!preferredDNS.isEmpty())
    {
        if (!isIpv6AddressValid(preferredDNS))
        {
            QString error = QString(tr("Ipv6 Preferred DNS invalid"));
            KiranMessageBox::KiranStandardButton btn = KiranMessageBox::message(this, tr("Error"),
                                                                                error,
                                                                                KiranMessageBox::Yes | KiranMessageBox::No);
            KLOG_DEBUG() << "Ipv6 Preferred DNS invalid";
            return false;
        }
    }

    QString alternateDNS = ui->ipv6AlternateDNS->text();
    if (!alternateDNS.isEmpty())
    {
        if (!isIpv6AddressValid(alternateDNS))
        {
            QString error = QString(tr("Ipv6 Alternate DNS invalid"));
            KiranMessageBox::KiranStandardButton btn = KiranMessageBox::message(this, tr("Error"),
                                                                                error,
                                                                                KiranMessageBox::Yes | KiranMessageBox::No);
            KLOG_DEBUG() << "Ipv6 Alternate DNS invalid";
            return false;
        }
    }

    return true;
}

bool Ipv6Widget::isIpv6AddressValid(const QString &address)
{
    QHostAddress ipAddr(address);
    if (ipAddr == QHostAddress(QHostAddress::Null) || ipAddr == QHostAddress(QHostAddress::AnyIPv6) || ipAddr.protocol() != QAbstractSocket::NetworkLayerProtocol::IPv6Protocol)
    {
        return false;
    }
    if (ipAddr == QHostAddress(QHostAddress::LocalHostIPv6))
    {
        return false;
    }
    return true;
}
