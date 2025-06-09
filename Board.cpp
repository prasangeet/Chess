// File includes : Custom game logic
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

    //load and display board background
    QPixmap boardPixmap(":/images/board.png");
    if (!boardPixmap.isNull()) {
        // Calculate the expected board size (8x8 squares + optional border)
        int boardSize = squareSize * 8 + 2 * border;

        // Scale the image to fit the board size
        QPixmap scaledPixmap = boardPixmap.scaled(boardSize, boardSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QGraphicsPixmapItem* background = new QGraphicsPixmapItem(scaledPixmap);
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

//Add Pieces
void Board::addPiece(ChessPiece* piece) {
    int row = piece->getRow();
    int col = piece->getCol();

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        qWarning() << "Piece position out of bounds:" << row << col;
        delete piece;
        return;
    }
    //Prevent visual overlap
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

void Board::resetSelection() {
    if (selectedPiece)
        selectedPiece->setZValue(0);
    clearHighlights();
    validMoves.clear();
    selectedPiece = nullptr;
    isDragging = false;
}


void Board::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() != Qt::LeftButton)
        return;  // Only respond to left click

    QPointF pos = event->scenePos();
    int clickedRow = qBound(0, static_cast<int>((pos.y() - border) / squareSize), 7);
    int clickedCol = qBound(0, static_cast<int>((pos.x() - border) / squareSize), 7);

    QGraphicsItem* item = itemAt(pos, QTransform());
    ChessPiece* piece = dynamic_cast<ChessPiece*>(item);

    if (piece && piece->getColor() != currentPlayer) {
        resetSelection();
        return;
    }

    if (selectedPiece) {
        for (const auto& move : validMoves) {
            if (move.first == clickedRow && move.second == clickedCol) {
                movePiece(selectedPiece, clickedRow, clickedCol);
                resetSelection();
                return;
            }
        }
    }

    if (piece) {
        selectedPiece = piece;
        originalPos = piece->pos();
        validMoves = getLegalMoves(piece);
        clearHighlights();
        highlightMoves(validMoves);
        selectedPiece->setZValue(100);
    } else {
        resetSelection();
        if (!isInCheck(ChessPiece::White) && !isInCheck(ChessPiece::Black)) {
            clearCheckHighlight();
        }
    }

    isDragging = false;
}

void Board::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (!selectedPiece || !(event->buttons() & Qt::LeftButton))
        return;

    QPointF delta = event->scenePos() - selectedPiece->pos();

    if (!isDragging && delta.manhattanLength() > squareSize / 2) {
        isDragging = true;
    }

    if (isDragging) {
        selectedPiece->setPos(event->scenePos() - QPointF(squareSize / 2, squareSize / 2));
    }
}

void Board::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (!selectedPiece || event->button() != Qt::LeftButton)
        return;

    selectedPiece->setZValue(0);

    int newRow = qBound(0, static_cast<int>((event->scenePos().y() - border) / squareSize), 7);
    int newCol = qBound(0, static_cast<int>((event->scenePos().x() - border) / squareSize), 7);

    if (isDragging) {
        for (const auto& mv : validMoves) {
            if (mv.first == newRow && mv.second == newCol) {
                movePiece(selectedPiece, newRow, newCol);
                resetSelection();
                return;
            }
        }
        selectedPiece->setPos(originalPos);
    } else {
        selectedPiece->setPos(originalPos);
    }
}

void Board::movePiece(ChessPiece* piece, int newRow, int newCol) {
    int oldRow = piece->getRow();
    int oldCol = piece->getCol();

    if (abs(newCol - oldCol) == 2) {
        if (tryCastling(piece, newRow, newCol)) {
            switchTurn();
            updateCheckHighlight();
            if (isCheckmate(currentPlayer)) {
                emit checkmate(currentPlayer);
            }
            return; // castling move done, no further move needed
        }
    }

    if (piece->getType() == ChessPiece::Pawn) {
        if (QPair<int, int>(newRow, newCol) == enPassantTarget) {
            // The pawn being captured is behind the en passant target square
            int capturedPawnRow = oldRow;
            int capturedPawnCol = newCol;

            if (board[capturedPawnRow][capturedPawnCol]) {
                removeItem(board[capturedPawnRow][capturedPawnCol]);
                delete board[capturedPawnRow][capturedPawnCol];
                board[capturedPawnRow][capturedPawnCol] = nullptr;
            }
        }
    }

    // Capture if needed
    if (board[newRow][newCol]) {
        removeItem(board[newRow][newCol]);
        delete board[newRow][newCol];
    }

    // Standard piece move
    board[oldRow][oldCol] = nullptr;
    board[newRow][newCol] = piece;
    piece->setBoardPosition(newRow, newCol);
    piece->updateGraphicsPosition(border, squareSize);

    if (!piece->hasMoved())
        piece->markMoved();  // ✅ important: mark the king (or any piece) as moved

    if (piece->getType() == ChessPiece::Pawn && abs(newRow - oldRow) == 2) {
        if (piece->getColor() == ChessPiece::White) {
            enPassantTarget = QPair<int, int>(oldRow - 1, newCol);
        } else { // Black
            enPassantTarget = QPair<int, int>(oldRow + 1, newCol);
        }
    } else {
        enPassantTarget = QPair<int, int>(-1, -1);  // Clear en passant
    }

    switchTurn();
    updateCheckHighlight();

    if (isCheckmate(currentPlayer)) {
        emit checkmate(currentPlayer);
    }
}


