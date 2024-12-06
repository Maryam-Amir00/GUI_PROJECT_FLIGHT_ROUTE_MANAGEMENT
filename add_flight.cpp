#include "add_flight.h"
#include "ui_add_flight.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

Add_Flight::Add_Flight(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Add_Flight)
{
    ui->setupUi(this);
    populateComboBoxes();
}

Add_Flight::~Add_Flight()
{
    delete ui;
}

void Add_Flight::populateComboBoxes() {

    ui->sourceComboBox->clear();
    ui->destinationComboBox->clear();
    QSqlQuery query;
    if (query.exec("SELECT name FROM Airports")) {
        while (query.next()) {
            QString airportName = query.value(0).toString();
            ui->sourceComboBox->addItem(airportName);
            ui->destinationComboBox->addItem(airportName);
        }
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to load airports: " + query.lastError().text());
        qDebug() << "Database Error:" << query.lastError().text();
    }
}

void Add_Flight::on_submit_button_clicked() {
    QString sourceAirport = ui->sourceComboBox->currentText();
    QString destinationAirport = ui->destinationComboBox->currentText();
    QString distance = ui->distanceLineEdit->text().trimmed();

    if (sourceAirport.isEmpty() || destinationAirport.isEmpty() || distance.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }
    if (sourceAirport == destinationAirport) {
        QMessageBox::warning(this, "Input Error", "Source and destination cannot be the same.");
        return;
    }

    bool distanceValid;
    double distanceValue = distance.toDouble(&distanceValid);

    if (!distanceValid || distanceValue <= 0) {
        QMessageBox::warning(this, "Input Error", "Distance must be a positive number.");
        return;
    }

    // Calculate cost based on distance
    double costValue = distanceValue * 0.5;  // Example: 0.5 currency units per kilometer

    QSqlQuery query;

    // Retrieve source airport ID
    if (!query.prepare("SELECT airport_id FROM Airports WHERE name = :name")) {
        QMessageBox::critical(this, "Database Error", "Failed to prepare query: " + query.lastError().text());
        return;
    }

    query.bindValue(":name", sourceAirport);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Database Error", "Failed to find source airport ID.");
        qDebug() << "Database Error:" << query.lastError().text();
        return;
    }
    int sourceID = query.value(0).toInt();

    // Retrieve destination airport ID
    query.bindValue(":name", destinationAirport);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Database Error", "Failed to find destination airport ID.");
        qDebug() << "Database Error:" << query.lastError().text();
        return;
    }
    int destinationID = query.value(0).toInt();

    // Insert flight into the database with the calculated cost
    if (!query.prepare("INSERT INTO Flights (source_airport, destination_airport, cost, distance) "
                       "VALUES (:source, :destination, :cost, :distance)")) {
        QMessageBox::critical(this, "Database Error", "Failed to prepare insert query: " + query.lastError().text());
        return;
    }

    query.bindValue(":source", sourceID);
    query.bindValue(":destination", destinationID);
    query.bindValue(":cost", costValue);  // Use calculated cost
    query.bindValue(":distance", distanceValue);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to add flight: " + query.lastError().text());
        qDebug() << "Database Error:" << query.lastError().text();
    } else {
        QMessageBox::information(this, "Success", "Flight added successfully!");
        emit flightAdded();  // Emit the signal
        ui->distanceLineEdit->clear();
    }
}

void Add_Flight::on_cancel_button_clicked() {
    this->close();
}
