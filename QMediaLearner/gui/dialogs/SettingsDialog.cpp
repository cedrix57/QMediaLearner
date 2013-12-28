#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <SettingsManagerSingleton.h>
#include <QPixmap>
#include <QColorDialog>
#include <FontManagerSingleton.h>

//====================================
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog){
    ui->setupUi(this);
    this->loadSettings();
    this->_connectSlots();
}
//====================================
SettingsDialog::~SettingsDialog(){
    delete ui;
}
//====================================
void SettingsDialog::_connectSlots(){
    this->connect(
                this->ui->buttonColor1,
                SIGNAL(clicked()),
                SLOT(pickSubColor1()));
    this->connect(
                this->ui->buttonColor2,
                SIGNAL(clicked()),
                SLOT(pickSubColor2()));
    this->connect(
                this->ui->buttonColor3,
                SIGNAL(clicked()),
                SLOT(pickSubColor3()));
}
//====================================
void SettingsDialog::loadSettings(){
    ML::SettingsManagerSingleton
            *settingsManager
            = ML::SettingsManagerSingleton
            ::getInstance();
    bool sendLogWhenCrashed
            = settingsManager->isSendLogFileWhenCrashed();
    this->ui->checkBoxSendLog
            ->setChecked(sendLogWhenCrashed);
    bool saveLogs
            = settingsManager->isSaveInLogFile();
    this->ui->checkBoxSaveLog
            ->setChecked(saveLogs);
    QString fontFamily1
            = settingsManager->getSubFontFamily(0);
    QString fontFamily2
            = settingsManager->getSubFontFamily(1);
    QString fontFamily3
            = settingsManager->getSubFontFamily(2);
    QStringList availableFont
            = ML::FontManagerSingleton
            ::getInstance()
            ->getFontNames();
    this->ui->comboBoxFont1
            ->clear();
    this->ui->comboBoxFont2
            ->clear();
    this->ui->comboBoxFont3
            ->clear();
    this->ui->comboBoxFont1
            ->addItems(
                availableFont);
    this->ui->comboBoxFont2
            ->addItems(
                availableFont);
    this->ui->comboBoxFont3
            ->addItems(
                availableFont);
    this->ui->comboBoxFont1
            ->setCurrentText(fontFamily1);
    this->ui->comboBoxFont2
            ->setCurrentText(fontFamily2);
    this->ui->comboBoxFont3
            ->setCurrentText(fontFamily3);
    ML::SubSize
            subSize1
            = settingsManager->getSubSize(0);
    this->ui->comboBoxSize1
            ->setCurrentIndex(
                (int)subSize1);
    ML::SubSize
            subSize2
            = settingsManager->getSubSize(1);
    this->ui->comboBoxSize2
            ->setCurrentIndex(
                (int)subSize2);
    ML::SubSize
            subSize3
            = settingsManager->getSubSize(2);
    this->ui->comboBoxSize3
            ->setCurrentIndex(
                (int)subSize3);
    ML::SubPosition
            subPosition1
            = settingsManager->getSubPosition(0);
    ML::SubPosition
            subPosition2
            = settingsManager->getSubPosition(1);
    ML::SubPosition
            subPosition3
            = settingsManager->getSubPosition(2);
    this->ui->comboBoxPosition1
            ->setCurrentIndex(
                (int)subPosition1);
    this->ui->comboBoxPosition2
            ->setCurrentIndex(
                (int)subPosition2);
    this->ui->comboBoxPosition3
            ->setCurrentIndex(
                (int)subPosition3);
    QColor subColor1
            = settingsManager->getSubColor(0);
    this->subColors[0] = subColor1;
    QIcon icon;
    QPixmap pixmap(32, 32);
    pixmap.fill(subColor1);
    icon.addPixmap(pixmap);
    this->ui->buttonColor1
            ->setIcon(icon);
    QColor subColor2
            = settingsManager->getSubColor(1);
    this->subColors[1] = subColor2;
    pixmap.fill(subColor2);
    icon.addPixmap(pixmap);
    this->ui->buttonColor2
            ->setIcon(icon);
    QColor subColor3
            = settingsManager->getSubColor(2);
    this->subColors[2] = subColor3;
    pixmap.fill(subColor3);
    icon.addPixmap(pixmap);
    this->ui->buttonColor3
            ->setIcon(icon);
}
//====================================
void SettingsDialog::accept(){
    QDialog::accept();
    this->saveSettings();
}
//====================================
void SettingsDialog::saveSettings(){
    ML::SettingsManagerSingleton
            *settingsManager
            = ML::SettingsManagerSingleton
            ::getInstance();
    bool sendLogWhenCrashed
            = this->ui->checkBoxSendLog
            ->isChecked();
    settingsManager->setSendLogFileWhenCrashed(
                sendLogWhenCrashed);
    bool saveLogs
            = this->ui->checkBoxSaveLog
            ->isChecked();
    settingsManager->setSaveInLogFiles(
                saveLogs);
    QString fontFamily1
            = this->ui->comboBoxFont1
            ->currentText();
            settingsManager->setSubFontFamily(
                        0,
                        fontFamily1);
    QString fontFamily2
            = this->ui->comboBoxFont2
            ->currentText();
            settingsManager->setSubFontFamily(
                        1,
                        fontFamily2);
    QString fontFamily3
            = this->ui->comboBoxFont3
            ->currentText();
            settingsManager->setSubFontFamily(
                        2,
                        fontFamily3);
    ML::SubSize
            subSize1
            = (ML::SubSize)
            this->ui->comboBoxSize1->currentIndex();
    settingsManager->setSubSize(
                0,
                subSize1);
    ML::SubSize
            subSize2
            = (ML::SubSize)
            this->ui->comboBoxSize2->currentIndex();
    settingsManager->setSubSize(
                1,
                subSize2);
    ML::SubSize
            subSize3
            = (ML::SubSize)
            this->ui->comboBoxSize3->currentIndex();
    settingsManager->setSubSize(
                2,
                subSize3);
    ML::SubPosition
            subPosition1
            = (ML::SubPosition)
            this->ui->comboBoxPosition1->currentIndex();
            settingsManager->setSubPosition(
                        0,
                        subPosition1);
    ML::SubPosition
            subPosition2
            = (ML::SubPosition)
            this->ui->comboBoxPosition2->currentIndex();
            settingsManager->setSubPosition(
                        1,
                        subPosition2);
    ML::SubPosition
            subPosition3
            = (ML::SubPosition)
            this->ui->comboBoxPosition3->currentIndex();
            settingsManager->setSubPosition(
                        2,
                        subPosition3);

    for(int i=0; i<3; i++){
        settingsManager->setSubColor(
                    i,
                    this->subColors[i]);
    }
}
//====================================
void SettingsDialog::pickSubColor1(){
    this->_pickSubColor(
                0,
                this->ui->buttonColor1);
}
//====================================
void SettingsDialog::_pickSubColor(
        int idSub,
        QPushButton *button){
    ML::SettingsManagerSingleton
            *settingsManager
            = ML::SettingsManagerSingleton
            ::getInstance();
    QColor subColor
            = settingsManager->getSubColor(idSub);
    QColor color = QColorDialog::getColor(subColor, this);
    if(color.isValid()){
        QIcon icon;
        QPixmap pixmap(32, 32);
        pixmap.fill(color);
        icon.addPixmap(pixmap);
        button->setIcon(icon);
        this->subColors[idSub] = color;
    }
}
//====================================
void SettingsDialog::pickSubColor2(){
    this->_pickSubColor(
                1,
                this->ui->buttonColor2);
}
//====================================
void SettingsDialog::pickSubColor3(){
    this->_pickSubColor(
                2,
                this->ui->buttonColor3);
}
//====================================
