#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Daily Motivation");
    setMinimumSize(600, 380);
    resize(700, 420);

    QWidget *central = new QWidget(this);
    central->setObjectName("central");
    QVBoxLayout *vlay = new QVBoxLayout(central);
    vlay->setContentsMargins(0, 0, 0, 0);

    // Title
    QLabel *titleLabel = new QLabel("✨ Daily Quote", central);
    titleLabel->setObjectName("titleLabel");
    vlay->addWidget(titleLabel);

    // Card frame
    QFrame *card = new QFrame(central);
    card->setObjectName("card");
    QVBoxLayout *cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(20, 20, 20, 20);
    cardLay->setSpacing(12);

    quoteLabel = new QLabel(card);
    quoteLabel->setObjectName("quoteLabel");
    quoteLabel->setWordWrap(true);
    quoteLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    authorLabel = new QLabel(card);
    authorLabel->setObjectName("authorLabel");

    cardLay->addWidget(quoteLabel);
    cardLay->addWidget(authorLabel);

    vlay->addWidget(card, 1);

    // Button row
    QHBoxLayout *btnRow = new QHBoxLayout;
    nextBtn = new QPushButton("Next Quote →", central);
    nextBtn->setObjectName("nextBtn");
    btnRow->addStretch();
    btnRow->addWidget(nextBtn);
    btnRow->addStretch();
    vlay->addLayout(btnRow);

    setCentralWidget(central);

    loadQuotes();

    if (!quotes.isEmpty()) {
        int idx = pickRandom();
        lastIndex = idx;
        showQuote(idx);
    }

    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::onNextQuote);
}

void MainWindow::loadQuotes()
{
    // Try loading from quotes.txt next to the executable
    QFile file("quotes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Fallback: a handful of built-in quotes
        quotes = {
            "The only way to do great work is to love what you do.|Steve Jobs",
            "In the middle of every difficulty lies opportunity.|Albert Einstein",
            "It does not matter how slowly you go as long as you do not stop.|Confucius",
            "Life is what happens when you're busy making other plans.|John Lennon",
            "The future belongs to those who believe in the beauty of their dreams.|Eleanor Roosevelt",
            "Strive not to be a success, but rather to be of value.|Albert Einstein",
            "You miss 100% of the shots you don't take.|Wayne Gretzky",
            "Whether you think you can or you think you can't, you're right.|Henry Ford"
        };
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
            quotes << line;
    }
    file.close();
}

// The quotes.txt from the gist has lines like:
//   "Quote text" - Author Name
// We split on " - " to separate quote from author.
void MainWindow::showQuote(int index)
{
    if (index < 0 || index >= quotes.size()) return;

    QString line = quotes[index];

    // Support two formats:
    // 1. Built-in:  text|Author
    // 2. File:      "text" - Author   (or just plain text)
    QString text, author;

    if (line.contains('|')) {
        auto parts = line.split('|');
        text   = parts[0].trimmed();
        author = parts.size() > 1 ? parts[1].trimmed() : "";
    } else {
        // Try splitting on last " - "
        int dashPos = line.lastIndexOf(" - ");
        if (dashPos != -1) {
            text   = line.left(dashPos).trimmed().remove('"');
            author = line.mid(dashPos + 3).trimmed();
        } else {
            text   = line;
            author = "";
        }
    }

    quoteLabel->setText(QString("\u201c%1\u201d").arg(text));   // "text"
    authorLabel->setText(author.isEmpty() ? "" : QString("— %1").arg(author));
}

int MainWindow::pickRandom() const
{
    if (quotes.size() == 1) return 0;
    int idx;
    do {
        idx = static_cast<int>(
            QRandomGenerator::global()->bounded(quotes.size()));
    } while (idx == lastIndex);
    return idx;
}

void MainWindow::onNextQuote()
{
    if (quotes.isEmpty()) return;
    int idx = pickRandom();
    lastIndex = idx;
    showQuote(idx);
}
