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
#include <libnotify/notify.h>
#include <kiran-application.h>
#include <qt5-log-i.h>
#include <QApplication>
#include "src/cpanel-network-widget.h"


int main(int argc, char *argv[]) {
    KiranApplication a(argc, argv);
    klog_qt5_init("", "kylinsec-session", "kiran-cpanel-network", "kiran-cpanel-network");
    CPanelNetworkWidget networkWidget;
    networkWidget.show();

//    notify_init("NetworkNotify");
//    NotifyNotification *hello = notify_notification_new("summary","body","network-wireless-signal-excellent-symbolic.symbolic");
//    notify_notification_show(hello, nullptr);
//    g_object_unref(G_OBJECT(hello));
//    notify_uninit();

    return QApplication::exec();
}
