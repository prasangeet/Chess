#include "Rook.h"
#include "ChessPiece.h"

Rook::Rook(PieceColor color, int row, int col) : ChessPiece(ChessPiece::Rook, color, row, col) {
    QString filename = (color == White)? ":/images/wr.png" : ":/images/br.png";
    QPixmap piecePixmap(filename);
    setPixmap(piecePixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QVector<QPair<int, int>> Rook::getValidMoves(const QVector<QVector<ChessPiece*>> &board) {
    QVector<QPair<int, int>> moves;
    const int row = getRow();
    const int col = getCol();

    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // vertical and horizontal

    for(auto& dir : directions){
        int r = row + dir[0];
        int c = col + dir[1];

        while(r >= 0 && r < 8 && c >= 0 && c < 8){
            ChessPiece* target = board[r][c];
            if(target == nullptr){
                moves.append({r, c});
            } else {
                if(target->getColor() != getColor()){
                    moves.append({r, c});
                }
                break;
            }
            r += dir[0];
            c += dir[1];
        }
    }

    return moves;
}
