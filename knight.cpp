#include "Knight.h"

Knight::Knight(PieceColor color, int row, int col) : ChessPiece(ChessPiece::Knight, color, row, col) {
    QString filename = (color == White)? ":/images/wn.png" : ":/images/bn.png";
    QPixmap piecePixmap(filename);
    setPixmap(piecePixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QVector<QPair<int, int>> Knight::getValidMoves(const QVector<QVector<ChessPiece*>> &board) {
    QVector<QPair<int, int>> moves;
    const int row = getRow();
    const int col = getCol();

    const int jumps[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},  {2, -1},  {2, 1}
    };

    for(auto& move : jumps){
        int r = row + move[0];
        int c = col + move[1];

        if(r >= 0 && r < 8 && c >= 0 && c < 8){
            ChessPiece* target = board[r][c];
            if(target == nullptr || target->getColor() != getColor()){
                moves.append({r, c});
            }
        }
    }

    return moves;
}