void Board::highlightMoves(const QVector<QPair<int, int>>& moves) {
    clearHighlights();  // Clear previous highlights

    for (const auto& move : moves) {
        int row = move.first;
        int col = move.second;

        int x = border + col * squareSize;
        int y = border + row * squareSize;

        QGraphicsItem* target = pieceAt(row, col);

        if (target) {
            // Highlight the full square in red (capture move)
            auto* rect = new QGraphicsRectItem(x, y, squareSize, squareSize);
            rect->setBrush(QColor(204, 119, 34, 100)); // Semi-transparent red
            rect->setPen(Qt::NoPen);
            rect->setZValue(-1); // Behind pieces
            rect->setData(0, "highlight"); // For cleanup
            addItem(rect);
        } else {
            // Normal move – gray dot
            int dotSize = squareSize / 3;
            int offset = (squareSize - dotSize) / 2;

            auto* dot = new QGraphicsEllipseItem(x + offset, y + offset, dotSize, dotSize);
            dot->setBrush(QColor(0, 0, 0, 50)); // Semi-transparent gray
            dot->setPen(Qt::NoPen);
            dot->setZValue(-1); // Behind pieces
            dot->setData(0, "highlight");
            addItem(dot);
        }
    }
}
QGraphicsItem* Board::pieceAt(int row, int col) {
    int x = border + col * squareSize;
    int y = border + row * squareSize;

    for (QGraphicsItem* item : items(QRectF(x, y, squareSize, squareSize))) {
        if (item->zValue() >= 0) { // Only consider pieces (not highlights)
            return item;
        }
    }

    return nullptr;
}
void Board::clearHighlights() {

    QList<QGraphicsItem*> toRemove;

    for (QGraphicsItem* item : items()) {
        if (item->zValue() == -1) {
            auto* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item);
            auto* rect = dynamic_cast<QGraphicsRectItem*>(item);
            if (ellipse || rect) {
                toRemove.append(item);
            }
        }
    }
    for (QGraphicsItem* item : toRemove) {
        removeItem(item);
        delete item;
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
    resetSelection();
    currentPlayer = ChessPiece::White;
    emit turnChanged(currentPlayer);
}

bool Board::isInCheck(ChessPiece::PieceColor color) const {
    int kingRow = -1, kingCol = -1;
    bool found = false;

    for (int row = 0; row < 8 && !found; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = board[row][col];
            if (piece && piece->getType() == ChessPiece::King && piece->getColor() == color) {
                kingRow = row;
                kingCol = col;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        qWarning() << "King not found!";
        return false;
    }

    // Check if king is attacked by opponent
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
void Board::updateCheckHighlight() {
    clearCheckHighlight();

    // If current player is in check, highlight their king
    if (isInCheck(currentPlayer)) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                ChessPiece* piece = board[row][col];
                if (piece && piece->getType() == ChessPiece::King && piece->getColor() == currentPlayer) {
                    highlightCheck(row, col);
                    return;
                }
            }
        }
    }
}



void Board::highlightCheck(int row, int col) {
    clearCheckHighlight();

    // Highlight only the king's square
    checkHighlight = new QGraphicsRectItem(
        border + col * squareSize,
        border + row * squareSize,
        squareSize,
        squareSize
        );
    checkHighlight->setBrush(QColor(255, 0, 0, 100));  // Semi-transparent red
    checkHighlight->setPen(Qt::NoPen);
    checkHighlight->setZValue(-0.4); // Behind pieces, but above normal highlights
    addItem(checkHighlight);

    // Create the "Check!" label
    checkLabel = new QGraphicsTextItem("Check!");
    checkLabel->setDefaultTextColor(Qt::red);
    QFont font("Arial", 18, QFont::Bold);
    checkLabel->setFont(font);
    checkLabel->setZValue(1); // On top
    checkLabel->setPos(border + 8 * squareSize + 20, border); // Right of board
    addItem(checkLabel);
}


void Board::clearCheckHighlight() {
    if (checkHighlight) {
        removeItem(checkHighlight);
        delete checkHighlight;
        checkHighlight = nullptr;
    }
    if (checkLabel) {
        removeItem(checkLabel);
        delete checkLabel;
        checkLabel = nullptr;
    }
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

    if (piece->getType() == ChessPiece::King) {
        QVector<QPair<int,int>> castleMoves = getCastlingMoves(piece);
        moves.append(castleMoves);
    }
    if (piece->getType() == ChessPiece::Pawn) {
        auto enPassant = getEnPassantMove(piece);
        if (enPassant.has_value() && isLegalMove(piece, enPassant->first, enPassant->second)) {
            moves.append(*enPassant);
        }
    }

    for (const auto& mv : moves) {
        if (isLegalMove(piece, mv.first, mv.second)) {
            result.append(mv);
        }
    }
    return result;
}

