#include "add_airport.h"
#include "ui_add_airport.h"

Add_Airport::Add_Airport(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Add_Airport)
{
    ui->setupUi(this);
    // Set focus to the first input field on opening the dialog
    ui->addAirportLineEdit->setFocus();
}

Add_Airport::~Add_Airport()
{
    delete ui;
}

void Add_Airport::on_submit_button_clicked()
{
    QString airportName = ui->addAirportLineEdit->text().trimmed();
    QString location = ui->locationLineEdit->text().trimmed();

    // Validate inputs
    if (airportName.isEmpty() || location.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (airportName.length() > 50 || location.length() > 100) {
        QMessageBox::warning(this, "Input Error", "Input exceeds the maximum allowed length.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not open. Please check your connection.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Airports (name, location) VALUES (:name, :location)");
    query.bindValue(":name", airportName);
    query.bindValue(":location", location);

    if (!query.exec()) {
        if (query.lastError().nativeErrorCode() == "19") { // SQLite constraint violation
            QMessageBox::warning(this, "Duplicate Error", "An airport with this name already exists.");
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to add airport: " + query.lastError().text());
            qDebug() << "Database Error:" << query.lastError().text();
        }
        return;
    }

    QMessageBox::information(this, "Success", "Airport added successfully!");
    emit airportAdded();  // Emit the signal to notify other components

    // Clear inputs for further entries
    ui->addAirportLineEdit->clear();
    ui->locationLineEdit->clear();
    ui->addAirportLineEdit->setFocus(); // Focus back to the first field
}

void Add_Airport::on_cancel_button_clicked()
{
    this->close();
}
