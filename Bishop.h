#ifndef BISHOP_H
#define BISHOP_H
#include "ChessPiece.h"
#include<QVector>
#include<QPair>

class Bishop : public ChessPiece{
public:
    Bishop(PieceColor color, int row, int col);

    QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) override;
};


#endif // BISHOP_H
