#include "King.h"

King::King(PieceColor color, int row, int col) : ChessPiece(ChessPiece::King, color, row, col) {
    QString filename = (color == White)? ":/images/wk.png" : ":/images/bk.png";
    QPixmap piecePixmap(filename);
    setPixmap(piecePixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QVector<QPair<int, int>> King::getValidMoves(const QVector<QVector<ChessPiece*>> &board) {
    QVector<QPair<int, int>> moves;
    const int row = getRow();
    const int col = getCol();

    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0}, {1, 1}
    };

    for(auto& dir : directions){
        int r = row + dir[0];
        int c = col + dir[1];
        if(r >= 0 && r < 8 && c >= 0 && c < 8){
            ChessPiece* target = board[r][c];
            if(target == nullptr || target->getColor() != getColor()){
                moves.append({r, c});
            }
        }
    }

    return moves;
}
