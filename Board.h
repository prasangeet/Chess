#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>  //Chess Board UI
#include <QGraphicsSceneMouseEvent> //Mouse Interactions
#include <QVector>
#include <QPair>  //Container for board and moves

#include "ChessPiece.h"

class Board : public QGraphicsScene {
    Q_OBJECT

public:
    Board(int border, int squareSize);
    ~Board();

    // Use ChessPiece::PieceColor instead of Player
    ChessPiece::PieceColor currentPlayer;

    void switchTurn();
    void clear();
    ChessPiece::PieceColor getCurrentPlayer() const;

    void setupInitialPosition();// Set up standard chess starting position
    void addPiece(ChessPiece* piece);
    ChessPiece* getPiece(int row, int col) const;

signals:
    void turnChanged(ChessPiece::PieceColor current);
    void checkmate(ChessPiece::PieceColor loser);

//Event Handlers
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QVector<ChessPiece*>> board;
    int border;
    int squareSize;
    QGraphicsRectItem* checkHighlight = nullptr;
    QGraphicsTextItem* checkLabel = nullptr;

    //Board Visualisation Parameters
    ChessPiece* selectedPiece = nullptr;
    QVector<QPair<int, int>> validMoves;
    QPointF originalPos;
    bool isDragging = false;

    //Move Legality Logic

    bool isLegalMove(ChessPiece* piece, int toRow, int toCol);
    QVector<QPair<int, int>> getLegalMoves(ChessPiece* piece);

    void movePiece(ChessPiece* piece, int newRow, int newCol);
    void highlightMoves(const QVector<QPair<int, int>> &moves);
    void clearHighlights();
    QGraphicsItem* pieceAt(int row, int col);

    void resetSelection() ;

    //Check Functions
    void highlightCheck(int row, int col);
    void clearCheckHighlight();
    void updateCheckHighlight();
    bool isInCheck(ChessPiece::PieceColor color) const;
    bool isCheckmate(ChessPiece::PieceColor color);

    // Castling moves
    QVector<QPair<int, int>> getCastlingMoves(ChessPiece* king);
    bool canCastle(ChessPiece::PieceColor color, bool kingside);
    bool isSquareAttacked(int row, int col, ChessPiece::PieceColor byColor) const;
    bool tryCastling(ChessPiece* king, int newRow, int newCol);
};

#endif
