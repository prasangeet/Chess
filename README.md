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

chess/
├── src/
│ ├── main.cpp # Application entry point
│ ├── mainwindow.cpp/.h # Main window and layout
│ ├── board.cpp/.h # Handles board rendering and logic
│ ├── game.cpp/.h # Chess rules, player turns, move legality
│ ├── piece.cpp/.h # Abstract base class for all pieces
│ └── pieces/
│ ├── pawn.cpp/.h
│ ├── rook.cpp/.h
│ ├── knight.cpp/.h
│ ├── bishop.cpp/.h
│ ├── queen.cpp/.h
│ └── king.cpp/.h
├── resources/
│ ├── images/ # PNGs of white and black pieces
│ └── resources.qrc # Qt Resource Collection file
├── CMakeLists.txt # Build configuration
└── README.md # Project documentation

yaml
Copy
Edit

---

## 🛠️ How to Build and Run

### 📋 Requirements

- Qt 6.x (or 5.x) installed
- CMake ≥ 3.15
- C++17-compatible compiler (GCC, Clang, MSVC)

### 🧪 Build Steps

```bash
# Clone the repository
git clone https://github.com/your-username/qt-chess-game.git
cd qt-chess-game

# Create a build directory
mkdir build && cd build

# Generate build files with CMake
cmake ..

# Build the project
cmake --build .

# Run the application
./ChessGame
On Windows: use ChessGame.exe instead of ./ChessGame

🧠 Gameplay Rules
Players alternate turns between White and Black

Legal moves are highlighted upon selecting a piece

Special rules like en passant and castling are validated before execution

Game ends when a checkmate or stalemate is detected

Upon reaching the final rank, a pawn is promoted to a chosen piece

📸 Screenshots
(Insert your screenshots or gameplay GIFs here)

🧩 Initial Board Setup

✅ Valid move highlighting

🔁 En Passant and Castling examples

🏁 Checkmate and Winner screen

🎭 Pawn Promotion dialog

📦 Future Improvements
🤖 AI Player using Minimax Algorithm

🌐 Multiplayer over LAN/Internet

📜 PGN or move history viewer

⏱️ Timer per player and move logs

📜 License
This project is released under the MIT License — feel free to use, modify, and distribute it.

🙌 Acknowledgements
Qt Community for GUI resources

Open-source chess engines for logic inspiration

Icons sourced from [chess.com-like pieces with transparent background]
