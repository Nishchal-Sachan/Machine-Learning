#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>  // For Windows sound and console functions
#include <mmsystem.h> // For PlaySound function
#include <iomanip>
#include <random>
#include <limits>
#include <thread>
#include <chrono>


#pragma comment(lib, "winmm.lib")  // Link Windows Multimedia library

using namespace std;
using namespace std::chrono;

class BombDiffuser {
private:
    int level;
    int timeLimit;
    steady_clock::time_point startTime;
    bool gameActive;
    bool bombDiffused;
    
public:
    BombDiffuser() : level(1), timeLimit(0), gameActive(false), bombDiffused(false) {
        srand(static_cast<unsigned>(time(0)));
    }
    
    void clearScreen() {
        system("cls");
    }
    
    void displayTitle() {
        cout << "\n";
        cout << "████████╗██╗███╗   ███╗███████╗██████╗  ██████╗ ███╗   ███╗██████╗ \n";
        cout << "╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗██╔═══██╗████╗ ████║██╔══██╗\n";
        cout << "   ██║   ██║██╔████╔██║█████╗  ██████╔╝██║   ██║██╔████╔██║██████╔╝\n";
        cout << "   ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗██║   ██║██║╚██╔╝██║██╔══██╗\n";
        cout << "   ██║   ██║██║ ╚═╝ ██║███████╗██████╔╝╚██████╔╝██║ ╚═╝ ██║██████╔╝\n";
        cout << "   ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═════╝  ╚═════╝ ╚═╝     ╚═╝╚═════╝ \n";
        cout << "\n           *** BOMB DIFFUSER SIMULATION ***\n";
        cout << "              Defuse or Face the Blast!\n\n";
    }
    
