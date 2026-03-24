#include "leaderboard.h"

static const char* LB_LEVEL_NAMES[] = {
    "Amateur 3", "Amateur 2", "Amateur 1",
    "Pro 3",     "Pro 2",     "Pro 1",
    "World Class 2", "World Class 1",
    "Legendary"
};

const char* getLevelNameLB(int level) {
    if (level < 0 || level > 8) {
        return "Unknown";
    }
    return LB_LEVEL_NAMES[level];
}

void initLeaderboard(Leaderboard* lb) {
    lb -> head  = NULL;
    lb -> count = 0;
}

void addToLeaderboard(Leaderboard* lb, int user_id, const char* name,
                      int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level) {
    LBNode* newNode = (LBNode *) malloc(sizeof(LBNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode -> user_id = user_id;
    strncpy(newNode -> user_name, name, MAX_NAME_LEN - 1);
    newNode -> user_name[MAX_NAME_LEN - 1] = '\0';
    newNode -> wins  = wins;
    newNode -> losses = losses;
    newNode -> draws = draws;
    newNode -> win_percentage = win_percentage;
    newNode -> week_win_percentage = week_win_percentage;
    newNode -> level = level;
    newNode -> next  = NULL;

    if (lb -> head == NULL) {
        newNode -> next = newNode;  
        lb -> head = newNode;
    } else {
        
        LBNode* tail = lb -> head;
        while (tail -> next != lb -> head) {
            tail = tail -> next;
        }
        tail -> next    = newNode;
        newNode -> next = lb -> head;
    }

    lb -> count++;
    sortLeaderboard(lb);
}

void removeFromLeaderboard(Leaderboard* lb, int user_id) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* current  = lb -> head;
    LBNode* previous = NULL;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            if (lb -> count == 1) {
                lb -> head = NULL;
            } else {
                if (previous == NULL) {
                    
                    LBNode* tail = lb -> head;
                    while (tail -> next != lb -> head) {
                        tail = tail -> next;
                    }
                    lb -> head   = current -> next;
                    tail -> next = lb -> head;
                } else {
                    previous -> next = current -> next;
                }
            }
            free(current);
            lb -> count--;
            return;
        }
        previous = current;
        current  = current -> next;
    }
}

void updateLeaderboardEntry(Leaderboard* lb, int user_id,
                             int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* current = lb -> head;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            current -> wins  = wins;
            current -> losses = losses;
            current -> draws = draws;
            current -> win_percentage = win_percentage;
            current -> week_win_percentage = week_win_percentage;
            current -> level = level;
            sortLeaderboard(lb);
            return;
        }
        current = current -> next;
    }
}

void sortLeaderboard(Leaderboard* lb) {
    if (lb -> count <= 1) {
        return;
    }

    bool swapped;
    int pass;

    for (pass = 0; pass < lb -> count - 1; pass++) {
        swapped = false;
        LBNode* current = lb -> head;
        int i;

        for (i = 0; i < lb -> count - 1 - pass; i++) {
            LBNode* next_node = current -> next;

            if (current -> wins < next_node -> wins) {
                
                int  tmp_id    = current -> user_id;
                int  tmp_wins  = current -> wins;
                int  tmp_losses = current -> losses;
                int  tmp_draws  = current -> draws;
                float tmp_win_perc = current -> win_percentage;
                float tmp_week_win_perc = current -> week_win_percentage;
                int  tmp_level = current -> level;
                char tmp_name[MAX_NAME_LEN];
                strncpy(tmp_name, current -> user_name, MAX_NAME_LEN);

                current -> user_id = next_node -> user_id;
                current -> wins    = next_node -> wins;
                current -> losses  = next_node -> losses;
                current -> draws   = next_node -> draws;
                current -> win_percentage = next_node -> win_percentage;
                current -> week_win_percentage = next_node -> week_win_percentage;
                current -> level   = next_node -> level;
                strncpy(current -> user_name, next_node -> user_name, MAX_NAME_LEN);

                next_node -> user_id = tmp_id;
                next_node -> wins    = tmp_wins;
                next_node -> losses  = tmp_losses;
                next_node -> draws   = tmp_draws;
                next_node -> win_percentage = tmp_win_perc;
                next_node -> week_win_percentage = tmp_week_win_perc;
                next_node -> level   = tmp_level;
                strncpy(next_node -> user_name, tmp_name, MAX_NAME_LEN);

                swapped = true;
            }
            current = current -> next;
        }

        if (!swapped) {
            break;
        }
    }
}

