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

// So sánh điểm số giữa các người chơi
bool comparePlayers(const Player& a, const Player& b) {
    return a.score > b.score;
}

// Thêm thông tin người chơi
void addPlayer(vector<Player> &leaderboard, const string &name, int score) {
    leaderboard.push_back({name, score});
    sort(leaderboard.begin(), leaderboard.end(), comparePlayers);
}

// Lưu bảng xếp hạng vào file
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
            string::size_type pos = line.find('-'); // Tìm vị trí của dấu '-'
            if (pos != string::npos) { // Nếu tìm thấy dấu '-'
                string name = line.substr(0, pos); // Lấy phần tên từ đầu đến trước dấu '-'
                string scoreString = line.substr(pos + 1); // Lấy phần điểm từ sau dấu '-'
                int score = stoi(scoreString); // Chuyển đổi phần điểm thành số nguyên
                leaderboard.push_back({name, score}); // Thêm vào leaderboard
            }
        }
        file.close();
    }
    return leaderboard;
}

void Leaderboard(const vector<Player>& leaderboard) {
    // In ra Leaderboard
    GoToCursorPos(5, 35);
    cout << "\t\t   Leaderboard\n";
    GoToCursorPos(6, 35);
    cout << "\t---------------------------------\n";
    GoToCursorPos(7, 35);
    cout << "\tName\t\t\t    Score\n";
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
// Vẽ leaderboard
void DrawStageLeaderboard(string filename) {
    vector<Player> leaderboard = readLeaderboard(filename);
    Leaderboard(leaderboard);
}
