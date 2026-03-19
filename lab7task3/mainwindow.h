#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QStringList>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onNextQuote();

private:
    QLabel      *quoteLabel;
    QLabel      *authorLabel;
    QPushButton *nextBtn;

    QStringList quotes;   // full "text — author" lines
    int         lastIndex = -1;

    void loadQuotes();
    void showQuote(int index);
    int  pickRandom() const;
};
