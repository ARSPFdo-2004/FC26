#include "squad.h"
#include "user_registration.h"

void initSquad(Squad* squad) {
    
    const char* types[SQUAD_SIZE] = {
        "FW","FW","FW",
        "MF","MF","MF",
        "DF","DF","DF","DF",
        "GK"
    };

    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        strncpy(squad -> slots[i].position_type, types[i], 3);
        squad -> slots[i].position_type[3] = '\0';
        squad -> slots[i].player_name[0]   = '\0';
        squad -> slots[i].native_type[0]   = '\0';
        squad -> slots[i].player_rating    = 0;
        squad -> slots[i].is_filled        = false;
    }
}

static InventoryNode* findBestOfType(PlayerInventory* inv, Squad* squad,
                                     const char* type) {
    InventoryNode* best    = NULL;
    InventoryNode* current = inv -> head;

    while (current != NULL) {
        if (strcasecmp(current -> type, type) == 0) {
            
            bool already_in = false;
            int k;
            for (k = 0; k < SQUAD_SIZE; k++) {
                if (squad -> slots[k].is_filled &&
                    strcasecmp(squad -> slots[k].player_name, current -> name) == 0) {
                    already_in = true;
                    break;
                }
            }
            if (!already_in) {
                if (best == NULL || current -> rating > best -> rating) {
                    best = current;
                }
            }
        }
        current = current -> next;
    }
    return best;
}

void autoSelectBestSquad(Squad* squad, PlayerInventory* inv) {
    
    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        squad -> slots[i].player_name[0] = '\0';
        squad -> slots[i].player_rating  = 0;
        squad -> slots[i].is_filled      = false;
    }

    for (i = 0; i < SQUAD_SIZE; i++) {
        InventoryNode* best = findBestOfType(inv, squad,
                                             squad -> slots[i].position_type);
        if (best != NULL) {
            strncpy(squad -> slots[i].player_name, best -> name, MAX_NAME_LEN - 1);
            squad -> slots[i].player_name[MAX_NAME_LEN - 1] = '\0';
            strncpy(squad -> slots[i].native_type, best -> type, 3);
            squad -> slots[i].native_type[3] = '\0';
            squad -> slots[i].player_rating = best -> rating;
            squad -> slots[i].is_filled     = true;
        }
    }

    printf("Auto-selection complete.\n");
    displaySquad(squad);
}

void displaySquad(Squad* squad) {
    printf("\n=== Current Squad ===\n");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "Slot", "Pos", "Player", "P Pos", "Rating");
    printf("%-5s %-5s %-25s %-6s %-7s\n",
           "----", "-----", "-------------------------", "------", "-------");

    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad -> slots[i].is_filled) {
            printf("%-5d %-5s %-25s %-6s %-7d\n",
                   i + 1,
                   squad -> slots[i].position_type,
                   squad -> slots[i].player_name,
                   squad -> slots[i].native_type,
                   squad -> slots[i].player_rating);
        } else {
            printf("%-5d %-5s %-25s %-6s %-7s\n",
                   i + 1,
                   squad -> slots[i].position_type,
                   "(empty)",
                   "-", "-");
        }
    }

    printf("\nAverage Squad Rating: %d\n\n", getSquadAverageRating(squad));
}


int getSquadAverageRating(Squad* squad) {
    int total  = 0;
    int filled = 0;
    int i;

    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad -> slots[i].is_filled) {
            total += squad -> slots[i].player_rating;
            filled++;
        }
    }

    if (filled == 0) {
        return 0;
    }
    return total / filled;
}

bool isPlayerInSquad(Squad* squad, const char* name) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled && isPartialMatch(squad->slots[i].player_name, name)) {
            return true;
        }
    }
    return false;
}