    void playSound(const string& soundType) {
        if (soundType == "beep") {
            Beep(800, 200);  // High pitch beep for timer warning
        } else if (soundType == "blast") {
            // Simulate blast sound with multiple beeps
            for (int i = 0; i < 10; i++) {
                Beep(200 + i * 50, 100);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        } else if (soundType == "success") {
            // Victory sound
            Beep(523, 200); // C note
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Beep(659, 200); // E note
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Beep(784, 400); // G note
        }
    }
    
    void displayTimer(int remainingTime) {
        cout << "\n┌─────────────────────┐\n";
        cout << "│  ⏰ TIME LEFT: " << setw(2) << setfill('0') << remainingTime << "s  │\n";
        cout << "└─────────────────────┘\n";
        
        // Sound warning when less than 10 seconds
        if (remainingTime <= 10 && remainingTime > 0) {
            playSound("beep");
        }
    }
    
    bool checkTimeUp() {
        auto currentTime = steady_clock::now();
        auto elapsed = duration_cast<seconds>(currentTime - startTime);
        int remainingTime = timeLimit - static_cast<int>(elapsed.count());
        
        if (remainingTime <= 0) {
            return true;
        }
        
        displayTimer(remainingTime);
        return false;
    }
    
    void selectLevel() {
        int choice;
        
        while (true) {
            clearScreen();
            displayTitle();
            
            cout << "🎯 SELECT DIFFICULTY LEVEL:\n\n";
            cout << "1. 🟢 BEGINNER   (60 seconds - Easy puzzles)\n";
            cout << "2. 🟡 INTERMEDIATE (45 seconds - Medium puzzles)\n";
            cout << "3. 🔴 ADVANCED   (30 seconds - Hard puzzles)\n";
            cout << "\nEnter your choice (1-3): ";
            
            cin >> choice;
            
            if (choice >= 1 && choice <= 3) {
                level = choice;
                switch (level) {
                    case 1: timeLimit = 60; break;
                    case 2: timeLimit = 45; break;
                    case 3: timeLimit = 30; break;
                }
                break;
            } else {
                cout << "\n❌ Invalid choice! Please enter 1, 2, or 3.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
    
    bool solveMathPuzzle() {
        vector<string> operations = {"+", "-", "*"};
        random_device rd;
        mt19937 gen(rd());
        
        int a = uniform_int_distribution<>(1, level * 10)(gen);
        int b = uniform_int_distribution<>(1, level * 5)(gen);
        string op = operations[uniform_int_distribution<>(0, 2)(gen)];
        
        int correctAnswer;
        if (op == "+") correctAnswer = a + b;
        else if (op == "-") correctAnswer = a - b;
        else correctAnswer = a * b;
        
        cout << "\n🧮 MATH PUZZLE:\n";
        cout << "Solve: " << a << " " << op << " " << b << " = ?\n";
        cout << "Answer: ";
        
        int userAnswer;
        cin >> userAnswer;
        
        return userAnswer == correctAnswer;
    }
    
    bool solveLogicPuzzle() {
        vector<vector<string>> puzzles = {
            // Beginner puzzles
            {"What comes next in the sequence: 2, 4, 6, 8, ?", "10"},
            {"If A=1, B=2, C=3, what is D?", "4"},
            {"Complete: Red, Blue, Green, ?", "Yellow"},
            
            // Intermediate puzzles
            {"What comes next: 1, 1, 2, 3, 5, 8, ?", "13"},
            {"If 3x + 5 = 14, what is x?", "3"},
            {"Mirror of 'BOMB' is?", "BMOB"},
            
            // Advanced puzzles
            {"What comes next: 2, 6, 12, 20, ?", "30"},
            {"If today is Tuesday, what day is it in 100 days?", "Thursday"},
            {"Binary of 15 is?", "1111"}
        };
        
        int startIndex = (level - 1) * 3;
        int puzzleIndex = startIndex + (rand() % 3);
        
        cout << "\n🧩 LOGIC PUZZLE:\n";
        cout << puzzles[puzzleIndex][0] << "\n";
        cout << "Answer: ";
        
        string userAnswer;
        cin >> userAnswer;
        
        // Convert to lowercase for comparison
        transform(userAnswer.begin(), userAnswer.end(), userAnswer.begin(), ::tolower);
        string correctAnswer = puzzles[puzzleIndex][1];
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);
        
        return userAnswer == correctAnswer;
    }
    
    bool solveSequencePuzzle() {
        vector<vector<int>> sequences;
        vector<int> answers;
        
        if (level == 1) {
            sequences = {{2, 4, 6, 8}, {5, 10, 15, 20}, {1, 3, 5, 7}};
            answers = {10, 25, 9};
        } else if (level == 2) {
            sequences = {{1, 4, 9, 16}, {3, 6, 12, 24}, {2, 6, 18, 54}};
            answers = {25, 48, 162};
        } else {
            sequences = {{1, 8, 27, 64}, {2, 3, 5, 8}, {1, 4, 10, 20}};
            answers = {125, 13, 35};
        }
        
        int puzzleIndex = rand() % sequences.size();
        
        cout << "\n🔢 SEQUENCE PUZZLE:\n";
        cout << "Find the next number in the sequence:\n";
        for (int num : sequences[puzzleIndex]) {
            cout << num << " ";
        }
        cout << "?\n";
        cout << "Answer: ";
        
        int userAnswer;
        cin >> userAnswer;
        
        return userAnswer == answers[puzzleIndex];
    }
    
    void displayWirePattern() {
        cout << "\n💣 BOMB STRUCTURE:\n";
        cout << "┌─────────────────────────────┐\n";
        cout << "│  ⚡ RED WIRE   - Memory     │\n";
        cout << "│  ⚡ BLUE WIRE  - Logic      │\n";
        cout << "│  ⚡ GREEN WIRE - Math       │\n";
        cout << "│                             │\n";
        cout << "│     ⏰ TIMER: TICKING       │\n";
        cout << "│                             │\n";
        cout << "│  ❗ CUT THE WRONG WIRE      │\n";
        cout << "│     AND BOOM! 💥            │\n";
        cout << "└─────────────────────────────┘\n";
    }
    
    void startDiffusion() {
        gameActive = true;
        bombDiffused = false;
        startTime = steady_clock::now();
        
        clearScreen();
        displayTitle();
        displayWirePattern();
        
        cout << "\n🚨 BOMB ACTIVATION DETECTED!\n";
        cout << "⚠️  You have " << timeLimit << " seconds to diffuse the bomb!\n";
        cout << "💡 Choose the correct wire to cut by solving puzzles!\n\n";
        
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        vector<string> wires = {"RED", "BLUE", "GREEN"};
        vector<string> wireTypes = {"Memory Challenge", "Logic Challenge", "Math Challenge"};
        string correctWire = wires[rand() % 3];
        
        cout << "\n🎯 The bomb has 3 wires. Each wire requires a different challenge:\n";
        cout << "🔴 RED WIRE   - Memory/Sequence Challenge\n";
        cout << "🔵 BLUE WIRE  - Logic Puzzle Challenge\n";
        cout << "🟢 GREEN WIRE - Mathematical Challenge\n\n";
        
        cout << "💡 HINT: The correct wire is the " << correctWire << " wire!\n";
        cout << "But first, you must solve its challenge to cut it safely!\n\n";
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        bool challengeSolved = false;
        
        while (gameActive && !bombDiffused) {
            if (checkTimeUp()) {
                gameActive = false;
                explodeBomb();
                return;
            }
            
            cout << "\n🔧 Which wire do you want to attempt? (1-Red, 2-Blue, 3-Green): ";
            int wireChoice;
            cin >> wireChoice;
            
            if (wireChoice < 1 || wireChoice > 3) {
                cout << "❌ Invalid choice! Try again.\n";
                continue;
            }
            
            string chosenWire = wires[wireChoice - 1];
            cout << "\n🎯 You chose the " << chosenWire << " wire!\n";
            
            if (checkTimeUp()) {
                gameActive = false;
                explodeBomb();
                return;
            }
            
            // Solve the challenge for the chosen wire
            bool puzzleSolved = false;
            
            if (wireChoice == 1) { // Red wire - Sequence
                puzzleSolved = solveSequencePuzzle();
            } else if (wireChoice == 2) { // Blue wire - Logic
                puzzleSolved = solveLogicPuzzle();
            } else { // Green wire - Math
                puzzleSolved = solveMathPuzzle();
            }
            
            if (checkTimeUp()) {
                gameActive = false;
                explodeBomb();
                return;
            }
            
            if (puzzleSolved && chosenWire == correctWire) {
                bombDiffused = true;
                gameActive = false;
                diffuseSuccess();
                return;
            } else if (puzzleSolved && chosenWire != correctWire) {
                cout << "\n⚠️  Puzzle solved correctly, but this isn't the right wire!\n";
                cout << "🔄 The bomb is still active. Choose another wire!\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            } else {
                cout << "\n❌ Wrong answer! The wire sparked but didn't cut.\n";
                cout << "⚡ Try a different approach or wire!\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
    
    void explodeBomb() {
        clearScreen();
        
        cout << "\n💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥\n";
        cout << "💥                                    💥\n";
        cout << "💥         ⏰ TIME'S UP! ⏰           💥\n";
        cout << "💥                                    💥\n";
        cout << "💥        🔥 BOOOOOOM! 🔥            💥\n";
        cout << "💥                                    💥\n";
        cout << "💥    The bomb has exploded! 💣      💥\n";
        cout << "💥                                    💥\n";
        cout << "💥        MISSION FAILED! ❌         💥\n";
        cout << "💥                                    💥\n";
        cout << "💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥💥\n";
        
        playSound("blast");
        
        cout << "\n😵 You failed to diffuse the bomb in time!\n";
        cout << "🔄 Better luck next time, agent!\n\n";
    }
    
    void diffuseSuccess() {
        clearScreen();
        
        cout << "\n🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉\n";
        cout << "🎉                                    🎉\n";
        cout << "🎉        ✅ SUCCESS! ✅             🎉\n";
        cout << "🎉                                    🎉\n";
        cout << "🎉       🏆 BOMB DIFFUSED! 🏆        🎉\n";
        cout << "🎉                                    🎉\n";
        cout << "🎉    You saved everyone! 🦸‍♂️         🎉\n";
        cout << "🎉                                    🎉\n";
        cout << "🎉      EXCELLENT WORK, AGENT!       🎉\n";
        cout << "🎉                                    🎉\n";
        cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉\n";
        
        playSound("success");
        
        auto endTime = steady_clock::now();
        auto elapsed = duration_cast<seconds>(endTime - startTime);
        int timeUsed = static_cast<int>(elapsed.count());
        int timeLeft = timeLimit - timeUsed;
        
        cout << "\n⏱️  Time remaining: " << timeLeft << " seconds\n";
        cout << "🎯 Difficulty Level: " << (level == 1 ? "Beginner" : level == 2 ? "Intermediate" : "Advanced") << "\n";
        cout << "⭐ Performance: " << (timeLeft > timeLimit/2 ? "EXCELLENT" : timeLeft > 10 ? "GOOD" : "CLOSE CALL") << "!\n\n";
    }
    
    void displayInstructions() {
        clearScreen();
        displayTitle();
        
        cout << "📋 GAME INSTRUCTIONS:\n\n";
        cout << "🎯 OBJECTIVE:\n";
        cout << "   Diffuse the bomb before time runs out!\n\n";
        
        cout << "🔧 HOW TO PLAY:\n";
        cout << "   1. Choose your difficulty level\n";
        cout << "   2. A bomb will activate with 3 colored wires\n";
        cout << "   3. Each wire requires solving a different puzzle\n";
        cout << "   4. Only ONE wire will diffuse the bomb\n";
        cout << "   5. Cut the wrong wire and the bomb stays active!\n";
        cout << "   6. Solve puzzles correctly to cut wires safely\n\n";
        
        cout << "⚠️  WARNING SYSTEMS:\n";
        cout << "   🔊 Beeping sounds when < 10 seconds remain\n";
        cout << "   ⏰ Timer display shows remaining time\n";
        cout << "   💥 Blast sounds if you fail!\n\n";
        
        cout << "🧩 PUZZLE TYPES:\n";
        cout << "   🔴 RED WIRE   - Number sequences & patterns\n";
        cout << "   🔵 BLUE WIRE  - Logic & reasoning puzzles\n";
        cout << "   🟢 GREEN WIRE - Mathematical calculations\n\n";
        
        cout << "Press any key to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        cin.get();
    }
    
    void mainMenu() {
        int choice;
        
        while (true) {
            clearScreen();
            displayTitle();
            
            cout << "🎮 MAIN MENU:\n\n";
            cout << "1. 🚀 Start New Mission\n";
            cout << "2. 📋 View Instructions\n";
            cout << "3. 🚪 Exit Game\n";
            cout << "\nEnter your choice: ";
            
            cin >> choice;
            
            switch (choice) {
                case 1:
                    selectLevel();
                    startDiffusion();
                    cout << "\nPress any key to return to menu...";
                    cin.ignore();
                    cin.get();
                    break;
                case 2:
                    displayInstructions();
                    break;
                case 3:
                    cout << "\n👋 Thanks for playing Bomb Diffuser Simulation!\n";
                    cout << "Stay safe, agent! 🕵️‍♂️\n";
                    return;
                default:
                    cout << "\n❌ Invalid choice! Please try again.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
};

int main() {
    // Hide cursor for better display
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    
    // Set console title
    SetConsoleTitle(TEXT("Bomb Diffuser Simulation - Agent Training"));
    
    BombDiffuser game;
    
    cout << "\n🚨 INITIALIZING BOMB DIFFUSER SIMULATION...\n";
    cout << "⚡ Loading explosive protocols...\n";
    cout << "🔧 Calibrating diffusion tools...\n";
    cout << "✅ System ready!\n\n";
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    game.mainMenu();
    
    return 0;
}
