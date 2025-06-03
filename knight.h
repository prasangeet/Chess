#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"
#include <QVector>
#include <QPair>

class Knight : public ChessPiece {
public:
    Knight(PieceColor color, int row, int col);
    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};

#endif // KNIGHT_H
