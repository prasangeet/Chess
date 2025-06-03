#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void onAbandonGame();
    void onTurnChanged(ChessPiece::PieceColor player);

private:
    Ui::MainWindow *ui;
    Board* chessBoard;   // Make this a member variable!
};

#endif // MAINWINDOW_H
