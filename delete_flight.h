#ifndef DELETE_FLIGHT_H
#define DELETE_FLIGHT_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Delete_Flight;
}

class Delete_Flight : public QDialog
{
    Q_OBJECT

public:
    explicit Delete_Flight(QWidget *parent = nullptr);
    ~Delete_Flight();

    void loadFlights();  // Make this method public

signals:
    void flightDeleted();  // Signal to notify when a flight is deleted

private slots:

    void on_delete_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Delete_Flight *ui;
    void debugComboBoxItems();  // New method for debugging
};

#endif // DELETE_FLIGHT_H
