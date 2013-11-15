#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
class QComboBox;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    virtual void accept();
    //virtual void reject();
    void loadSettings();
    void saveSettings();

public slots:
    void pickSubColor1();
    void pickSubColor2();
    void pickSubColor3();

protected:
    Ui::SettingsDialog *ui;
    void _connectSlots();
    void _pickSubColor(
            int idSub,
            QPushButton *button);
    QColor subColors[3];
};

#endif // SETTINGSDIALOG_H
