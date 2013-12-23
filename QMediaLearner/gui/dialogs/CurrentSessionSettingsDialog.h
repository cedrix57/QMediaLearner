#ifndef CURRENTSESSIONSETTINGSDIALOG_H
#define CURRENTSESSIONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class CurrentSessionSettingsDialog;
}
namespace ML {
class MediaLearnerLib;
}

class CurrentSessionSettingsDialog : public QDialog{
    Q_OBJECT
    
public:
    explicit CurrentSessionSettingsDialog(
            ML::MediaLearnerLib
            *mediaLearner,
            QWidget *parent = NULL);
    ~CurrentSessionSettingsDialog();
    
protected:
    Ui::CurrentSessionSettingsDialog *ui;
    virtual void accept();
    ML::MediaLearnerLib *mediaLearner;
};

#endif // CURRENTSESSIONSETTINGSDIALOG_H
