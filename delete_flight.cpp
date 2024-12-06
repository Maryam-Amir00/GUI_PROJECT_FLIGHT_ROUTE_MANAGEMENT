#include "delete_flight.h"
#include "ui_delete_flight.h"

Delete_Flight::Delete_Flight(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Delete_Flight)
{
    ui->setupUi(this);
    loadFlights(); // Populate dropdown with flights
}

Delete_Flight::~Delete_Flight()
{
    delete ui;
}

void Delete_Flight::loadFlights()
{
    QSqlQuery query;

    // Fetch flight data from the database
    if (!query.exec("SELECT flight_id, source_airport, destination_airport FROM Flights")) {
        qDebug() << "Failed to load flights:" << query.lastError().text();
        return;
    }

    ui->comboBox->clear(); // Clear existing dropdown items

    while (query.next()) {
        int flightID = query.value(0).toInt();
        QString source = query.value(1).toString();
        QString destination = query.value(2).toString();

        // Add flight to the dropdown with display text and data
        QString displayText = QString("Flight ID: %1 (From: %2, To: %3)")
                                  .arg(flightID)
                                  .arg(source)
                                  .arg(destination);
        ui->comboBox->addItem(displayText, flightID);
    }

    if (ui->comboBox->count() == 0) {
        qDebug() << "No flights available.";
    }
}

void Delete_Flight::on_delete_button_clicked()
{
    if (ui->comboBox->currentIndex() == -1) {
        QMessageBox::warning(this, "Warning", "Please select a flight to delete.");
        return;
    }

    // Retrieve selected flight ID
    int flightID = ui->comboBox->currentData().toInt();
    qDebug() << "Selected Flight ID for deletion:" << flightID;

    // Prepare and execute the deletion query
    QSqlQuery query;
    query.prepare("DELETE FROM Flights WHERE flight_id = :flightID");
    query.bindValue(":flightID", flightID);

    if (query.exec()) {
        // Deletion successful
        QMessageBox::information(this, "Success", "Flight deleted successfully.");
        qDebug() << "Flight with ID" << flightID << "deleted successfully.";

        emit flightDeleted(); // Emit signal to notify other components

        loadFlights(); // Refresh the dropdown to reflect the updated list
    } else {
        // Deletion failed
        QMessageBox::critical(this, "Error", "Failed to delete the flight: " + query.lastError().text());
        qDebug() << "Failed to delete flight:" << query.lastError().text();
    }
}


void Delete_Flight::on_cancel_button_clicked()
{
    this->close();
}

