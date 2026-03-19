#pragma once

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onOpenFile();

private:
    QTableWidget *table;

    void loadCSV(const QString &filePath);
};
