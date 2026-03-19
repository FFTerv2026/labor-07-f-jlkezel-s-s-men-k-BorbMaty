#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CSV Viewer");
    resize(800, 500);

    // Menu bar
    QMenu   *fileMenu   = menuBar()->addMenu("File");
    QAction *openAction = fileMenu->addAction("Open File...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenFile);

    // Table (read-only)
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setCentralWidget(table);

    statusBar()->showMessage("Use File → Open File... to load a CSV.");
}

void MainWindow::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open CSV File", QDir::homePath(),
        "CSV Files (*.csv);;All Files (*)");

    if (!path.isEmpty())
        loadCSV(path);
}

void MainWindow::loadCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", file.errorString());
        return;
    }

    table->clear();
    table->setRowCount(0);
    table->setColumnCount(0);

    QTextStream in(&file);
    bool firstLine = true;
    int  row       = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        // Handle quoted fields with commas inside (basic CSV)
        QStringList fields;
        QString     field;
        bool        inQuotes = false;
        for (QChar ch : line) {
            if (ch == '"') {
                inQuotes = !inQuotes;
            } else if (ch == ',' && !inQuotes) {
                fields << field.trimmed();
                field.clear();
            } else {
                field += ch;
            }
        }
        fields << field.trimmed(); // last field

        if (firstLine) {
            table->setColumnCount(fields.size());
            table->setHorizontalHeaderLabels(fields);
            firstLine = false;
            continue;
        }

        table->insertRow(row);
        for (int col = 0; col < fields.size() && col < table->columnCount(); ++col) {
            table->setItem(row, col, new QTableWidgetItem(fields[col]));
        }
        ++row;
    }

    file.close();
    statusBar()->showMessage(
        QString("Loaded: %1  |  %2 rows, %3 columns")
            .arg(filePath).arg(row).arg(table->columnCount()));
}
