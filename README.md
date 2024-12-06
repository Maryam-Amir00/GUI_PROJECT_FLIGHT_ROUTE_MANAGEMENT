# **Flight Route Management System**  

## **Overview**  
The **Flight Route Management System** is an interactive application designed for managing airports, flights, and routes efficiently. This C++ application, built using the Qt framework, integrates dynamic graph visualization, shortest path calculations, and real-time updates.  

---

## **Features**  

1. **Add Airport**  
   - Add airports to the database with specified names and locations.  
   - Nodes representing airports are dynamically added to the graph.  

2. **Add Flight**  
   - Establish flight routes between airports with costs calculated based on predefined distances.  
   - Dynamically update flights in the database and graph visualization.  

3. **Delete Airport**  
   - Remove an airport and automatically delete its associated flights.  
   - Updates the graph and interface in real time.  

4. **Delete Flight**  
   - Remove specific flights from the database and graph visualization.  
   - Reflects changes dynamically without requiring system restarts.  

5. **Shortest Path Calculation**  
   - Calculate and visualize the shortest path between two airports using **Dijkstra’s algorithm**.  
   - Highlights the shortest path in green on the graph.  

6. **Dynamic Graph Visualization**  
   - Displays an undirected graph of airports and flight routes using **QGraphicsView**.  
   - Automatically reflects all additions and deletions of nodes (airports) and edges (flights).  

---

### WorkFlow Diagram
The workflow below illustrates how the Project Works.

![WorkFlow](https://github.com/Maryam-Amir00/GUI_PROJECT_FLIGHT_ROUTE_MANAGEMENT/blob/main/Images/Workflow.png)
![WorkFlow](https://github.com/Maryam-Amir00/GUI_PROJECT_FLIGHT_ROUTE_MANAGEMENT/blob/main/Images/Workflow_2.png)


---

## **Technical Specifications**  

- **Programming Language**: C++  
- **Framework**: Qt (for GUI and dynamic visualization)  
- **Database**: SQLite (for efficient data storage and management)  
- **Algorithm**: Dijkstra’s Algorithm (for shortest path computation)  
- **Visualization**: QGraphicsView (for real-time graph updates)  

---

## **How to Run the Project**  

### **Prerequisites**  
1. Install **Qt Creator** and set up the development environment.  
2. Ensure **SQLite** is supported within your Qt installation.  

### **Steps**  
1. Clone the repository:  
   ```bash  
   git clone https://github.com/<your-username>/FlightRouteManagementSystem.git  
   ```  
2. Open the `.pro` file in Qt Creator.  
3. Build and run the project.  
4. Use the intuitive graphical interface to manage airports, flights, and routes.  

---

## **Usage Scenarios**  

- **Airline Companies**: Manage and optimize flight routes and schedules.  
- **Travel Agencies**: Provide customers with the most efficient travel plans.  
- **Airport Management**: Plan and organize airport operations effectively.  

---

## **Benefits**  

- **Real-Time Updates**: Reflects all changes immediately.  
- **Efficiency**: Simplifies flight and airport management.  
- **Scalability**: Handles increasing amounts of data for larger operations.  
- **User-Friendly Interface**: Easy-to-use GUI for effective management.  

---

## **Future Enhancements**  

- Add support for international distances using geolocation.  
- Enable historical route analysis and reporting.  
- Implement flight scheduling and capacity management.  

---

## **Contributing**  

Contributions are welcome! Follow these steps:  

1. Fork the repository.  
2. Create a feature branch:  
   ```bash  
   git checkout -b feature-name  
   ```  
3. Commit your changes:  
   ```bash  
   git commit -m "Add feature-name"  
   ```  
4. Push to the branch:  
   ```bash  
   git push origin feature-name  
   ```  
5. Create a pull request.  

---

## **License**  

This project is licensed under the [MIT License](LICENSE).  
