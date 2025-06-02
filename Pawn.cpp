#include"Pawn.h"
#include "ChessPiece.h"

Pawn::Pawn(PieceColor color, int row, int col)
    : ChessPiece(ChessPiece::Pawn, color, row, col)
{
    QString filename = (color == White) ? ":/images/wp.png" : ":/images/bp.png";
    QPixmap piecePixmap(filename);
    QPixmap scaled = piecePixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(scaled);
}

QVector<QPair<int, int>> Pawn::getValidMoves(const QVector<QVector<ChessPiece*>> &board) {
    QVector<QPair<int, int>> moves;

    int dir = (getColor() == White) ? -1 : 1;
    int startRow = (getColor() == White) ? 6 : 1;

    int row = getRow();
    int col = getCol();

    int nextRow = row + dir;

    // Check one step forward
    if (nextRow >= 0 && nextRow < 8) {
        if (board[nextRow][col] == nullptr) {
            moves.append({nextRow, col});

            // Check two steps forward from start
            int twoStepRow = row + 2 * dir;
            if (row == startRow && twoStepRow >= 0 && twoStepRow < 8 && board[twoStepRow][col] == nullptr)
                moves.append({twoStepRow, col});
        }

        // Diagonal captures
        for (int dcol = -1; dcol <= 1; dcol += 2) {
            int c = col + dcol;
            if (c >= 0 && c < 8) {
                ChessPiece* target = board[nextRow][c];
                if (target && target->getColor() != getColor())
                    moves.append({nextRow, c});
            }
        }
    }

    return moves;
}
