#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const int border = 8;
    const int squareSize = 96;

    // 🔹 Setup Chess Board
    chessBoard = new Board(border, squareSize);
    chessBoard->setupInitialPosition();

    ui->graphicsView->setScene(chessBoard);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFrameStyle(QFrame::NoFrame);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    const int boardSize = squareSize * 8 + border * 2;
    ui->graphicsView->setSceneRect(0, 0, boardSize, boardSize);
    ui->graphicsView->setFixedSize(boardSize, boardSize);

    // 🔹 Add Chess Icon (Chess.com logo) above Start Button
    QLabel* chessIconLabel = new QLabel(this);
    QPixmap chessIcon(":/images/chesscom_logo.png");
    chessIconLabel->setPixmap(chessIcon.scaled(300,120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    chessIconLabel->setAlignment(Qt::AlignCenter);
    chessIconLabel->setFixedSize(400, 180);
    ui->verticalLayout->insertWidget(0, chessIconLabel);  // insert at top of layout

    // Optional: Add spacing if layout looks tight
    ui->verticalLayout->setSpacing(15);

    // 🔹 Style Buttons
    ui->startButton->setText("Start Game");
    ui->startButton->setStyleSheet(
        "QPushButton {"
        " background-color: #4CAF50;"
        " color: white;"
        " font-weight: bold;"
        " font-size: 18px;"
        " padding: 10px 20px;"
        " border-radius: 8px;"
        " }"
        "QPushButton:hover { background-color: #45a049; }"
        );

    ui->abandonButton->setText("Restart");
    ui->abandonButton->setStyleSheet(
        "QPushButton {"
        " background-color: orange;"
        " color: white;"
        " font-weight: bold;"
        " font-size: 16px;"
        " padding: 10px 20px;"
        " border-radius: 8px;"
        " }"
        "QPushButton:hover { background-color: #e69500; }"
        );

    // Disable abandon button initially until game starts
    ui->abandonButton->setEnabled(false);

    // 🔹 Modern Status Label
    ui->statusLabel->setAlignment(Qt::AlignCenter);
    updateStatusLabel("Game Ready 🎯", "#2c3e50", "#ecf0f1", "#bdc3c7");

    // 🔹 Signal-Slot Connections
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
    connect(ui->abandonButton, &QPushButton::clicked, this, &MainWindow::onAbandonGame);
    connect(chessBoard, &Board::turnChanged, this, &MainWindow::onTurnChanged);
    connect(chessBoard, &Board::checkmate, this, &MainWindow::onCheckmate);

    onTurnChanged(chessBoard->getCurrentPlayer());
}

MainWindow::~MainWindow() {
    delete chessBoard;
    delete ui;
}

// Helper to update status label with consistent style
void MainWindow::updateStatusLabel(const QString& text,
                                   const QString& color,
                                   const QString& bgColor,
                                   const QString& borderColor,
                                   int fontSize,
                                   int borderWidth,
                                   int borderRadius,
                                   int padding,
                                   int fontWeight)
{
    ui->statusLabel->setText(text);
    QString style = QString(
                        "QLabel {"
                        " font-size: %1px;"
                        " color: %2;"
                        " background-color: %3;"
                        " border: %4px solid %5;"
                        " border-radius: %6px;"
                        " padding: %7px;"
                        " font-weight: %8;"
                        "}").arg(fontSize)
                        .arg(color)
                        .arg(bgColor)
                        .arg(borderWidth)
                        .arg(borderColor)
                        .arg(borderRadius)
                        .arg(padding)
                        .arg(fontWeight);
    ui->statusLabel->setStyleSheet(style);
}

void MainWindow::onStartGame() {
    ui->graphicsView->setEnabled(true);
    chessBoard->clear();
    chessBoard->setupInitialPosition();
    onTurnChanged(chessBoard->getCurrentPlayer());

    updateStatusLabel("Game Started ✅", "green", "#eafaf1", "green", 20, 2, 10, 10, 700);

    ui->abandonButton->setEnabled(true);
}

void MainWindow::onAbandonGame() {
    auto reply = QMessageBox::question(this, "Confirm Restart", "Are you sure you want to restart the game?");
    if (reply != QMessageBox::Yes)
        return;

    chessBoard->clear();
    ui->graphicsView->setEnabled(false);

    updateStatusLabel("Game Restarted 🔄", "orange", "#fff3e0", "orange", 20, 2, 10, 10, 700);

    QMessageBox::information(this, "Game Restarted", "The game has been reset.");

    ui->abandonButton->setEnabled(false);
}

void MainWindow::onTurnChanged(ChessPiece::PieceColor player) {
    QString playerText = (player == ChessPiece::PieceColor::White) ? "White" : "Black";
    updateStatusLabel(playerText + "'s Turn ⚔️", "#2c3e50", "#ecf0f1", "#bdc3c7", 20, 2, 10, 10, 700);
}

void MainWindow::onCheckmate(ChessPiece::PieceColor loser) {
    QString winnerText = (loser == ChessPiece::PieceColor::White) ? "Black" : "White";
    QString resultMsg = "🏆 " + winnerText + " Wins\n" ;

    updateStatusLabel(resultMsg, "#f1c40f", "#1a1a2e", "#16213e", 26, 3, 15, 15, 900);


    QMessageBox msgBox;
    msgBox.setWindowTitle("🏆 Checkmate!");
    msgBox.setText(
        "<div style='color:#e74c3c; font-size:28px; font-weight:bold;'>CHECKMATE! 🛡️</div>"
        "<div style='font-size:20px; color:#2c3e50;'>" + winnerText + " wins the game 🎉</div>"
        );
    msgBox.setIconPixmap(QPixmap(":/images/trophy.png").scaled(120, 120, Qt::KeepAspectRatio));
    msgBox.exec();

    ui->graphicsView->setEnabled(false);
    ui->abandonButton->setEnabled(false);
}
