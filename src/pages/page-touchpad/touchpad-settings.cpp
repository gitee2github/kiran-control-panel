/**
 * @file touchpad-settings.cpp
 * @brief  初始化触摸板属性，并处理用户切换属性信号
 * @author yuanxing <yuanxing@kylinos.com.cn>
 * @copyright (c) 2020 KylinSec. All rights reserved.
 */

#include "touchpad-settings.h"
#include "ui_touchpad-settings.h"
#include "general-functions/general-function-class.h"
#include "dbus-interface/touchpad-interface.h"

#include <QCheckBox>
#include <iostream>

TouchPadSettings::TouchPadSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchPadSettings)
{
    ui->setupUi(this);
}

TouchPadSettings::~TouchPadSettings()
{
    delete ui;
}

bool TouchPadSettings::initUI()
{
    m_touchPadInterface = ComKylinsecKiranSessionDaemonTouchPadInterface::instance();
    if(!m_touchPadInterface->isValid())
    {
        qDebug() << "connect touchpad dbus faild!" << endl;
        return false;
    }

    m_comboBoxList = this->findChildren<QComboBox *>();
    m_checkBoxList = {ui->checkBox_tap_to_click,
                      ui->checkBox_tp_natural_scroll,
                      ui->checkBox_disable_while_typing};

    m_labelList = this->findChildren<QLabel *>();

    addComboBoxItem();

    ui->slider_tp_speed->setMaximum(SLIDER_MAXIMUN);
    ui->slider_tp_speed->setMinimum(SLIDER_MINIMUM);
    ui->slider_tp_speed->setPageStep((SLIDER_MAXIMUN-SLIDER_MINIMUM+1)/2);
    ui->slider_tp_speed->setSingleStep((SLIDER_MAXIMUN-SLIDER_MINIMUM+1)/2);

    ui->widget_tp_click_mode->hide();

    initPageTouchPadUI();
    return true;
}

