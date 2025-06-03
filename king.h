#ifndef KING_H
#define KING_H

#include "ChessPiece.h"
#include <QVector>
#include <QPair>

class King : public ChessPiece {
public:
    King(PieceColor color, int row, int col);
    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};

#endif // KING_H
