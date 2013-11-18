#ifndef EXPORTVIDEODIALOG_H
#define EXPORTVIDEODIALOG_H

#include <QDialog>

namespace Ui {
class ExportVideoDialog;
}

class ExportVideoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportVideoDialog(QWidget *parent = 0);
    ~ExportVideoDialog();
    
private:
    Ui::ExportVideoDialog *ui;
};

#endif // EXPORTVIDEODIALOG_H
