#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const int border = 8;
    const int squareSize = 96;

    chessBoard = new Board(border, squareSize);
    chessBoard->setupInitialPosition();


    ui->graphicsView->setScene(chessBoard);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Remove extra margins that can force scrollbars
    ui->graphicsView->setFrameStyle(QFrame::NoFrame);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Set fixed size exactly equal to scene
    const int boardSize = 96 * 8 + 8 * 2;  // squareSize * 8 + border * 2
    ui->graphicsView->setSceneRect(0, 0, boardSize, boardSize);
    ui->graphicsView->setFixedSize(boardSize, boardSize);




    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
    connect(ui->abandonButton, &QPushButton::clicked, this, &MainWindow::onAbandonGame);
    connect(chessBoard, &Board::turnChanged, this, &MainWindow::onTurnChanged);
    connect(chessBoard, &Board::checkmate, this, &MainWindow::onCheckmate);

    onTurnChanged(chessBoard->getCurrentPlayer());
}

void MainWindow::onStartGame() {
    ui->graphicsView->setEnabled(true);
    chessBoard->clear();
    chessBoard->setupInitialPosition();
    onTurnChanged(chessBoard->getCurrentPlayer());
}

void MainWindow::onAbandonGame() {
    chessBoard->clear();
    ui->turnLabel->setText("Current Turn: -");
    QMessageBox::information(this, "Game Over", "You abandoned the game.");
}

void MainWindow::onTurnChanged(ChessPiece::PieceColor player) {
    QString playerText = (player == ChessPiece::PieceColor::White) ? "White" : "Black";
    ui->turnLabel->setText("Current Turn: " + playerText);
}

void MainWindow::onCheckmate(ChessPiece::PieceColor loser){
    QString loserText = (loser == ChessPiece::PieceColor::White) ? "White" : "Black";
    QString winnerText = (loser == ChessPiece::PieceColor::White) ? "Black" : "White";

    ui->turnLabel->setText("Checkmate! " + winnerText + " wins.");
    QMessageBox::information(this, "Checkmate", "Checkmate! " + winnerText + " wins.");

    // Optionally disable further interaction
    ui->graphicsView->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete chessBoard;  // Clean up explicitly
    delete ui;
}
