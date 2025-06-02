#include "Bishop.h"
#include "ChessPiece.h"

Bishop::Bishop(PieceColor color, int row, int col): ChessPiece(ChessPiece::Bishop, color, row, col) {
    QString filename = (color == White)? ":/images/wb.png" : ":/images/bb.png";
    QPixmap piecePixmap(filename);
    QPixmap scaled = piecePixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(scaled);
}

QVector<QPair<int, int>> Bishop::getValidMoves(const QVector<QVector<ChessPiece*>> &board){
    QVector<QPair<int, int>> moves;

    const int row = getRow();
    const int col = getCol();

    const int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for(auto& dir : directions){
        int r = row + dir[0];
        int c = col + dir[1];

        while(r >= 0 && r < 8 && c >= 0 && c < 8){
            ChessPiece* target = board[r][c];

            if(target == nullptr){
                moves.append({r, c});
            }else {
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
