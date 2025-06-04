#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QPair>
#include "ChessPiece.h"

class Board : public QGraphicsScene {
    Q_OBJECT

public:
    Board(int border, int squareSize);
    ~Board();

    // Use ChessPiece::PieceColor instead of Player
    ChessPiece::PieceColor currentPlayer;

    void switchTurn();
    void clear();
    ChessPiece::PieceColor getCurrentPlayer() const;

    void setupInitialPosition();
    void addPiece(ChessPiece* piece);
    ChessPiece* getPiece(int row, int col) const;

signals:
    void turnChanged(ChessPiece::PieceColor current);
    void checkmate(ChessPiece::PieceColor loser);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QVector<ChessPiece*>> board;
    int border;
    int squareSize;
    QGraphicsRectItem* checkHighlight = nullptr;

    ChessPiece* selectedPiece = nullptr;
    QVector<QPair<int, int>> validMoves;
    QPointF originalPos;
    bool isDragging = false;

    bool isLegalMove(ChessPiece* piece, int toRow, int toCol);
    QVector<QPair<int, int>> getLegalMoves(ChessPiece* piece);


    void movePiece(ChessPiece* piece, int newRow, int newCol);
    void highlightMoves(const QVector<QPair<int, int>> &moves);
    void clearHighlights();

    void highlightCheck(int row, int col);
    void clearCheckHighlight();

    bool isInCheck(ChessPiece::PieceColor color) const;
    bool isCheckmate(ChessPiece::PieceColor color);
};

#endif // BOARD_H
