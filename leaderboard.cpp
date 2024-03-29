#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <sstream>
#include "structs.h"
#include "draw_console.h"
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
        cout << "\t" << i + 1 << ". " << leaderboard[i].name << "\t\t" << leaderboard[i].score << "\n";
    }
    GoToCursorPos(20, 35);
    cout << "\t---------------------------------\n";
    GoToCursorPos(21, 35);
    cout << "\tPress Esc to return to main menu...\n";
}
void DrawStageLeaderboard(){
    // cout << SetColor(0, 0 , kBackgroundBlack);
    vector<Player> leaderboard = readLeaderboard("stage_filename.txt");
    Leaderboard(leaderboard);
}

// Vẽ Leaderboard
// void DrawStageLeaderboard(string stage_filename, vector<Player> &leaderboard){
//     // Nạp danh sách từ file vào Leaderboard
//     leaderboard.clear();
//     ifstream fin(stage_filename);
//     string name;
//     int score;
//     while (fin >> name >> score) {
//         leaderboard.push_back({name, score});
//     }
//     fin.close();
//     // In ra Leaderboard
//     GoToCursorPos(5, 35);
//     cout << "\t\tLeaderboard\n";
//     GoToCursorPos(6, 35);
//     cout << "\t---------------------------------\n";
//     GoToCursorPos(7, 35);
//     cout << "\tName\t\tScore\n";
//     GoToCursorPos(8, 35);
//     cout << "\t---------------------------------\n";
//     for (const auto& player : leaderboard) {
//         GoToCursorPos(9, 35);
//         cout << player.name << "\t\t" << player.score << "\n";
//     }
//     GoToCursorPos(10, 35);
//     cout << "\t---------------------------------\n";
    
// }

// int main() {
//     string stage_filename = "3x4 - Easy";
//     vector<Player> leaderboard = {
//         {"Player 1", 100},
//         {"Player 2", 150},
//         {"Player 3", 80},
//         {"Player 4", 200}
//     };

//     bool running = true;
//     while (running) {
//         DrawStageLeaderboard(stage_filename, leaderboard);
//         cout << "Press Esc to return to main menu...\n";
        
//         while (!_kbhit()) {} // Chờ người dùng nhấn một phím
//         if (_getch() == 27) { // Kiểm tra xem phím nhấn có phải là Esc (mã ASCII: 27) không
//             running = false; // Thoát khỏi vòng lặp
//             // Trở về main menu
//         }
//     }

//     return 0;
// }