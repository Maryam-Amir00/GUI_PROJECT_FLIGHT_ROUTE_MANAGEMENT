#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "add_airport.h"
#include "add_flight.h"
#include "delete_airport.h"
#include "delete_flight.h"
#include "shortest_path.h"
#include "display.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_Airport_clicked();
    void on_add_flight_clicked();
    void on_delete_airport_clicked();
    void on_delete_flight_clicked();
    void on_shortest_path_clicked();
    void on_display_clicked();
    void on_exit_clicked();

    void updateComboBoxes();  // Slot to update combo boxes
    void updateFlights();     // Slot to update flight lists
    void updateGraph(); // Slot to update the graph in Shortest_Path

private:
    Ui::MainWindow *ui;
    Add_Airport *ptrAddAirport;
    Add_Flight *ptrAddFlight;
    Delete_Airport *ptrDeleteAirport;
    Delete_Flight *ptrDeleteFlight;
    Shortest_Path *ptrShortestPath;
    Display *ptrDisplay;

    bool setupDatabase();
};

#endif // MAINWINDOW_H
