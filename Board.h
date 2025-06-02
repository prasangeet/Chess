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

    void setupInitialPosition();
    void addPiece(ChessPiece* piece);
    ChessPiece* getPiece(int row, int col) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QVector<ChessPiece*>> board;
    int border;
    int squareSize;

    ChessPiece* selectedPiece = nullptr;
    QVector<QPair<int, int>> validMoves;
    QPointF originalPos;
    bool isDragging = false;

    void movePiece(ChessPiece* piece, int newRow, int newCol);
    void highlightMoves(const QVector<QPair<int, int>> &moves);
    void clearHighlights();
};

#endif // BOARD_H
