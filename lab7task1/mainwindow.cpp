#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QProgressDialog>
#include <QHeaderView>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("File Searcher");
    resize(750, 500);

    QWidget     *central = new QWidget(this);
    QVBoxLayout *vlay    = new QVBoxLayout(central);

    // Directory row
    QHBoxLayout *dirRow = new QHBoxLayout;
    dirEdit  = new QLineEdit(QDir::currentPath(), this);
    browseBtn = new QPushButton("Browse...", this);
    dirRow->addWidget(new QLabel("Directory:", this));
    dirRow->addWidget(dirEdit, 1);
    dirRow->addWidget(browseBtn);

    // Search row
    QHBoxLayout *searchRow = new QHBoxLayout;
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search text (empty = all files)");
    searchBtn  = new QPushButton("Search", this);
    searchRow->addWidget(new QLabel("Search:", this));
    searchRow->addWidget(searchEdit, 1);
    searchRow->addWidget(searchBtn);

    // Table
    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(2);
    resultsTable->setHorizontalHeaderLabels({"File Name", "Size (bytes)"});
    resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    statusLabel = new QLabel("Ready.", this);

    vlay->addLayout(dirRow);
    vlay->addLayout(searchRow);
    vlay->addWidget(resultsTable);
    vlay->addWidget(statusLabel);

    setCentralWidget(central);

    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowse);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearch);
}

void MainWindow::onBrowse()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, "Select Directory", dirEdit->text());
    if (!dir.isEmpty())
        dirEdit->setText(dir);
}

void MainWindow::onSearch()
{
    searchFiles(dirEdit->text(), searchEdit->text());
}

void MainWindow::searchFiles(const QString &dirPath, const QString &term)
{
    resultsTable->setRowCount(0);

    QDir dir(dirPath);
    if (!dir.exists()) {
        statusLabel->setText("Directory does not exist.");
        return;
    }

    // Collect all files first (for progress)
    QStringList allFiles;
    QDirIterator it(dirPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        allFiles << it.filePath();
    }

    QProgressDialog progress("Searching files...", "Cancel", 0, allFiles.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);

    int found = 0;
    for (int i = 0; i < allFiles.size(); ++i) {
        progress.setValue(i);
        QApplication::processEvents();
        if (progress.wasCanceled()) break;

        const QString &filePath = allFiles[i];

        bool matches = false;

        if (term.isEmpty()) {
            matches = true;
        } else {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                while (!in.atEnd()) {
                    if (in.readLine().contains(term, Qt::CaseInsensitive)) {
                        matches = true;
                        break;
                    }
                }
                file.close();
            }
        }

        if (matches) {
            QFileInfo info(filePath);
            int row = resultsTable->rowCount();
            resultsTable->insertRow(row);
            resultsTable->setItem(row, 0, new QTableWidgetItem(info.absoluteFilePath()));
            resultsTable->setItem(row, 1, new QTableWidgetItem(
                                              QString::number(info.size())));
            ++found;
        }
    }

    progress.setValue(allFiles.size());
    statusLabel->setText(QString("Found %1 file(s) out of %2 scanned.")
                             .arg(found).arg(allFiles.size()));
}
