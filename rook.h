#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"
#include <QVector>
#include <QPair>

class Rook : public ChessPiece {
public:
    Rook(PieceColor color, int row, int col);
    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};

#endif // ROOK_H
