#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "Board.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const int border = 8;
    const int squareSize = 96;

    Board* chessBoard = new Board(border, squareSize);
    chessBoard->setupInitialPosition();

    ui->graphicsView->setScene(chessBoard);
    ui->graphicsView->setFixedSize(784 + 2, 784 + 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
