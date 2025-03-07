# Anti-Matter Maze

## 📌 Project Description
**Anti-Matter Maze** is a terminal-based strategy game written in C, where players navigate a maze, collect subatomic particles, and attempt to produce anti-hydrogen while avoiding black holes. The game features user authentication, high score tracking, and an AI-driven auto-play mode.

## 🚀 Features
- **User Authentication**: Players must register or log in before playing.
- **Dynamic Maze System**: Players can choose existing maps or add new ones.
- **Particle Collection Mechanics**: Collecting opposite particles cancels them out.
- **Black Hole Mechanic**: Stepping into a black hole results in instant loss.
- **Time-Based Scoring**: Faster completion leads to higher scores.
- **AI Auto-Play Mode**: The game can be solved automatically.
- **Persistent Data Storage**: High scores and user data are stored in files.
- **Interactive CLI**: The terminal refreshes dynamically for a smooth user experience.

## 🛠️ Technologies Used
- **C Programming**: Core logic implementation.
- **File I/O (Binary & Text)**: Storing high scores, user credentials, and tutorials.
- **Dynamic Memory Allocation**: Efficient management of game state and user data.
- **AI Pathfinding (Auto-Play Mode)**: Automated maze solving.
- **Terminal UI**: Clear and interactive CLI experience.

## 📜 How to Compile and Run
### 🏗️ Compilation Steps
Ensure you have `gcc` installed, then compile the project with:
```sh
gcc game.c -o antimatter_maze -Wall
```

### ▶️ Run the Game
Execute the compiled program:
```sh
./antimatter_maze
```

## 📂 File Structure
```
.
├── maps/                 # Folder containing maze files
├── scores.txt            # Stores the top 5 high scores
├── tutorial.txt          # Contains tutorial instructions
├── accounts.txt          # Stores user login information (binary format)
├── game.c                # Main game source code
├── README.md             # Project documentation
```

## 🎮 Gameplay Instructions
- Move the player using arrow keys.
- Collect subatomic particles **(P+, e-, P-, e+)**.
- Avoid **black holes (K)** or you will lose.
- Reach the exit **(C)** within the time limit to win.
- The **auto-play mode** allows AI to solve the maze.

## 🏆 Scoring System
- **Valid Anti-Hydrogen (P- and e+ left at exit)** → Maximum points.
- **Opposite particles cancel each other** → No points.
- **Faster completion** → Higher bonus points.
- **Stepping into a black hole** → Instant loss.

## 🔧 Configuration and Customization
- Add new maps to the `maps/` directory.
- Edit `tutorial.txt` to update the in-game tutorial.
- Modify `game.c` to tweak game mechanics or add features.

## 🤖 Auto-Play Mode
The game includes an **AI solver** that autonomously finds the best path to reach the exit while maximizing points.

## 📜 License
This project is released under the **MIT License**.

## 👨‍💻 Contributors
- **Yusuf YILMAZ**  
For contributions, feel free to fork the repository and submit a pull request!

Happy coding! 🎮
