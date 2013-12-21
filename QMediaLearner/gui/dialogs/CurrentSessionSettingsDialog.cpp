#include "CurrentSessionSettingsDialog.h"
#include "ui_CurrentSessionSettingsDialog.h"

CurrentSessionSettingsDialog::CurrentSessionSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrentSessionSettingsDialog)
{
    ui->setupUi(this);
}

CurrentSessionSettingsDialog::~CurrentSessionSettingsDialog()
{
    delete ui;
}
