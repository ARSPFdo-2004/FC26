#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50

/* Structure for a leaderboard node (circular linked list) */
typedef struct LBNode {
    int  user_id;
    char user_name[MAX_NAME_LEN];
    int  wins;
    int  level;
    struct LBNode* next;  /* circular */
} LBNode;

/* Structure for the leaderboard (circular linked list) */
typedef struct Leaderboard {
    LBNode* head;
    int count;
} Leaderboard;

/* INIT_LEADERBOARD(lb : REFERENCE TO Leaderboard) */
void initLeaderboard(Leaderboard* lb);

/* ADD_TO_LEADERBOARD(lb : REFERENCE TO Leaderboard,
                      user_id, wins, level : INTEGER, name : STRING) */
void addToLeaderboard(Leaderboard* lb, int user_id, const char* name,
                      int wins, int level);

/* REMOVE_FROM_LEADERBOARD(lb : REFERENCE TO Leaderboard, user_id : INTEGER) */
void removeFromLeaderboard(Leaderboard* lb, int user_id);

/* UPDATE_LEADERBOARD_ENTRY(lb : REFERENCE TO Leaderboard,
                             user_id, wins, level : INTEGER) */
void updateLeaderboardEntry(Leaderboard* lb, int user_id,
                             int wins, int level);

/* SORT_LEADERBOARD(lb : REFERENCE TO Leaderboard) — bubble sort by wins */
void sortLeaderboard(Leaderboard* lb);

/* DISPLAY_LEADERBOARD(lb : REFERENCE TO Leaderboard) */
void displayLeaderboard(Leaderboard* lb);

/* GET_USER_RANK(lb : REFERENCE TO Leaderboard, user_id : INTEGER)
   RETURNS INTEGER (1-based rank, or -1 if not found) */
int getUserRank(Leaderboard* lb, int user_id);

/* GET_LEVEL_NAME_LB(level : INTEGER) RETURNS STRING */
const char* getLevelNameLB(int level);

/* FREE_LEADERBOARD(lb : REFERENCE TO Leaderboard) */
void freeLeaderboard(Leaderboard* lb);

#endif /* LEADERBOARD_H */
