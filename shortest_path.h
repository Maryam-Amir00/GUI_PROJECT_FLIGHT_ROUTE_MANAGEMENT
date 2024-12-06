#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include <QDialog>
#include <QGraphicsScene>
#include <QMap>
#include <QVector>
#include <QPointF>

namespace Ui {
class Shortest_Path;
}

class Shortest_Path : public QDialog
{
    Q_OBJECT

public:
    explicit Shortest_Path(QWidget *parent = nullptr);
    ~Shortest_Path();

    void populateComboBoxes();  // Make this method public
    double calculateDistance(int source, int dest);

public slots:
    void updateGraph();  // Slot to update the graph when new airport is added

private slots:
    void on_calculate_button_clicked();
    void on_cancel_button_clicked();

private:
    Ui::Shortest_Path *ui;
    QGraphicsScene *scene;
    QMap<int, QMap<int, double>> graph;
    QMap<int, QPointF> airportPositions;

    void loadGraph();
    void drawAirportsAndFlights();
    QVector<int> findShortestPath(int start, int end);
    void highlightShortestPath(const QVector<int> &path);
};

#endif // SHORTEST_PATH_H