static int getSimulatedRank(int level, int real_rank) {
    int base_ranks[] = {165, 125, 85, 65, 45, 25, 15, 5, 0};
    if (level >= 0 && level <= 8) {
        int base = base_ranks[level];
        return (real_rank > base) ? real_rank : (base + real_rank);
    }
    return real_rank;
}

void displayLeaderboard(Leaderboard* lb, int current_user_id) {
    if (lb -> head == NULL) {
        printf("Leaderboard is empty.\n");
        return;
    }

    printf("\n=== Global Leaderboard ===\n");
    printf("%-5s | %-20s | %-15s | %-5s | %-6s | %-5s | %-6s\n",
           "Rank", "Name", "Level", "Wins", "Losses", "Draws", "Win %");
    printf("--------------------------------------------------------------------------------\n");

    LBNode* current = lb -> head;
    int rank = 1;
    int i;
    int last_level = -1;
    int displayed_count = 0;
    
    LBNode* current_user_node = NULL;
    int current_user_rank = -1;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == current_user_id) {
            current_user_node = current;
            current_user_rank = rank;
        }

        if (displayed_count < 8) {
            if (last_level != -1 && last_level != current->level) {
                printf("--------------------------------------------------------------------------------\n");
            }
            
            float total_matches = current->wins + current->losses + current->draws;
            float win_perc = 0.0;
            if (total_matches > 0) {
                win_perc = (current->wins / total_matches) * 100.0;
            }
            
            int sim_rank = getSimulatedRank(current->level, rank);
            printf("%-5d | %-20s | %-15s | %-5d | %-6d | %-5d | %-5.1f%%\n",
                   sim_rank, current -> user_name,
                   getLevelNameLB(current -> level), current -> wins, current -> losses, current->draws, win_perc);
            
            last_level = current->level;
            displayed_count++;
        }
        
        current = current -> next;
        rank++;
    }
    
    if (current_user_node != NULL && current_user_rank > 8) {
        printf("--------------------------------------------------------------------------------\n");
        for (int j = 0; j < 5; j++) {
            printf("%-5s | %-20s | %-15s | %-5s | %-6s | %-5s | %-5s\n", "", "", "", "", "", "", "");
        }
        printf("--------------------------------------------------------------------------------\n");
        float total_matches = current_user_node->wins + current_user_node->losses + current_user_node->draws;
        float win_perc = 0.0;
        if (total_matches > 0) {
            win_perc = (current_user_node->wins / total_matches) * 100.0;
        }
        int sim_user_rank = getSimulatedRank(current_user_node->level, current_user_rank);
        printf("%-5d | %-20s | %-15s | %-5d | %-6d | %-5d | %-5.1f%%\n",
               sim_user_rank, current_user_node -> user_name,
               getLevelNameLB(current_user_node -> level), current_user_node -> wins, current_user_node -> losses, current_user_node->draws, win_perc);
    }
    
    printf("\n");
}

int getUserRank(Leaderboard* lb, int user_id) {
    if (lb -> head == NULL) {
        return -1;
    }

    LBNode* current = lb -> head;
    int rank = 1;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            return rank;
        }
        current = current -> next;
        rank++;
    }
    return -1;
}

void freeLeaderboard(Leaderboard* lb) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* tail = lb -> head;
    while (tail -> next != lb -> head) {
        tail = tail -> next;
    }
    tail -> next = NULL;

    LBNode* current = lb -> head;
    while (current != NULL) {
        LBNode* temp = current;
        current = current -> next;
        free(temp);
    }
    lb -> head  = NULL;
    lb -> count = 0;
}
