#ifndef PAWN_H
#define PAWN_H
#include "ChessPiece.h"
#include <QVector>
#include <QPair>

class Pawn : public ChessPiece {
public:
    Pawn(PieceColor color, int row, int col);

    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};


#endif // PAWN_H
