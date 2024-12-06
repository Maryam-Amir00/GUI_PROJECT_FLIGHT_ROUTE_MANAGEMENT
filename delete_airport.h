#ifndef DELETE_AIRPORT_H
#define DELETE_AIRPORT_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class Delete_Airport;
}

class Delete_Airport : public QDialog
{
    Q_OBJECT

public:
    explicit Delete_Airport(QWidget *parent = nullptr);
    ~Delete_Airport();

    void populateComboBox();
     void refreshComboBox();

signals:
    void airportDeleted();  // Signal to notify when an airport is deleted

private slots:
    void on_delete_button_clicked();
    void on_cancel_button_clicked();

private:
    Ui::Delete_Airport *ui;
};

#endif // DELETE_AIRPORT_H
