/**
 * Copyright (c) 2022 KylinSec Co., Ltd.
 * kiran-control-panel is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     luoqing <luoqing@kylinsec.com.cn>
 */
// clang-format off
#include <qt5-log-i.h>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>
#include <NetworkManagerQt/WiredDevice>
#include <NetworkManagerQt/WirelessSetting>
#include "status-notification.h"
#include "connection-list.h"
#include "connection-show-page.h"
#include "tray-widget.h"
// clang-format on
using namespace NetworkManager;
#define TRAY_ITEM_NORAML_HIEGHT 50

TrayWidget::TrayWidget(QWidget *parent) : QWidget(parent)

{
    init();
}

TrayWidget::~TrayWidget()
{
}

void TrayWidget::init()
{
    initUI();
    initConnection();
    m_StateActivatedTimer.setInterval(100);
    m_StateActivatedTimer.setSingleShot(true);
}

void TrayWidget::initUI()
{
    setFixedWidth(240);
    setContentsMargins(0, 0, 0, 0);
    m_verticalLayout = new QVBoxLayout(this);
    m_verticalLayout->setSpacing(0);
    m_verticalLayout->setContentsMargins(0, 0, 0, 0);
}

void TrayWidget::addWidget(QWidget *widget)
{
    m_verticalLayout->addWidget(widget);
}

void TrayWidget::removeWidget(QWidget *widget)
{
    m_verticalLayout->removeWidget(widget);
}

//XXX: 同一个ActiveConnection StateChanged的信号中会发送两次 ActiveConnection::State::Activated
//原因未知，暂时用定时器处理
void TrayWidget::initConnection()
{
    connect(&m_StateActivatedTimer, &QTimer::timeout, this, [this](){
        handleStateActivated(m_activatedPath);
    });
}

//TODO:缓存一份ActiveConnection，进行对比，状态为Activated的连接就不再重复进行相应状态改变
void TrayWidget::handleActiveConnectionStateChanged(ActiveConnection::State state)
{
    auto activeConnection = qobject_cast<ActiveConnection *>(sender());

    // KLOG_DEBUG() << "sender:" << activeConnection;
    // KLOG_DEBUG() << "activeConnection->type():" << activeConnection->type();
    
    m_activatedPath = activeConnection->path();
    QString id = activeConnection->id();
    QStringList deviceList = activeConnection->devices();

    switch (state)
    {
    case ActiveConnection::State::Unknown:
        KLOG_DEBUG() << "ActiveConnection::State::Unknown";
        break;
    case ActiveConnection::State::Activating:
        KLOG_DEBUG() << "ActiveConnection::State::Activating";
        handleStateActivating(m_activatedPath);
        break;
    case ActiveConnection::State::Activated:
        KLOG_DEBUG() << "ActiveConnection::State::Activated";
        KLOG_DEBUG() << "id:" << id;
        KLOG_DEBUG() << "deviceList:" << deviceList; 
        m_StateActivatedTimer.start();
        KLOG_DEBUG() << "m_StateActivatedTimer start";
        // handleStateActivated(m_activatedPath);
        break;
    case ActiveConnection::State::Deactivating:
        KLOG_DEBUG() << "ActiveConnection::State::Deactivating";
        break;
    case ActiveConnection::State::Deactivated:
        KLOG_DEBUG() << "ActiveConnection::State::Deactivated id:" << id;
        KLOG_DEBUG() << "device path:" << m_devicePtr->uni();
        if (deviceList.contains(m_devicePtr->uni()))
        {
            if (!id.isEmpty())
                StatusNotification::ActiveConnectionDeactivatedNotify(id);
            handleStateDeactivated(m_activatedPath);
        }
        break;
    default:
        break;
    }
}

void TrayWidget::handleDeviceStateChanged(Device::State newstate, Device::State oldstate, Device::StateChangeReason reason)
{
}
