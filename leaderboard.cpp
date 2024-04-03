#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <sstream>
#include "structs.h"
#include "draw_console.h"
#include <iomanip>
using namespace std;


// Compare scores between players
bool comparePlayers(const Player& a, const Player& b) {
    return a.score > b.score;
}

// Add player information
void addPlayer(vector<Player> &leaderboard, const string &name, int score) {
    leaderboard.push_back({name, score});
    sort(leaderboard.begin(), leaderboard.end(), comparePlayers);
}

// Save Leaderboard to file
void saveLeaderboard(const vector<Player> &leaderboard, string stage_filename) {
    ofstream file(stage_filename.c_str());
    for (const auto& player : leaderboard) {
        file << player.name << " " << player.score << endl;
    }
    file.close();      
}
vector<Player> readLeaderboard(const string& filename) {
    ifstream file(filename);
    vector<Player> leaderboard;

    if (file.good()) {
        string line;
        while (getline(file, line)) {
            string::size_type pos = line.find('-'); // Find the position of the '-' character in the line
            if (pos != string::npos) { // If '-' character is found
                string name = line.substr(0, pos); // Extract player name from the beginning of the line up to before the '-'
                string scoreString = line.substr(pos + 1); // Extract score from after the '-' character
                int score = stoi(scoreString); // Convert the score string to an integer
                leaderboard.push_back({name, score}); // Add to leaderboard 
            }
        }
        file.close();
    }
    return leaderboard;
}

void Leaderboard(const vector<Player>& leaderboard) {
    // Print to Leaderboard
    GoToCursorPos(5, 35);
    cout << "\t\t   Leaderboard\n";
    GoToCursorPos(6, 35);
    cout << "\t---------------------------------\n";
    GoToCursorPos(7, 35);
    cout << "\tName\t\t\t    Time\n";
    GoToCursorPos(8, 35);
    cout << "\t---------------------------------\n";
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        GoToCursorPos(9 + i, 35);
        cout << "\t" << i + 1 << ". " << leaderboard[i].name ;
        GoToCursorPos(9 + i, 70);
        cout << leaderboard[i].score << "\n";
    }
    GoToCursorPos(30, 35);
    cout << "\t---------------------------------\n";
    GoToCursorPos(31, 35);
    cout << "\tPress Esc to return to main menu...\n";
}
// Draw Leaderboard
void DrawStageLeaderboard(string filename) {
    vector<Player> leaderboard = readLeaderboard(filename);
    Leaderboard(leaderboard);
}
