#ifndef GAME_LEADER_BOARD_H
#define GAME_LEADER_BOARD_H

#include "structs.h"
#include "draw_console.h"
#include <vector> 

void DrawStageLeaderboard(string filename);

void Leaderboard(const vector<Player>& leaderboard);

vector<Player> readLeaderboard(const string& filename);

void addPlayer(vector<Player> &leaderboard, const string &name, int score);
#endif // GAME_LEADER_BOARD_H 