void TouchPadSettings::initPageTouchPadUI()
{
    m_touchPadEnabled = m_touchPadInterface->touchpad_enabled();
    ui->checkBox_tp_disable_touchpad->setChecked(!m_touchPadEnabled);
    if(!m_touchPadEnabled)
    {
        setDisableWidget(true);
    }
    connect(ui->checkBox_tp_disable_touchpad, &QCheckBox::toggled, this,
            &TouchPadSettings::onDisabelTouchPadToggled);

    m_touchPadLeftHand = m_touchPadInterface->left_handed();
    ui->comboBox_tp_hand_mode->setCurrentIndex(m_touchPadLeftHand);
    connect(ui->comboBox_tp_hand_mode, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            [this](int currentIntex){
        m_touchPadLeftHand = currentIntex;
        m_touchPadInterface->setLeft_handed(m_touchPadLeftHand);
    });

    m_touchPadMotionAcceleration = m_touchPadInterface->motion_acceleration();
    if(m_touchPadMotionAcceleration == MOTION_SLOW)
    {
        ui->slider_tp_speed->setValue(SLIDER_MINIMUM);
        ui->label_tp_speed->setText(tr("Slow"));
    }
    else if(m_touchPadMotionAcceleration == MOTION_STANDARD)
    {
        ui->slider_tp_speed->setValue((SLIDER_MAXIMUN-SLIDER_MINIMUM+1)/2);
        ui->label_tp_speed->setText(tr("Standard"));
    }
    else
    {
        ui->slider_tp_speed->setValue(MOTION_FAST);
        ui->label_tp_speed->setText(tr("Fast"));
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout,
            [this]{
        std::cout << "timeout" << endl;
        double scrollSpeed;
        int value;

        value = ui->slider_tp_speed->value();
        scrollSpeed = GeneralFunctionClass::convertValue(ui->slider_tp_speed,ui->label_tp_speed,m_mousePressed,value);

        m_touchPadMotionAcceleration = scrollSpeed;
        m_touchPadInterface->setMotion_acceleration(m_touchPadMotionAcceleration);
        m_timer->stop();
    });

    connect(ui->slider_tp_speed,&QSlider::sliderPressed,[this](){
        m_mousePressed = true;
    });
    connect(ui->slider_tp_speed,&QSlider::sliderReleased,[this](){
        m_mousePressed = false;
        emit ui->slider_tp_speed->valueChanged(ui->slider_tp_speed->value());
    });
    connect(ui->slider_tp_speed,&QSlider::valueChanged,this, &TouchPadSettings::onSliderValueChange);

    m_clickMethod = m_touchPadInterface->click_method();
    ui->comboBox_tp_click_mode->setCurrentIndex(m_clickMethod);
    connect(ui->comboBox_tp_click_mode, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            [this](int currentIndex){
       m_clickMethod = currentIndex;
       m_touchPadInterface->setClick_method(m_clickMethod);
    });

    m_scrollMethod = m_touchPadInterface->scroll_method();
    ui->comboBox_tp_move_win_mode ->setCurrentIndex(m_scrollMethod);
    connect(ui->comboBox_tp_move_win_mode,static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
            [this](int currentIndex){
       m_scrollMethod = currentIndex;
       m_touchPadInterface->setScroll_method(m_scrollMethod);
    });

    m_touchPadNaturalScroll = m_touchPadInterface->natural_scroll();
    ui->checkBox_tp_natural_scroll->setChecked(m_touchPadNaturalScroll);
    connect(ui->checkBox_tp_natural_scroll,&KiranSwitchButton::toggled,
            [this](bool isNaturalScroll){
       m_touchPadNaturalScroll = isNaturalScroll;
       m_touchPadInterface->setNatural_scroll(m_touchPadNaturalScroll);
    });

    m_disabelWhileTyping = m_touchPadInterface->disable_while_typing();
    ui->checkBox_disable_while_typing->setChecked(m_disabelWhileTyping);
    connect(ui->checkBox_disable_while_typing,&KiranSwitchButton::toggled,
            [this](bool disabelWhileTyping){
       m_disabelWhileTyping = disabelWhileTyping;
       m_touchPadInterface->setDisable_while_typing(m_disabelWhileTyping);
    });

    m_tapToClick = m_touchPadInterface->tap_to_click();
    ui->checkBox_tap_to_click->setChecked(m_tapToClick);
    connect(ui->checkBox_tap_to_click,&KiranSwitchButton::toggled,
            [this](bool isTapToClick){
       m_tapToClick  = isTapToClick;
       m_touchPadInterface->setTap_to_click(m_tapToClick);
    });
}

void TouchPadSettings::addComboBoxItem()
{
    QStringList hand_mode;
    hand_mode << tr("Right Hand Mode") << tr("Left Hand Mode") ;
    ui->comboBox_tp_hand_mode->addItems(hand_mode);

    QStringList tpClickMode;
    tpClickMode << tr("Press and Tap") << tr("Tap") ;
    ui->comboBox_tp_click_mode->addItems(tpClickMode);

    QStringList tpScrollWinMode;
    tpScrollWinMode << tr("Two Finger Scroll") <<tr("Edge Scroll");
    ui->comboBox_tp_move_win_mode->addItems(tpScrollWinMode);
}

void TouchPadSettings::setDisableWidget(bool disabled)
{
    foreach (QComboBox *comboBox, m_comboBoxList)
    {
        comboBox->setDisabled(disabled);
    }
    foreach (KiranSwitchButton* checkBox, m_checkBoxList)
    {
        checkBox->setDisabled(disabled);
    }
    ui->slider_tp_speed->setDisabled(disabled);
    ui->label_tp_speed->setDisabled(disabled);
}

void TouchPadSettings::onSliderValueChange()
{
    m_timer->start(100);
}

void TouchPadSettings::onDisabelTouchPadToggled(bool disabled)
{
    m_touchPadEnabled = !disabled;
    m_touchPadInterface->setTouchpad_enabled(m_touchPadEnabled);
    if(disabled)
    {
        setDisableWidget(true);
    }
    else
    {
        setDisableWidget(false);
    }
}
