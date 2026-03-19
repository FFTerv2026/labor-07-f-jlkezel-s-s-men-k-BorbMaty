#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Stylesheet – warm, readable, motivating look
    app.setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e2e;
        }
        QWidget#central {
            background-color: #1e1e2e;
        }
        QLabel#quoteLabel {
            color: #cdd6f4;
            font-size: 20px;
            font-style: italic;
            padding: 30px;
            qproperty-alignment: AlignCenter;
        }
        QLabel#authorLabel {
            color: #89b4fa;
            font-size: 14px;
            font-weight: bold;
            qproperty-alignment: AlignCenter;
            margin-bottom: 20px;
        }
        QLabel#titleLabel {
            color: #f38ba8;
            font-size: 26px;
            font-weight: bold;
            qproperty-alignment: AlignCenter;
            padding: 20px 0 10px 0;
        }
        QPushButton#nextBtn {
            background-color: #89b4fa;
            color: #1e1e2e;
            font-size: 15px;
            font-weight: bold;
            border: none;
            border-radius: 10px;
            padding: 12px 30px;
            margin: 20px;
        }
        QPushButton#nextBtn:hover {
            background-color: #b4befe;
        }
        QPushButton#nextBtn:pressed {
            background-color: #74c7ec;
        }
        QFrame#card {
            background-color: #313244;
            border-radius: 16px;
            margin: 20px;
        }
    )");

    MainWindow w;
    w.show();
    return app.exec();
}
