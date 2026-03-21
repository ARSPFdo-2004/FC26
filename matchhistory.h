#ifndef MATCHHISTORY_H
#define MATCHHISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN  50
#define RESULT_WIN    'W'
#define RESULT_LOSS   'L'
#define RESULT_DRAW   'D'

/* Structure for a match record (stack node) */
typedef struct MatchRecord {
    char opponent_name[MAX_NAME_LEN];
    char result;        /* 'W', 'L', or 'D' */
    int  coins_earned;
    int  week;
    struct MatchRecord* next;
} MatchRecord;

/* Structure for a match history stack */
typedef struct MatchStack {
    MatchRecord* top;
    int total_matches;
    int total_wins;
    int total_losses;
    int total_draws;
    int week_wins;
    int week_matches;
    int current_week;
} MatchStack;

/* INIT_MATCH_STACK(stack : REFERENCE TO MatchStack) */
void initMatchStack(MatchStack* stack);

/* IS_STACK_EMPTY(stack : REFERENCE TO MatchStack) RETURNS BOOLEAN */
bool isStackEmpty(MatchStack* stack);

/* PUSH_MATCH(stack : REFERENCE TO MatchStack, opponent : STRING,
              result : CHAR, coins : INTEGER, week : INTEGER) */
void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week);

/* POP_MATCH(stack : REFERENCE TO MatchStack) */
void popMatch(MatchStack* stack);

/* DISPLAY_MATCH_HISTORY(stack : REFERENCE TO MatchStack) */
void displayMatchHistory(MatchStack* stack);

/* GET_WIN_PERCENTAGE(stack : REFERENCE TO MatchStack) RETURNS FLOAT */
float getWinPercentage(MatchStack* stack);

/* GET_WEEKLY_WIN_PERCENTAGE(stack : REFERENCE TO MatchStack) RETURNS FLOAT */
float getWeeklyWinPercentage(MatchStack* stack);

/* RESET_WEEKLY_STATS(stack : REFERENCE TO MatchStack) */
void resetWeeklyStats(MatchStack* stack);

/* FREE_MATCH_STACK(stack : REFERENCE TO MatchStack) */
void freeMatchStack(MatchStack* stack);

#endif /* MATCHHISTORY_H */
