#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"
#include <QVector>
#include <QPair>

class Queen : public ChessPiece {
public:
    Queen(PieceColor color, int row, int col);
    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};

#endif // QUEEN_H
