#ifndef CURRENTSESSIONSETTINGSDIALOG_H
#define CURRENTSESSIONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class CurrentSessionSettingsDialog;
}

class CurrentSessionSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CurrentSessionSettingsDialog(QWidget *parent = 0);
    ~CurrentSessionSettingsDialog();
    
private:
    Ui::CurrentSessionSettingsDialog *ui;
};

#endif // CURRENTSESSIONSETTINGSDIALOG_H
