# ♟️ C++ Qt Chess Game

A fully-featured chess game built using **C++** and the **Qt Framework**. This game includes a complete implementation of chess rules with a clean and interactive GUI — supporting **en passant**, **castling**, **check**, **checkmate**, **stalemate**, and **pawn promotion**.

---

## 🎮 Features

- ✅ All standard chess rules:
  - ♟ En Passant
  - ♜ Castling (King-side & Queen-side)
  - ♛ Pawn Promotion (Queen, Rook, Bishop, Knight)
  - ♚ Check & Checkmate detection
  - 🧊 Stalemate recognition
- 🖱️ Intuitive mouse-based piece movement
- ✨ Highlighting valid moves on click
- 🖼️ High-quality transparent PNG chess pieces
- 🎉 Winner announcement with visual effects (icons, emojis, labels)
- 📐 Scalable board layout using `QGraphicsScene`
- 🧹 Automatic clearing of move highlights and selection

---

## 🧱 Tech Stack

| Component         | Description                            |
|------------------|----------------------------------------|
| Language          | C++17                                  |
| GUI Framework     | Qt 6 (or Qt 5 compatible)              |
| Build System      | CMake                                  |
| UI Components     | QGraphicsView, QLabel, QPushButton     |
| Assets            | PNG icons for chess pieces via `.qrc` |

---

## 📁 Project Structure
```
chess/
├── src/ # Source files
│ ├── main.cpp # Application entry point
│ ├── mainwindow.cpp/.h # Main GUI window and layout handling
│ ├── board.cpp/.h # Renders the board and manages board state
│ ├── game.cpp/.h # Chess rules, player turns, move validation
│ ├── piece.cpp/.h # Abstract base class for all chess pieces
│ └── pieces/ # Individual piece logic
│ ├── pawn.cpp/.h
│ ├── rook.cpp/.h
│ ├── knight.cpp/.h
│ ├── bishop.cpp/.h
│ ├── queen.cpp/.h
│ └── king.cpp/.h
├── resources/ # UI resources
│ ├── images/ # PNGs for white and black pieces
│ └── resources.qrc # Qt Resource Collection file
├── CMakeLists.txt # Build system configuration (CMake)
└── README.md # Project documentation

```

---

## 🛠️ How to Build and Run

### 📋 Requirements

- Qt 6.x (or 5.x) installed  
- CMake ≥ 3.15  
- C++17-compatible compiler (GCC, Clang, MSVC)

---

### 🧪 Build Steps
```
 📥 Clone the repository
git clone https://github.com/your-username/qt-chess-game.git
cd qt-chess-game
🗂️ Create a build directory
mkdir build && cd build
⚙️ Generate build files with CMake
cmake ..
🛠️ Build the project
cmake --build .
▶️ Run the application
./ChessGame
🪟 On Windows, run ChessGame.exe instead of ./ChessGame

```

---
### 🧠 Gameplay Rules
1.Players alternate turns between White and Black
2.Legal moves are highlighted upon selecting a piece
3.Special rules like en passant and castling are validated before execution
4.Game ends when a checkmate or stalemate is detected
5.Upon reaching the final rank, a pawn is promoted to a chosen piece

---
### 📸 Screenshots
(Insert your screenshots or gameplay GIFs here)

🧩 Initial Board Setup
✅ Valid move highlighting
🔁 En Passant and Castling examples
🏁 Checkmate and Winner screen
🎭 Pawn Promotion dialog

---

### 📦 Future Improvements
🤖 AI Player using Minimax Algorithm
🌐 Multiplayer over LAN/Internet
📜 PGN or move history viewer
⏱️ Timer per player and move logs

---

🙌 Acknowledgements
Qt Community for GUI resources
Icons sourced from [chess.com-like pieces with transparent background]
