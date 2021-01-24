#include "kiran-display-configuration-window.h"
#include "kiran-display-config-global.h"
#include "kiranwidgets-qt5/kiran-message-box.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QDBusInterface>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
        return 1;
    }
    if(!QDBusInterface(KIRAN_DBUS_SERVICE_NAME, KIRAN_DBUS_DISPLAY).isValid())
    {
        KiranMessageBox box;
        box.setTitle(QObject::tr("Tips"));

        QPushButton btn;
        btn.setText(QObject::tr("OK(K)"));
        btn.setFixedSize(QSize(200, box.buttonSize().height()));
        btn.setShortcut(Qt::CTRL + Qt::Key_K);
        box.addButton(&btn, QDialogButtonBox::AcceptRole);
        box.setText(QObject::tr("Background D-Bus service failed to connect, display setting failed to start, please check if D-Bus service is start."));
        box.exec();
        return 1;
    }

    QString locale = QLocale::system().name();
    QString qmFile = QString("%1.%2.qm").arg(TRANSLATE_PREFIX).arg(locale);
    QTranslator translator;
    if(translator.load(qmFile) == false)
        qDebug() << "load qm: " << qmFile <<  " error.";
    else
        a.installTranslator(&translator);

    DisplayModesStu::registerMetaType();

    KiranDisplayConfigurationWindow w;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    w.move(screenRect.center()-QPoint(w.rect().width()/2, w.rect().height()/2));
    w.show();

    return a.exec();
}
