#ifndef ADD_FLIGHT_H
#define ADD_FLIGHT_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Add_Flight;
}

class Add_Flight : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Flight(QWidget *parent = nullptr);
    ~Add_Flight();
    void populateComboBoxes();  // Make this method public

signals:
    void flightAdded();  // Signal to notify when a flight is added

private slots:
    void on_submit_button_clicked();
    void on_cancel_button_clicked();

private:
    Ui::Add_Flight *ui;
};

#endif // ADD_FLIGHT_H
