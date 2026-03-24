#include "matchhistory.h"

void initMatchStack(MatchStack* stack) {
    stack -> top           = NULL;
    stack -> total_matches = 0;
    stack -> total_wins    = 0;
    stack -> total_losses  = 0;
    stack -> total_draws   = 0;
    stack -> week_wins     = 0;
    stack -> week_matches  = 0;
    stack -> current_week  = 1;
}

bool isStackEmpty(MatchStack* stack) {
    return stack -> top == NULL;
}

void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week,
               int my_goals, int opp_goals,
               int my_shots, int opp_shots,
               int my_assists, int opp_assists,
               int my_fouls, int opp_fouls,
               int my_offsides, int opp_offsides,
               int my_red_cards, int opp_red_cards,
               int my_yellow_cards, int opp_yellow_cards) {
    MatchRecord* record = (MatchRecord *) malloc(sizeof(MatchRecord));
    if (record == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(record -> opponent_name, opponent, MAX_NAME_LEN - 1);
    record -> opponent_name[MAX_NAME_LEN - 1] = '\0';
    record -> result       = result;
    record -> coins_earned = coins;
    record -> week         = week;
    
    int match_count = 0;
    MatchRecord* curr = stack->top;
    while (curr) {
        if (curr->week == week) match_count++;
        curr = curr->next;
    }
    record -> match_number = match_count + 1;
    
    record -> my_goals = my_goals;
    record -> opp_goals = opp_goals;
    record -> my_shots = my_shots;
    record -> opp_shots = opp_shots;
    record -> my_assists = my_assists;
    record -> opp_assists = opp_assists;
    record -> my_fouls = my_fouls;
    record -> opp_fouls = opp_fouls;
    record -> my_offsides = my_offsides;
    record -> opp_offsides = opp_offsides;
    record -> my_red_cards = my_red_cards;
    record -> opp_red_cards = opp_red_cards;
    record -> my_yellow_cards = my_yellow_cards;
    record -> opp_yellow_cards = opp_yellow_cards;

    record -> next         = stack -> top;

    stack -> top = record;
    stack -> total_matches++;

    if (result == RESULT_WIN) {
        stack -> total_wins++;
        stack -> week_wins++;
        stack -> week_matches++;
    } else if (result == RESULT_LOSS) {
        stack -> total_losses++;
        stack -> week_matches++;
    } else if (result == RESULT_DRAW) {
        stack -> total_draws++;
        stack -> week_matches++;
    }
}

void popMatch(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("Match history is empty.\n");
        return;
    }

    MatchRecord* temp = stack -> top;
    stack -> top        = stack -> top->next;

    stack -> total_matches--;
    if (temp -> result == RESULT_WIN) {
        stack -> total_wins--;
    } else if (temp -> result == RESULT_LOSS) {
        stack -> total_losses--;
    } else if (temp -> result == RESULT_DRAW) {
        stack -> total_draws--;
    }

    free(temp);
}

void displayMatchHistory(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("No match history available.\n");
        return;
    }

    printf("\n=== Match History (Recent 2 Weeks Max) ===\n");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "#", "Opponent", "Result", "Match No", "Week");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "---", "--------------------", "------", "--------", "----");

    MatchRecord* current = stack -> top;
    int num = 1;

    int latest_week = -1;
    int prev_week = -1;

    if (current != NULL) {
        latest_week = current->week;
    }

    while (current != NULL) {
        if (current->week != latest_week) {
            if (prev_week == -1) {
                prev_week = current->week;
            } else if (current->week != prev_week) {
                break; // Stop after maximum 2 distinct weeks
            }
        }

        const char* result_str;
        if (current -> result == RESULT_WIN) {
            result_str = "WIN";
        } else if (current -> result == RESULT_LOSS) {
            result_str = "LOSS";
        } else {
            result_str = "DRAW";
        }

        printf("%-5d %-20s %-7s %-10d %-5d\n",
               num, current -> opponent_name,
               result_str, current -> match_number, current -> week);
        current = current -> next;
        num++;
    }

    printf("\nTotal: %d matches | %d wins | %d losses | %d draws\n",
           stack -> total_matches, stack -> total_wins,
           stack -> total_losses, stack -> total_draws);
}

float getWinPercentage(MatchStack* stack) {
    if (stack -> total_matches == 0) {
        return 0.0f;
    }
    return (float)stack -> total_wins / (float)stack -> total_matches * 100.0f;
}

float getWeeklyWinPercentage(MatchStack* stack) {
    if (stack -> week_matches == 0) {
        return 0.0f;
    }
    return (float)stack -> week_wins / (float)stack -> week_matches * 100.0f;
}

void resetWeeklyStats(MatchStack* stack) {
    stack -> week_wins    = 0;
    stack -> week_matches = 0;
    stack -> current_week++;
}

void freeMatchStack(MatchStack* stack) {
    while (!isStackEmpty(stack)) {
        popMatch(stack);
    }
}

void displayStatistics(MatchStack* stack, int week, int match_num, const char* user_name) {
    if (week < stack->current_week - 3 || week > stack->current_week - 1) {
        printf("Statistics are only available for the last 3 weeks (Weeks %d, %d, %d).\n", 
               stack->current_week - 1, stack->current_week - 2, stack->current_week - 3);
        return;
    }
    
    MatchRecord* curr = stack->top;
    while (curr) {
        if (curr->week == week && curr->match_number == match_num) {
            printf("\n");
            printf("                        === Match Statistics ===\n");
            printf("                       Opponent: %s\n", curr->opponent_name);
            printf("-----------------------------------------------------------------------\n");
            printf("             %20s | %-20s\n", user_name, curr->opponent_name);
            printf("-----------------------------------------------------------------------\n");
            printf("      Goals: %20d | %-20d\n", curr->my_goals, curr->opp_goals);
            printf("Shots on Goal: %18d | %-20d\n", curr->my_shots, curr->opp_shots);
            printf("    Assists: %20d | %-20d\n", curr->my_assists, curr->opp_assists);
            printf("      Fouls: %20d | %-20d\n", curr->my_fouls, curr->opp_fouls);
            printf("   Offsides: %20d | %-20d\n", curr->my_offsides, curr->opp_offsides);
            printf("  Red Cards: %20d | %-20d\n", curr->my_red_cards, curr->opp_red_cards);
            printf("Yellow Cards: %19d | %-20d\n", curr->my_yellow_cards, curr->opp_yellow_cards);
            printf("-----------------------------------------------------------------------\n");
            return;
        }
        curr = curr->next;
    }
    printf("Match not found for Week %d, Match Number %d.\n", week, match_num);
}
