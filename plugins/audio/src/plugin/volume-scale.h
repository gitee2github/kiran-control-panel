/**
 * Copyright (c) 2020 ~ 2021 KylinSec Co., Ltd.
 * kiran-cpanel-audio is licensed under Mulan PSL v2.
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

#ifndef KIRAN_CPANEL_AUDIO_VOLUMESCALE_H
#define KIRAN_CPANEL_AUDIO_VOLUMESCALE_H

#include <QWidget>

class VolumeScale : public QWidget
{
    Q_OBJECT
public:
    VolumeScale(QWidget *parent = nullptr);
    ~VolumeScale();

    void paintEvent(QPaintEvent *event) override;
    void drawTicks(QPainter *painter);
    void setPercent(qreal percent);

private:
    qreal m_percent = 0;
};

#endif  //KIRAN_CPANEL_AUDIO_VOLUMESCALE_H