void swapPlayersInSquad(Squad* squad, const char* name1, const char* name2) {
    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            if (idx1 == -1 && isPartialMatch(squad->slots[i].player_name, name1)) idx1 = i;
            else if (idx2 == -1 && isPartialMatch(squad->slots[i].player_name, name2)) idx2 = i;
        }
    }

    if (idx1 != -1 && idx2 != -1) {
        char temp_name[MAX_NAME_LEN];
        char temp_native[4];
        int temp_rating;

        strncpy(temp_name, squad->slots[idx1].player_name, MAX_NAME_LEN-1);
        temp_name[MAX_NAME_LEN-1] = '\0';
        strncpy(temp_native, squad->slots[idx1].native_type, 3);
        temp_native[3] = '\0';
        temp_rating = squad->slots[idx1].player_rating;

        strncpy(squad->slots[idx1].player_name, squad->slots[idx2].player_name, MAX_NAME_LEN-1);
        squad->slots[idx1].player_name[MAX_NAME_LEN-1] = '\0';
        strncpy(squad->slots[idx1].native_type, squad->slots[idx2].native_type, 3);
        squad->slots[idx1].native_type[3] = '\0';
        squad->slots[idx1].player_rating = squad->slots[idx2].player_rating;

        strncpy(squad->slots[idx2].player_name, temp_name, MAX_NAME_LEN-1);
        squad->slots[idx2].player_name[MAX_NAME_LEN-1] = '\0';
        strncpy(squad->slots[idx2].native_type, temp_native, 3);
        squad->slots[idx2].native_type[3] = '\0';
        squad->slots[idx2].player_rating = temp_rating;

        printf("\nSwapped %s with %s.\n", squad->slots[idx2].player_name, squad->slots[idx1].player_name);
    } else {
        printf("\nUnable to find both players. Make sure they are in the squad.\n");
    }
}
void displaySquad(Squad* squad) {
    printf("\n=== Current Squad ===\n");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "Slot", "Pos", "Player", "P Pos", "Rating");
    printf("%-5s %-5s %-25s %-6s %-7s\n",
           "----", "-----", "-------------------------", "------", "-------");

    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad -> slots[i].is_filled) {
            printf("%-5d %-5s %-25s %-6s %-7d\n",
                   i + 1,
                   squad -> slots[i].position_type,
                   squad -> slots[i].player_name,
                   squad -> slots[i].native_type,
                   squad -> slots[i].player_rating);
        } else {
            printf("%-5d %-5s %-25s %-6s %-7s\n",
                   i + 1,
                   squad -> slots[i].position_type,
                   "(empty)",
                   "-", "-");
        }
    }

    printf("\nAverage Squad Rating: %d\n\n", getSquadAverageRating(squad));
}


int getSquadAverageRating(Squad* squad) {
    int total  = 0;
    int filled = 0;
    int i;

    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad -> slots[i].is_filled) {
            total += squad -> slots[i].player_rating;
            filled++;
        }
    }

    if (filled == 0) {
        return 0;
    }
    return total / filled;
}

bool isPlayerInSquad(Squad* squad, const char* name) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled && isPartialMatch(squad->slots[i].player_name, name)) {
            return true;
        }
    }
    return false;
}

void swapPlayersInSquad(Squad* squad, const char* name1, const char* name2) {
    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            if (idx1 == -1 && isPartialMatch(squad->slots[i].player_name, name1)) {
                idx1 = i;
            } else if (idx2 == -1 && isPartialMatch(squad->slots[i].player_name, name2)) {
                idx2 = i;
            }
        }
    }

    if (idx1 == -1) {
        printf("Player '%s' not found in LineUp.\n", name1);
        return;
    }
    if (idx2 == -1) {
        printf("Player '%s' not found in LineUp.\n", name2);
        return;
    }

    char temp_name[MAX_NAME_LEN];
    char temp_native[4];
    int temp_rating;

    strncpy(temp_name, squad->slots[idx1].player_name, MAX_NAME_LEN-1);
    temp_name[MAX_NAME_LEN-1] = '\0';
    strncpy(temp_native, squad->slots[idx1].native_type, 3);
    temp_native[3] = '\0';
    temp_rating = squad->slots[idx1].player_rating;

    strncpy(squad->slots[idx1].player_name, squad->slots[idx2].player_name, MAX_NAME_LEN-1);
    squad->slots[idx1].player_name[MAX_NAME_LEN-1] = '\0';
    strncpy(squad->slots[idx1].native_type, squad->slots[idx2].native_type, 3);
    squad->slots[idx1].native_type[3] = '\0';
    squad->slots[idx1].player_rating = squad->slots[idx2].player_rating;

    strncpy(squad->slots[idx2].player_name, temp_name, MAX_NAME_LEN-1);
    squad->slots[idx2].player_name[MAX_NAME_LEN-1] = '\0';
    strncpy(squad->slots[idx2].native_type, temp_native, 3);
    squad->slots[idx2].native_type[3] = '\0';
    squad->slots[idx2].player_rating = temp_rating;

    printf("Successfully swapped '%s' and '%s' in the LineUp.\n", squad->slots[idx1].player_name, squad->slots[idx2].player_name);
}
