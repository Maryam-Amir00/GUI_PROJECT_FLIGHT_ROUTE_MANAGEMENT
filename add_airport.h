#ifndef ADD_AIRPORT_H
#define ADD_AIRPORT_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Add_Airport;
}

class Add_Airport : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Airport(QWidget *parent = nullptr);
    ~Add_Airport();

signals:
    void airportAdded();  // Signal emitted when a new airport is added

private slots:
    void on_submit_button_clicked();
    void on_cancel_button_clicked();

private:
    Ui::Add_Airport *ui;
};

#endif // ADD_AIRPORT_H
