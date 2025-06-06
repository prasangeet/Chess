//represent a single chess piece on a graphics-based chessboard

#include "ChessPiece.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, int row, int col, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), m_type(type), m_color(color), m_row(row), m_col(col)
{
    // You can load pixmaps here based on type/color or do that in MainWindow
}

ChessPiece::PieceType ChessPiece::getType() const {return m_type;}
ChessPiece::PieceColor ChessPiece::getColor() const {return m_color;}
int ChessPiece::getRow() const {return m_row;}
int ChessPiece::getCol() const {return m_col;}

void ChessPiece::setBoardPosition(int row, int col){
    m_row = row;
    m_col = col;
}
void ChessPiece::updateGraphicsPosition(int border, int squareSize){
    setPos(border + m_col * squareSize, border + m_row * squareSize);
}
