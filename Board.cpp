// Custom game logic
#include "Board.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
// Qt graphics and utilities
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QBrush>
#include <QPixmap>
#include <QDebug>

Board::Board(int border, int squareSize)
    : border(border), squareSize(squareSize) {

    currentPlayer = ChessPiece::White;

    board.resize(8, QVector<ChessPiece*>(8, nullptr));
    clearCheckHighlight();

    QPixmap boardPixmap(":/images/board.png");
    if (!boardPixmap.isNull()) {
        QGraphicsPixmapItem* background = new QGraphicsPixmapItem(boardPixmap);
        background->setZValue(-10);
        addItem(background);
    } else {
        qWarning() << "Failed to load board background image!";
    }
}

Board::~Board() {
    // Clean up all pieces on destruction
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete board[row][col];
        }
    }
}

void Board::setupInitialPosition() {
    // --- White pieces (bottom side) ---
    // Row 7: Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook
    addPiece(new Rook(ChessPiece::White, 7, 0));
    addPiece(new Knight(ChessPiece::White, 7, 1));
    addPiece(new Bishop(ChessPiece::White, 7, 2));
    addPiece(new Queen(ChessPiece::White, 7, 3));
    addPiece(new King(ChessPiece::White, 7, 4));
    addPiece(new Bishop(ChessPiece::White, 7, 5));
    addPiece(new Knight(ChessPiece::White, 7, 6));
    addPiece(new Rook(ChessPiece::White, 7, 7));

    // Row 6: White pawns
    for (int col = 0; col < 8; ++col) {
        addPiece(new Pawn(ChessPiece::White, 6, col));
    }

    // --- Black pieces (top side) ---
    // Row 0: Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook
    addPiece(new Rook(ChessPiece::Black, 0, 0));
    addPiece(new Knight(ChessPiece::Black, 0, 1));
    addPiece(new Bishop(ChessPiece::Black, 0, 2));
    addPiece(new Queen(ChessPiece::Black, 0, 3));
    addPiece(new King(ChessPiece::Black, 0, 4));
    addPiece(new Bishop(ChessPiece::Black, 0, 5));
    addPiece(new Knight(ChessPiece::Black, 0, 6));
    addPiece(new Rook(ChessPiece::Black, 0, 7));

    // Row 1: Black pawns
    for (int col = 0; col < 8; ++col) {
        addPiece(new Pawn(ChessPiece::Black, 1, col));
    }
}


void Board::addPiece(ChessPiece* piece) {
    int row = piece->getRow();
    int col = piece->getCol();

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        qWarning() << "Piece position out of bounds:" << row << col;
        delete piece;
        return;
    }

    if (board[row][col]) {
        qWarning() << "Overwriting existing piece at:" << row << col;
        removeItem(board[row][col]);
        delete board[row][col];
    }

    board[row][col] = piece;
    piece->updateGraphicsPosition(border, squareSize);
    addItem(piece);
}

ChessPiece* Board::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return nullptr;
    return board[row][col];
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF pos = event->scenePos();
    int clickedRow = (pos.y() - border) / squareSize;
    int clickedCol = (pos.x() - border) / squareSize;

    clickedRow = qBound(0, clickedRow, 7);
    clickedCol = qBound(0, clickedCol, 7);

    QGraphicsItem* item = itemAt(pos, QTransform());
    auto* piece = dynamic_cast<ChessPiece*>(item);

    if (piece && piece->getColor() != currentPlayer) {
        clearHighlights();
        validMoves.clear();
        selectedPiece = nullptr;
        return;
    }

    if (selectedPiece) {
        bool clickedValidMove = std::any_of(validMoves.begin(), validMoves.end(),
                                            [&](const QPair<int, int>& mv) {
                                                return mv.first == clickedRow && mv.second == clickedCol;
                                            });

        if (clickedValidMove) {
            movePiece(selectedPiece, clickedRow, clickedCol);

            // Highlight check if opponent is now in check
            ChessPiece::PieceColor nextPlayer = currentPlayer;
            if (isInCheck(nextPlayer)) {
                qDebug() << "Player" << (nextPlayer == ChessPiece::White ? "White" : "Black") << "is in check!";
                for (int row = 0; row < 8; ++row) {
                    for (int col = 0; col < 8; ++col) {
                        ChessPiece* king = board[row][col];
                        if (king && king->getType() == ChessPiece::King && king->getColor() == nextPlayer) {
                            highlightCheck(row, col);
                            break;
                        }
                    }
                }
            } else {
                clearCheckHighlight();
            }

            clearHighlights();
            validMoves.clear();
            selectedPiece = nullptr;
            return;
        }
    }

    if (piece) {
        selectedPiece = piece;
        originalPos = piece->pos();

        // ✅ Use filtered legal moves instead of all valid moves
        validMoves = getLegalMoves(piece);

        clearHighlights();
        highlightMoves(validMoves);
        selectedPiece->setZValue(100);
    } else {
        clearHighlights();
        validMoves.clear();
        selectedPiece = nullptr;

        // If no one is in check, clear red highlight
        if (!isInCheck(currentPlayer) && !isInCheck(currentPlayer == ChessPiece::White ? ChessPiece::Black : ChessPiece::White)) {
            clearCheckHighlight();
        }
    }

    isDragging = false;
}



void Board::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!selectedPiece) return;

    QPointF delta = event->scenePos() - originalPos;
    if (!isDragging && delta.manhattanLength() > 5)
        isDragging = true;

    if (isDragging)
        selectedPiece->setPos(event->scenePos() - QPointF(squareSize / 2, squareSize / 2));
}