QVector<QPair<int, int>> Board::getCastlingMoves(ChessPiece* king){
    QVector<QPair<int, int>> castlingMoves;

    if(king->hasMoved() || isInCheck(king->getColor())){
        return castlingMoves;
    }

    int row = king->getRow();
    int col = king->getCol();

    if (canCastle(king->getColor(), true)) {
        // Assume rook is at (row, 7)
        // Add move for king to move two squares right
        castlingMoves.append(qMakePair(row, col + 2));
    }

    // Check queenside castling (left side)
    if (canCastle(king->getColor(), false)) {
        // Assume rook is at (row, 0)
        // Add move for king to move two squares left
        castlingMoves.append(qMakePair(row, col - 2));
    }

    return castlingMoves;
}

bool Board::canCastle(ChessPiece::PieceColor color, bool kingside){
    int row = (color == ChessPiece::White)? 7 : 0;
    int kingCol = 4;

    int rookCol = (kingside)? 7 : 0;

    ChessPiece* rook = board[row][rookCol];
    ChessPiece* king = board[row][kingCol];

    if(!king || !rook){
        return false;
    }

    if (king->getColor() != color || rook->getColor() != color)
        return false;

    // Neither king nor rook must have moved
    if (king->hasMoved() || rook->hasMoved())
        return false;

    int step = kingside? 1 : -1;
    int start = kingCol + step;
    int end = kingside? rookCol -1 : rookCol + 1;

    for (int c = start; c != end + step; c += step) {
        if (board[row][c] != nullptr) {
            return false;  // something blocking the path
        }
    }

    if(isInCheck(color)){
        return false;
    }

    for (int c = kingCol; c != kingCol + 2 * step + step; c += step) {
        ChessPiece::PieceColor oppositeColor = (color == ChessPiece::White) ? ChessPiece::Black : ChessPiece::White;
        if (isSquareAttacked(row, c, oppositeColor)) {
            return false;
        }
        if (c == kingCol + 2 * step)
            break;
    }

    return true;
}

bool Board::isSquareAttacked(int row, int col, ChessPiece::PieceColor byColor) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            ChessPiece* attacker = board[r][c];
            if (!attacker || attacker->getColor() != byColor)
                continue;

            QVector<QPair<int, int>> pseudoMoves = attacker->getValidMoves(board);

            for (const auto& move : pseudoMoves) {
                if (move.first == row && move.second == col) {
                    return true;  // square is attacked
                }
            }
        }
    }
    return false;
}

bool Board::tryCastling(ChessPiece* king, int newRow, int newCol) {
    if (king->getType() != ChessPiece::King)
        return false;

    int oldCol = king->getCol();
    int row = king->getRow();

    bool kingside = newCol > oldCol;

    int rookCol = kingside ? 7 : 0;
    int newRookCol = kingside ? newCol - 1 : newCol + 1;

    ChessPiece* rook = board[row][rookCol];

    if (!rook)
        return false;

    // Move the king
    board[row][oldCol] = nullptr;
    board[row][newCol] = king;
    king->setBoardPosition(row, newCol);
    king->updateGraphicsPosition(border, squareSize);
    king->markMoved();

    // Move the rook
    board[row][rookCol] = nullptr;
    board[row][newRookCol] = rook;
    rook->setBoardPosition(row, newRookCol);
    rook->updateGraphicsPosition(border, squareSize);
    rook->markMoved();

    return true;
}


std::optional<QPair<int, int>> Board::getEnPassantMove(ChessPiece* piece) {
    if (!piece || piece->getType() != ChessPiece::Pawn)
        return std::nullopt;

    int row = piece->getRow();
    int col = piece->getCol();
    int dir = (piece->getColor() == ChessPiece::White) ? -1 : 1;

    // Check left
    if (col > 0 && enPassantTarget == QPair<int, int>(row + dir, col - 1)) {
        return QPair<int, int>(row + dir, col - 1);
    }

    // Check right
    if (col < 7 && enPassantTarget == QPair<int, int>(row + dir, col + 1)) {
        return QPair<int, int>(row + dir, col + 1);
    }

    return std::nullopt;
}


bool Board::isStalemate(ChessPiece::PieceColor playerColor) {
    QVector<ChessPiece*> pieces = getPieces(playerColor);

    // 2. For each piece, check if it has any legal moves
    for (ChessPiece* piece : pieces) {
        QVector<QPair<int, int>> legalMoves = getLegalMoves(piece);
        if (!legalMoves.isEmpty()) {
            return false;
        }
    }

    if (!isInCheck(playerColor)) {
        return true;
    }

    return false;
}





