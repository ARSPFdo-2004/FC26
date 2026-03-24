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

typedef struct MatchRecord {
    char opponent_name[MAX_NAME_LEN];
    char result;        
    int  coins_earned;
    int  week;
    int  match_number;
    int  my_goals;
    int  opp_goals;
    int  my_shots;
    int  opp_shots;
    int  my_assists;
    int  opp_assists;
    int  my_fouls;
    int  opp_fouls;
    int  my_offsides;
    int  opp_offsides;
    int  my_red_cards;
    int  opp_red_cards;
    int  my_yellow_cards;
    int  opp_yellow_cards;
    struct MatchRecord* next;
} MatchRecord;

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

void initMatchStack(MatchStack* stack);

bool isStackEmpty(MatchStack* stack);

void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week,
               int my_goals, int opp_goals,
               int my_shots, int opp_shots,
               int my_assists, int opp_assists,
               int my_fouls, int opp_fouls,
               int my_offsides, int opp_offsides,
               int my_red_cards, int opp_red_cards,
               int my_yellow_cards, int opp_yellow_cards);
void displayMatchHistory(MatchStack* stack);

void displayStatistics(MatchStack* stack, int week, int match_num, const char* user_name);

float getWinPercentage(MatchStack* stack);

float getWeeklyWinPercentage(MatchStack* stack);

void resetWeeklyStats(MatchStack* stack);

void freeMatchStack(MatchStack* stack);

#endif 
