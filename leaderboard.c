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
                      int wins, int level) {
    LBNode* newNode = (LBNode *) malloc(sizeof(LBNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode -> user_id = user_id;
    strncpy(newNode -> user_name, name, MAX_NAME_LEN - 1);
    newNode -> user_name[MAX_NAME_LEN - 1] = '\0';
    newNode -> wins  = wins;
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
                             int wins, int level) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* current = lb -> head;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            current -> wins  = wins;
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
                int  tmp_level = current -> level;
                char tmp_name[MAX_NAME_LEN];
                strncpy(tmp_name, current -> user_name, MAX_NAME_LEN);

                current -> user_id = next_node -> user_id;
                current -> wins    = next_node -> wins;
                current -> level   = next_node -> level;
                strncpy(current -> user_name, next_node -> user_name, MAX_NAME_LEN);

                next_node -> user_id = tmp_id;
                next_node -> wins    = tmp_wins;
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

void displayLeaderboard(Leaderboard* lb) {
    if (lb -> head == NULL) {
        printf("Leaderboard is empty.\n");
        return;
    }

    printf("\n=== Global Leaderboard ===\n");
    printf("%-5s %-6s %-20s %-15s %-5s\n",
           "Rank", "ID", "Name", "Level", "Wins");
    printf("%-5s %-6s %-20s %-15s %-5s\n",
           "----", "------", "--------------------",
           "---------------", "-----");

    LBNode* current = lb -> head;
    int rank = 1;
    int i;

    for (i = 0; i < lb -> count; i++) {
        printf("%-5d %-6d %-20s %-15s %-5d\n",
               rank, current -> user_id, current -> user_name,
               getLevelNameLB(current -> level), current -> wins);
        current = current -> next;
        rank++;
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
