#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!setupDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    ptrAddAirport = new Add_Airport();
    ptrAddFlight = new Add_Flight();
    ptrDeleteAirport = new Delete_Airport();
    ptrDeleteFlight = new Delete_Flight();
    ptrShortestPath = new Shortest_Path();
    ptrDisplay = new Display();

    connect(ptrAddAirport, &Add_Airport::airportAdded, this, &MainWindow::updateComboBoxes);
    connect(ptrDeleteAirport, &Delete_Airport::airportDeleted, this, &MainWindow::updateComboBoxes);
    connect(ptrDeleteFlight, &Delete_Flight::flightDeleted, this, &MainWindow::updateFlights);
    connect(ptrAddFlight, &Add_Flight::flightAdded, this, &MainWindow::updateFlights);

    // Connect signals to update the display table and graph whenever necessary
    connect(ptrAddAirport, &Add_Airport::airportAdded, ptrDisplay, &Display::populateTable);
    connect(ptrDeleteAirport, &Delete_Airport::airportDeleted, ptrDisplay, &Display::populateTable);
    connect(ptrDeleteFlight, &Delete_Flight::flightDeleted, ptrDisplay, &Display::populateTable);
    connect(ptrAddFlight, &Add_Flight::flightAdded, ptrDisplay, &Display::populateTable);

    // Connect signal to update the graph in Shortest_Path
    connect(ptrAddAirport, &Add_Airport::airportAdded, ptrShortestPath, &Shortest_Path::updateGraph);
    connect(ptrDeleteAirport, &Delete_Airport::airportDeleted, ptrShortestPath, &Shortest_Path::updateGraph);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptrAddAirport;
    delete ptrAddFlight;
    delete ptrDeleteAirport;
    delete ptrDeleteFlight;
    delete ptrShortestPath;
    delete ptrDisplay;
}

bool MainWindow::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("flights.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to connect to database!" << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully.";

    QSqlQuery query;

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Airports ("
            "airport_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL UNIQUE, "
            "location TEXT NOT NULL)")) {
        qDebug() << "Failed to create Airports table:" << query.lastError().text();
    } else {
        qDebug() << "Airports table created or already exists.";
    }

    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS Flights ("
            "flight_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "source_airport INTEGER NOT NULL, "
            "destination_airport INTEGER NOT NULL, "
            "cost REAL NOT NULL, "
            "distance REAL NOT NULL, "
            "FOREIGN KEY (source_airport) REFERENCES Airports (airport_id), "
            "FOREIGN KEY (destination_airport) REFERENCES Airports (airport_id))")) {
        qDebug() << "Failed to create Flights table:" << query.lastError().text();
    } else {
        qDebug() << "Flights table created or modified successfully.";
    }

    return true;
}

void MainWindow::on_add_Airport_clicked()
{
    ptrAddAirport->show();
}

void MainWindow::on_add_flight_clicked()
{
    ptrAddFlight->show();
}

void MainWindow::on_delete_airport_clicked()
{
    ptrDeleteAirport->show();
}

void MainWindow::on_delete_flight_clicked()
{
    ptrDeleteFlight->show();
}

void MainWindow::on_shortest_path_clicked()
{
    ptrShortestPath->show();
}

void MainWindow::on_display_clicked()
{
    ptrDisplay->show();
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::updateComboBoxes()
{
    ptrAddFlight->populateComboBoxes();
    ptrShortestPath->populateComboBoxes();
    ptrDeleteAirport->populateComboBox();
}

void MainWindow::updateFlights()
{
    ptrDeleteFlight->loadFlights();
    ptrDisplay->populateTable();  // Update display table as well
}

// Slot to update the graph in Shortest_Path
void MainWindow::updateGraph()
{
    ptrShortestPath->updateGraph();
}
