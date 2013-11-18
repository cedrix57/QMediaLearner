#include "ExportVideoDialog.h"
#include "ui_ExportVideoDialog.h"

ExportVideoDialog::ExportVideoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportVideoDialog)
{
    ui->setupUi(this);
}

ExportVideoDialog::~ExportVideoDialog()
{
    delete ui;
}
