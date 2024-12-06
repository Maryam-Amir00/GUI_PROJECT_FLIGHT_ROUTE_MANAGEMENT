#include "display.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// Constructor
Display::Display(QWidget *parent)
    : QDialog(parent)
    , model(new QSqlQueryModel(this))
{
    setupUI();
    populateTable();
}

// Destructor
Display::~Display()
{
    delete model;
}

// Setup UI
void Display::setupUI()
{
    this->setWindowTitle("Display");
    this->setFixedSize(550, 430);
    this->setStyleSheet("background-color: rgb(33, 37, 41);");

    // Label
    label = new QLabel("Display All Routes", this);
    label->setGeometry(180, 10, 261, 41);
    label->setStyleSheet("font: 700 18pt 'Segoe UI'; text-decoration: underline;");
    label->setAlignment(Qt::AlignCenter);

    // Table View
    tableView = new QTableView(this);
    tableView->setGeometry(30, 80, 491, 281);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Back Button
    backButton = new QPushButton("Back", this);
    backButton->setGeometry(30, 370, 491, 41);
    backButton->setStyleSheet("background-color: rgb(235, 240, 245); color: rgb(0, 0, 0); font: 700 9pt 'Segoe UI';");

    connect(backButton, &QPushButton::clicked, this, &Display::on_back_button_clicked);
}

// Populate Table
void Display::populateTable()
{
    QString queryStr = R"(
        SELECT
            a1.name || ' -> ' || a2.name AS Route,
            f.cost || ' $ ' AS Cost,
            f.distance || ' km' AS Distance
        FROM Flights f
        JOIN Airports a1 ON f.source_airport = a1.airport_id
        JOIN Airports a2 ON f.destination_airport = a2.airport_id
    )";

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Database Error:" << model->lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to retrieve flight routes.");
    } else {
        qDebug() << "Table refreshed successfully.";
        tableView->setModel(model);
        model->setHeaderData(0, Qt::Horizontal, "Route");
        model->setHeaderData(1, Qt::Horizontal, "Cost");
        model->setHeaderData(2, Qt::Horizontal, "Distance");
    }
}

// Slot to refresh data
void Display::refreshData()
{
    populateTable(); // Simply call populateTable to refresh the data
}

// Back Button Slot
void Display::on_back_button_clicked()
{
    this->close();
}
