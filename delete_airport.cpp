#include "delete_airport.h"
#include "ui_delete_airport.h"

Delete_Airport::Delete_Airport(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Delete_Airport)
{
    ui->setupUi(this);
    populateComboBox(); // Initial population of the combo box
}

Delete_Airport::~Delete_Airport()
{
    delete ui;
}

// Populate the combo box with airport names
void Delete_Airport::populateComboBox()
{
    ui->comboBox->clear(); // Clear previous items to avoid duplicates

    QSqlQuery query;
    if (query.exec("SELECT name FROM Airports")) {
        while (query.next()) {
            QString airportName = query.value(0).toString();
            ui->comboBox->addItem(airportName); // Add airport name to combo box
        }
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to load airports: " + query.lastError().text());
        qDebug() << "Database Error:" << query.lastError().text();
    }
}

// Refresh the combo box dynamically
void Delete_Airport::refreshComboBox()
{
    populateComboBox(); // Repopulate combo box with the latest data
}

// Handle delete button click
void Delete_Airport::on_delete_button_clicked()
{
    QString airportName = ui->comboBox->currentText();

    if (airportName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "No airport selected.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT airport_id FROM Airports WHERE name = :name");
    query.bindValue(":name", airportName);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Database Error", "Failed to find airport: " + query.lastError().text());
        return;
    }

    int airportID = query.value(0).toInt();

    // Delete flights associated with the airport
    query.prepare("DELETE FROM Flights WHERE source_airport = :id OR destination_airport = :id");
    query.bindValue(":id", airportID);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete associated flights: " + query.lastError().text());
        return;
    }

    // Delete the airport
    query.prepare("DELETE FROM Airports WHERE airport_id = :id");
    query.bindValue(":id", airportID);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete airport: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Airport and associated flights deleted successfully!");
    emit airportDeleted(); // Emit signal for other windows
    refreshComboBox();     // Refresh combo box after deletion
}

// Handle cancel button click
void Delete_Airport::on_cancel_button_clicked()
{
    this->close();
}
