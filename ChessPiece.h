#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <QVector>
#include <QPair>

class ChessPiece : public QGraphicsPixmapItem {
public:
    enum PieceType {King, Queen, Rook, Bishop, Knight, Pawn, None};
    enum PieceColor {White, Black, NoColor};

    ChessPiece(PieceType type, PieceColor color, int row, int col, QGraphicsItem* parent = nullptr);

    PieceType getType() const;
    PieceColor getColor() const;
    int getRow() const;
    int getCol() const;

    void setBoardPosition(int row, int col);
    void updateGraphicsPosition(int border, int squareSize);

    virtual QVector<QPair<int, int>> getValidMoves(const QVector<QVector<ChessPiece*>> &board) = 0;

    bool hasMoved() const;
    void markMoved();

private:
    PieceType m_type;
    PieceColor m_color;
    int m_row;
    int m_col;

    // track if the piece has moved
    bool m_hasMoved = false;
};

#endif // CHESSPIECE_H
