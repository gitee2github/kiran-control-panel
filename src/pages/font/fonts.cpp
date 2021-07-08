#include "fonts.h"
#include "ui_fonts.h"
#include <QFontDatabase>
#include <kiran-session-daemon/appearance-i.h>
#include <kiran-message-box.h>
#include "dbus-interface/appearance-global-info.h"
#include <iostream>
#include <QDebug>


using namespace std;
Fonts::Fonts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Fonts)
{
    ui->setupUi(this);
    initUI();
}


Fonts::~Fonts()
{
    delete ui;
}

bool Fonts::initUI()
{
    //统一QComboBox样式，并初始化可选值列表
    QList<QComboBox* > comboBoxList = this ->findChildren<QComboBox*>();
    foreach (QComboBox* comboBox, comboBoxList) {
        comboBox->setStyleSheet("QComboBox {combobox-popup: 0;}");
    }

    QStringList sizes ;
    sizes << "7"<< "8" <<"9" << "10" << "11" << "12" << "13" << "14" ;

    ui->cbox_application_font_size->addItems(sizes);
    ui->cbox_monospace_font_size->addItems(sizes);
    ui->cbox_titlebar_font_size->addItems(sizes);

    QFontDatabase database;
    foreach (const QString &family, database.families()) {
        ui->cbox_application_font_name->addItem(family);
        ui->cbox_monospace_font_name->addItem(family);
        ui->cbox_titlebar_font_name->addItem(family);
        }

    // 获取各个类型的字体值
    getCurrentFontInfo(APPEARANCE_FONT_TYPE_APPLICATION);
    getCurrentFontInfo(APPEARANCE_FONT_TYPE_WINDOW_TITLE);
    getCurrentFontInfo(APPEARANCE_FONT_TYPE_MONOSPACE);

    connectSignals();

    return true;

}

void Fonts::getCurrentFontInfo(int fontType)
{
    QStringList fontInfoList;
    switch (fontType) {
    case APPEARANCE_FONT_TYPE_APPLICATION:
        fontInfoList = getFont(APPEARANCE_FONT_TYPE_APPLICATION);
        if(!fontInfoList.isEmpty())
        {
            m_applicationFontInfo = fontInfoList;

            ui->cbox_application_font_name->blockSignals(true);
            ui->cbox_application_font_size->blockSignals(true);
            ui->cbox_application_font_name->setCurrentText(fontInfoList.at(0));
            ui->cbox_application_font_size->setCurrentText(fontInfoList.at(1));
            ui->cbox_application_font_name->blockSignals(false);
            ui->cbox_application_font_size->blockSignals(false);
        }
        break;
    case APPEARANCE_FONT_TYPE_WINDOW_TITLE:
        fontInfoList = getFont(APPEARANCE_FONT_TYPE_WINDOW_TITLE);
        if(!fontInfoList.isEmpty())
        {
            m_windowTitleFontInfo = fontInfoList;

            ui->cbox_titlebar_font_name->blockSignals(true);
            ui->cbox_titlebar_font_size->blockSignals(true);
            ui->cbox_titlebar_font_name->setCurrentText(fontInfoList.at(0));
            ui->cbox_titlebar_font_size->setCurrentText(fontInfoList.at(1));
            ui->cbox_titlebar_font_name->blockSignals(false);
            ui->cbox_titlebar_font_size->blockSignals(false);
        }
        break;
    case APPEARANCE_FONT_TYPE_MONOSPACE:
        fontInfoList = getFont(APPEARANCE_FONT_TYPE_MONOSPACE);
        if(!fontInfoList.isEmpty())
        {
            m_monospaceFontInfo = fontInfoList;
            ui->cbox_monospace_font_name->blockSignals(true);
            ui->cbox_titlebar_font_size->blockSignals(true);
            ui->cbox_monospace_font_name->setCurrentText(fontInfoList.at(0));
            ui->cbox_monospace_font_size->setCurrentText(fontInfoList.at(1));
            ui->cbox_monospace_font_name->blockSignals(false);
            ui->cbox_titlebar_font_size->blockSignals(false);
        }
        break;
    default:
        break;
    }
}

QStringList Fonts::getFont(int fontType)
{
    return AppearanceGlobalInfo::instance()->getFont(fontType);
}

void Fonts::setFont(int fontType, QStringList fontInfoList)
{
    if(!AppearanceGlobalInfo::instance()->setFont(fontType,fontInfoList))
    {
        KiranMessageBox::message(nullptr,QObject::tr("Failed"),
                                 QObject::tr("Set font  failed!"),
                                 KiranMessageBox::Ok);
    }
}

void Fonts::connectSignals()
{
    connect(ui->cbox_application_font_name, &QComboBox::currentTextChanged, [=](QString text){
        m_applicationFontInfo.replace(0,text);
        QString temp0 = m_applicationFontInfo.at(0);
        QString temp1 = m_applicationFontInfo.at(1);
        cout << "m_applicationFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_applicationFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_APPLICATION,m_applicationFontInfo);
    });
    connect(ui->cbox_application_font_size, &QComboBox::currentTextChanged, [=](QString text){
        m_applicationFontInfo.replace(1,text);
        QString temp0 = m_applicationFontInfo.at(0);
        QString temp1 = m_applicationFontInfo.at(1);
        cout << "m_applicationFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_applicationFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_APPLICATION,m_applicationFontInfo);
    });
    connect(ui->cbox_monospace_font_name, &QComboBox::currentTextChanged, [=](QString text){
        m_monospaceFontInfo.replace(0,text);
        QString temp0 = m_monospaceFontInfo.at(0);
        QString temp1 = m_monospaceFontInfo.at(1);
        cout << "m_monospaceFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_monospaceFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_MONOSPACE,m_monospaceFontInfo);
    });
    connect(ui->cbox_monospace_font_size, &QComboBox::currentTextChanged, [=](QString text){
        m_monospaceFontInfo.replace(1,text);
        QString temp0 = m_monospaceFontInfo.at(0);
        QString temp1 = m_monospaceFontInfo.at(1);
        cout << "m_monospaceFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_monospaceFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_MONOSPACE,m_monospaceFontInfo);
    });
    connect(ui->cbox_titlebar_font_name, &QComboBox::currentTextChanged, [=](QString text){
        m_windowTitleFontInfo.replace(0,text);
        QString temp0 = m_windowTitleFontInfo.at(0);
        QString temp1 = m_windowTitleFontInfo.at(1);
        cout << "m_windowTitleFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_windowTitleFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_WINDOW_TITLE,m_windowTitleFontInfo);
    });
    connect(ui->cbox_titlebar_font_size, &QComboBox::currentTextChanged, [=](QString text){
        m_windowTitleFontInfo.replace(1,text);
        QString temp0 = m_windowTitleFontInfo.at(0);
        QString temp1 = m_windowTitleFontInfo.at(1);
        cout << "m_windowTitleFontInfo.at(0) = " << temp0.toStdString() << endl;
        cout << "m_windowTitleFontInfo.at(1) = " << temp1.toStdString() << endl;
        setFont(APPEARANCE_FONT_TYPE_WINDOW_TITLE,m_windowTitleFontInfo);
    });
}
