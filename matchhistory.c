#include "matchhistory.h"

/* INIT_MATCH_STACK(stack : REFERENCE TO MatchStack) */
void initMatchStack(MatchStack* stack) {
    stack->top           = NULL;
    stack->total_matches = 0;
    stack->total_wins    = 0;
    stack->total_losses  = 0;
    stack->total_draws   = 0;
    stack->week_wins     = 0;
    stack->week_matches  = 0;
    stack->current_week  = 1;
}

/* IS_STACK_EMPTY(stack : REFERENCE TO MatchStack) RETURNS BOOLEAN */
bool isStackEmpty(MatchStack* stack) {
    return stack->top == NULL;
}

/* PUSH_MATCH(stack : REFERENCE TO MatchStack, opponent : STRING,
              result : CHAR, coins : INTEGER, week : INTEGER) */
void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week) {
    MatchRecord* record = (MatchRecord*)malloc(sizeof(MatchRecord));
    if (record == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(record->opponent_name, opponent, MAX_NAME_LEN - 1);
    record->opponent_name[MAX_NAME_LEN - 1] = '\0';
    record->result       = result;
    record->coins_earned = coins;
    record->week         = week;
    record->next         = stack->top;

    stack->top = record;
    stack->total_matches++;

    if (result == RESULT_WIN) {
        stack->total_wins++;
        stack->week_wins++;
        stack->week_matches++;
    } else if (result == RESULT_LOSS) {
        stack->total_losses++;
        stack->week_matches++;
    } else if (result == RESULT_DRAW) {
        stack->total_draws++;
        stack->week_matches++;
    }
}

/* POP_MATCH(stack : REFERENCE TO MatchStack) */
void popMatch(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("Match history is empty.\n");
        return;
    }

    MatchRecord* temp = stack->top;
    stack->top        = stack->top->next;

    /* Update counters */
    stack->total_matches--;
    if (temp->result == RESULT_WIN) {
        stack->total_wins--;
    } else if (temp->result == RESULT_LOSS) {
        stack->total_losses--;
    } else if (temp->result == RESULT_DRAW) {
        stack->total_draws--;
    }

    free(temp);
}

/* DISPLAY_MATCH_HISTORY(stack : REFERENCE TO MatchStack) */
void displayMatchHistory(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("No match history available.\n");
        return;
    }

    printf("\n=== Match History (most recent first) ===\n");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "#", "Opponent", "Result", "Coins", "Week");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "---", "--------------------", "------", "----------", "----");

    MatchRecord* current = stack->top;
    int num = 1;

    while (current != NULL) {
        const char* result_str;
        if (current->result == RESULT_WIN) {
            result_str = "WIN";
        } else if (current->result == RESULT_LOSS) {
            result_str = "LOSS";
        } else {
            result_str = "DRAW";
        }

        printf("%-5d %-20s %-7s %-10d %-5d\n",
               num, current->opponent_name,
               result_str, current->coins_earned, current->week);
        current = current->next;
        num++;
    }

    printf("\nTotal: %d matches | %d wins | %d losses | %d draws\n",
           stack->total_matches, stack->total_wins,
           stack->total_losses, stack->total_draws);
    printf("Overall win rate : %.1f%%\n", getWinPercentage(stack));
    printf("This week win rate: %.1f%% (%d/%d)\n\n",
           getWeeklyWinPercentage(stack),
           stack->week_wins, stack->week_matches);
}

/* GET_WIN_PERCENTAGE(stack : REFERENCE TO MatchStack) RETURNS FLOAT */
float getWinPercentage(MatchStack* stack) {
    if (stack->total_matches == 0) {
        return 0.0f;
    }
    return (float)stack->total_wins / (float)stack->total_matches * 100.0f;
}

/* GET_WEEKLY_WIN_PERCENTAGE(stack : REFERENCE TO MatchStack) RETURNS FLOAT */
float getWeeklyWinPercentage(MatchStack* stack) {
    if (stack->week_matches == 0) {
        return 0.0f;
    }
    return (float)stack->week_wins / (float)stack->week_matches * 100.0f;
}

/* RESET_WEEKLY_STATS(stack : REFERENCE TO MatchStack) */
void resetWeeklyStats(MatchStack* stack) {
    stack->week_wins    = 0;
    stack->week_matches = 0;
    stack->current_week++;
}

/* FREE_MATCH_STACK(stack : REFERENCE TO MatchStack) */
void freeMatchStack(MatchStack* stack) {
    while (!isStackEmpty(stack)) {
        popMatch(stack);
    }
}
