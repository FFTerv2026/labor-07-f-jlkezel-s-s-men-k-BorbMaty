#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onBrowse();
    void onSearch();

private:
    QLineEdit   *dirEdit;
    QLineEdit   *searchEdit;
    QPushButton *browseBtn;
    QPushButton *searchBtn;
    QTableWidget *resultsTable;
    QLabel      *statusLabel;

    void searchFiles(const QString &dir, const QString &term);
};
