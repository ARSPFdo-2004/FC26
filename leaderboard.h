#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50

typedef struct LBNode {
    int  user_id;
    char user_name[MAX_NAME_LEN];
    int  wins;
    int  losses;
    int  draws;
    float win_percentage;
    float week_win_percentage;
    int  level;
    struct LBNode* next;
} LBNode;

typedef struct Leaderboard {
    LBNode* head;
    int count;
} Leaderboard;

void initLeaderboard(Leaderboard* lb);

void addToLeaderboard(Leaderboard* lb, int user_id, const char* name,
                      int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level);

void removeFromLeaderboard(Leaderboard* lb, int user_id);

void updateLeaderboardEntry(Leaderboard* lb, int user_id,
                             int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level);

void sortLeaderboard(Leaderboard* lb);

void displayLeaderboard(Leaderboard* lb, int current_user_id);

int getUserRank(Leaderboard* lb, int user_id);

const char* getLevelNameLB(int level);

void freeLeaderboard(Leaderboard* lb);

#endif 
