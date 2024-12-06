#include "shortest_path.h"
#include "ui_shortest_path.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QtMath>
#include <limits>
#include <queue>
#include <QGraphicsTextItem>

// Constructor
Shortest_Path::Shortest_Path(QWidget *parent)
    : QDialog(parent), ui(new Ui::Shortest_Path), scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    populateComboBoxes();
    loadGraph();
    drawAirportsAndFlights();
}

// Destructor
Shortest_Path::~Shortest_Path()
{
    delete ui;
}

void Shortest_Path::populateComboBoxes()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();

    // Query the database for airports
    QSqlQuery query("SELECT airport_id, name FROM Airports");
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    // Populate combo boxes with new airport data
    while (query.next()) {
        int airportID = query.value(0).toInt();
        QString airportName = query.value(1).toString();
        ui->comboBox->addItem(airportName, airportID);
        ui->comboBox_2->addItem(airportName, airportID);
    }
}


void Shortest_Path::loadGraph()
{
    graph.clear();
    airportPositions.clear();

    QSqlQuery query("SELECT airport_id FROM Airports");
    if (!query.exec()) {
        QMessageBox::critical(this, "Query Error", query.lastError().text());
        return;
    }

    QVector<int> allAirports;

    // Load all airports
    while (query.next()) {
        int airportID = query.value(0).toInt();
        allAirports.append(airportID);
        graph[airportID] = {};
        airportPositions[airportID] = QPointF();  // Initialize positions
    }

    // Fully connect the graph
    for (int i = 0; i < allAirports.size(); ++i) {
        for (int j = i + 1; j < allAirports.size(); ++j) {
            int source = allAirports[i];
            int dest = allAirports[j];
            graph[source][dest] = 1.0;  // Example distance, assuming uniform distance
            graph[dest][source] = 1.0;
        }
    }
}

void Shortest_Path::drawAirportsAndFlights()
{
    scene->clear(); // Clear existing items in the scene

    // Set dark background for the graphics view
    ui->graphicsView->setStyleSheet("background-color: #2E2E2E; border: none;");

    int centerX = ui->graphicsView->width() / 2;
    int centerY = ui->graphicsView->height() / 2;
    int radius = qMin(centerX, centerY) - 50; // Radius for circular layout
    int airportCount = airportPositions.size();
    double angleStep = 360.0 / qMax(airportCount, 1); // Avoid division by zero

    QMap<int, QString> airportNames;

    // Query the database to fetch airport names
    QSqlQuery query("SELECT airport_id, name FROM Airports");
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    while (query.next()) {
        int airportID = query.value(0).toInt();
        QString airportName = query.value(1).toString();
        airportNames[airportID] = airportName;
    }

    double angle = 0;

    // Draw airport nodes and labels
    for (int airport : airportPositions.keys()) {
        // Calculate position based on circular layout
        double radians = qDegreesToRadians(angle);
        QPointF pos(centerX + radius * qCos(radians), centerY + radius * qSin(radians));
        airportPositions[airport] = pos;

        // Draw node as a blue circle
        QGraphicsEllipseItem *node = scene->addEllipse(
            pos.x() - 15, pos.y() - 15, 30, 30, QPen(Qt::NoPen), QBrush(Qt::blue));

        // Add airport name label centered above the node
        QGraphicsTextItem *textItem = scene->addText(airportNames[airport], QFont("Arial", 10, QFont::Bold));
        textItem->setDefaultTextColor(Qt::white);
        textItem->setPos(pos.x() - textItem->boundingRect().width() / 2, pos.y() - 30); // Center above the node

        angle += angleStep; // Increment angle for next node
    }

    // Draw edges (flights) with white pen
    QSet<QPair<int, int>> drawnEdges;
    for (int source : graph.keys()) {
        for (int dest : graph[source].keys()) {
            QPair<int, int> edge = qMakePair(qMin(source, dest), qMax(source, dest));
            if (!drawnEdges.contains(edge)) {
                // Draw a thin white line between source and destination
                scene->addLine(
                    QLineF(airportPositions[source], airportPositions[dest]), QPen(Qt::white, 2));
                drawnEdges.insert(edge);
            }
        }
    }

    // Adjust the scene rectangle to fit all elements
    scene->setSceneRect(scene->itemsBoundingRect());

    // Fit the scene into the graphics view while maintaining aspect ratio
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


// Find the shortest path using Dijkstra's algorithm
QVector<int> Shortest_Path::findShortestPath(int source, int destination) {
    QMap<int, double> distances;
    QMap<int, int> previous;
    QSet<int> visited;

    for (int airport : graph.keys()) {
        distances[airport] = std::numeric_limits<double>::infinity();
    }
    distances[source] = 0;

    using NodeDist = QPair<double, int>;  // {distance, node}
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> queue;
    queue.push({0, source});

    while (!queue.empty()) {
        NodeDist current = queue.top();
        queue.pop();
        double currentDist = current.first;
        int currentNode = current.second;
        if (visited.contains(currentNode)) continue;
        visited.insert(currentNode);

        for (auto neighbor : graph[currentNode].keys()) {
            double newDist = currentDist + graph[currentNode][neighbor];
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = currentNode;
                queue.push({newDist, neighbor});
            }
        }
    }

    QVector<int> path;
    for (int at = destination; at != source; at = previous.value(at, source)) {
        path.prepend(at);
        if (!previous.contains(at)) return {};  // No path exists
    }
    path.prepend(source);
    return path;
}

// Highlight the shortest path on the QGraphicsView
void Shortest_Path::highlightShortestPath(const QVector<int> &path) {
    for (int i = 0; i < path.size() - 1; ++i) {
        QLineF line(airportPositions[path[i]], airportPositions[path[i + 1]]);
        scene->addLine(line, QPen(Qt::green, 3));  // Highlight path in green
    }

    // Optionally, add labels for each step in the path
    for (int airport : path) {
        QGraphicsTextItem *text = scene->addText(QString::number(airport));
        QPointF pos = airportPositions[airport];
        text->setPos(pos.x() + 10, pos.y() + 10);  // Label position adjustment
    }
}

// Slot for calculate button
void Shortest_Path::on_calculate_button_clicked()
{
    int sourceID = ui->comboBox->currentData().toInt();
    int destID = ui->comboBox_2->currentData().toInt();

    QVector<int> path = findShortestPath(sourceID, destID);

    if (path.isEmpty()) {
        QMessageBox::information(this, "Path Error", "No path found between the selected airports.");
    } else {
        highlightShortestPath(path);
    }
}

// Slot for cancel button
void Shortest_Path::on_cancel_button_clicked()
{
    this->close();
}

// Slot to update the graph when a new airport is added
void Shortest_Path::updateGraph()
{
    populateComboBoxes();
    loadGraph();
    drawAirportsAndFlights();
}