void Board::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (!selectedPiece) return;

    selectedPiece->setZValue(0);

    if (!isDragging) {
        selectedPiece->setPos(originalPos);
        return;
    }

    int newRow = (event->scenePos().y() - border) / squareSize;
    int newCol = (event->scenePos().x() - border) / squareSize;

    newRow = qBound(0, newRow, 7);
    newCol = qBound(0, newCol, 7);

    bool isValid = std::any_of(validMoves.begin(), validMoves.end(), [&](const QPair<int, int>& mv) {
        return mv.first == newRow && mv.second == newCol;
    });

    if (isValid) {
        movePiece(selectedPiece, newRow, newCol);

        if (isInCheck(currentPlayer)) {
            qDebug() << "Player" << (currentPlayer == ChessPiece::White ? "White" : "Black") << "is in check!";
        }

        ChessPiece::PieceColor nextPlayer = currentPlayer;
        if (isInCheck(nextPlayer)) {
            qDebug() << "Player" << (nextPlayer == ChessPiece::White ? "White" : "Black") << "is in check!";

            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    ChessPiece* piece = board[row][col];
                    if (piece && piece->getType() == ChessPiece::King && piece->getColor() == nextPlayer) {
                        highlightCheck(row, col);
                        break;
                    }
                }
            }
        } else {
            clearCheckHighlight();
        }
    } else {
        selectedPiece->setPos(originalPos);
    }

    selectedPiece = nullptr;
    validMoves.clear();
    clearHighlights();
    isDragging = false;
}


void Board::movePiece(ChessPiece* piece, int newRow, int newCol) {
    int oldRow = piece->getRow();
    int oldCol = piece->getCol();

    if (board[newRow][newCol]) {
        removeItem(board[newRow][newCol]);
        delete board[newRow][newCol];
    }

    board[oldRow][oldCol] = nullptr;
    board[newRow][newCol] = piece;

    piece->setBoardPosition(newRow, newCol);
    piece->updateGraphicsPosition(border, squareSize);

    switchTurn();
    if (isCheckmate(currentPlayer)) {
        emit checkmate(currentPlayer);
    }
}

void Board::highlightMoves(const QVector<QPair<int, int>>& moves) {
    for (const auto& move : moves) {
        int row = move.first;
        int col = move.second;

        auto* rect = new QGraphicsRectItem(
            border + col * squareSize,
            border + row * squareSize,
            squareSize,
            squareSize
            );
        rect->setBrush(QColor(0, 255, 0, 80));
        rect->setZValue(-1);
        addItem(rect);
    }
}

void Board::clearHighlights() {
    QList<QGraphicsItem*> allItems = items();
    for (auto* item : allItems) {
        auto* rect = dynamic_cast<QGraphicsRectItem*>(item);
        if (rect && rect->zValue() == -1) {
            removeItem(rect);
            delete rect;
        }
    }
}

ChessPiece::PieceColor Board::getCurrentPlayer() const {
    return currentPlayer;
}

void Board::switchTurn() {
    currentPlayer = (currentPlayer == ChessPiece::White) ? ChessPiece::Black : ChessPiece::White;
    emit turnChanged(currentPlayer);
}

void Board::clear() {
    // Remove all pieces and clear the board array
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c]) {
                removeItem(board[r][c]);
                delete board[r][c];
                board[r][c] = nullptr;
            }
        }
    }

    clearHighlights();
    selectedPiece = nullptr;
    validMoves.clear();
    currentPlayer = ChessPiece::White;
    emit turnChanged(currentPlayer);
}

bool Board::isInCheck(ChessPiece::PieceColor color) const{
    int kingRow = -1, kingCol = -1;
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            ChessPiece* piece = board[row][col];
            if(piece && piece->getType() == ChessPiece::King && piece->getColor() == color){
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    if (kingRow == -1 || kingCol == -1) {
        qWarning() << "King not found!";
        return false;  // Should never happen unless king is missing
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = board[row][col];
            if (piece && piece->getColor() != color) {
                QVector<QPair<int, int>> moves = piece->getValidMoves(board);
                for (const auto& move : moves) {
                    if (move.first == kingRow && move.second == kingCol) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Board::highlightCheck(int row, int col){
    clearCheckHighlight();

    checkHighlight = new QGraphicsRectItem(
        border + col*squareSize,
        border + row * squareSize,
        squareSize,
        squareSize
        );
    checkHighlight->setBrush(QColor(255, 0, 0, 100)); // red overlay
    checkHighlight->setZValue(-0.5);
    addItem(checkHighlight);
}

void Board::clearCheckHighlight() {
    if (checkHighlight) {
        removeItem(checkHighlight);
        delete checkHighlight;
        checkHighlight = nullptr;
    }
}

bool Board::isLegalMove(ChessPiece* piece, int toRow, int toCol) {
    int fromRow = piece->getRow();
    int fromCol = piece->getCol();

    ChessPiece* captured = board[toRow][toCol];
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;
    piece->setBoardPosition(toRow, toCol);

    bool safe = !isInCheck(piece->getColor());

    // Undo the move
    board[fromRow][fromCol] = piece;
    board[toRow][toCol] = captured;
    piece->setBoardPosition(fromRow, fromCol);

    return safe;
}

QVector<QPair<int, int>> Board::getLegalMoves(ChessPiece* piece) {
    QVector<QPair<int, int>> result;
    QVector<QPair<int, int>> moves = piece->getValidMoves(board);

    for (const auto& mv : moves) {
        if (isLegalMove(piece, mv.first, mv.second)) {
            result.append(mv);
        }
    }
    return result;
}

bool Board::isCheckmate(ChessPiece::PieceColor color) {
    if (!isInCheck(color)) return false;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = board[row][col];
            if (piece && piece->getColor() == color) {
                QVector<QPair<int, int>> legal = getLegalMoves(piece);
                if (!legal.isEmpty()) {
                    return false;
                }
            }
        }
    }

    return true;
}


