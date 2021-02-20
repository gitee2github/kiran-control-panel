/**
 * @file   kiran-system-widget.h
 * @brief  带有自定义标题栏的系统信息界面
 * @author yuanxing<yuanxing@kylinos.com.cn>
 * @copyright(c) 2021 KylinSec. All rights reserved.
 */

#ifndef KIRANSYSTEMWIDGET_H
#define KIRANSYSTEMWIDGET_H

#include <kiranwidgets-qt5/kiran-titlebar-window.h>
#include "kiran-system-information.h"

#include <QPushButton>

class KiranSystemWidget : public KiranTitlebarWindow
{
    Q_OBJECT
public:
    explicit KiranSystemWidget();
    ~KiranSystemWidget();

private:
    QMenu* createMenu();
    void setMenuIntoTitlebar();

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
     kiranSystemInformation *centerWgt;
     QPushButton* btnMenu;
};

#endif // KIRANSYSTEMWIDGET_H
