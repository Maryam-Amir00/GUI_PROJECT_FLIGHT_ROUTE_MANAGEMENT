#ifndef DISPLAY_H
#define DISPLAY_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>

namespace Ui {
class Display;
}

class Display : public QDialog
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = nullptr);
    ~Display();

    void populateTable(); // Method to refresh the table

private slots:
    void on_back_button_clicked();

public slots:
    void refreshData(); // New public slot to refresh data on signal

private:
    void setupUI();
    Ui::Display *ui;
    QSqlQueryModel *model;
    QLabel *label;
    QTableView *tableView;
    QPushButton *backButton;
};

#endif // DISPLAY_H
