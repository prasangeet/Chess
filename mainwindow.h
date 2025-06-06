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
    void onCheckmate(ChessPiece::PieceColor loser);

private:
    Ui::MainWindow *ui;
    Board* chessBoard;   // Make this a member variable!
private:
    void updateStatusLabel(const QString& text,
                           const QString& color,
                           const QString& bgColor,
                           const QString& borderColor,
                           int fontSize = 16,
                           int borderWidth = 2,
                           int borderRadius = 8,
                           int padding = 8,
                           int fontWeight = 600);

};

#endif // MAINWINDOW_H
