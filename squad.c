#include "squad.h"
#include "user_registration.h"

void initSquad(Squad* squad) {
    const char* types[SQUAD_SIZE] = {
        "FW","FW","FW",
        "MF","MF","MF",
        "DF","DF","DF","DF",
        "GK"
    };
    for (int i = 0; i < SQUAD_SIZE; i++) {
        strncpy(squad->slots[i].position_type, types[i], 3);
        squad->slots[i].position_type[3] = '\0';
        squad->slots[i].player_name[0]   = '\0';
        squad->slots[i].native_type[0]   = '\0';
        squad->slots[i].player_rating    = 0;
        squad->slots[i].is_filled        = false;
    }
}

static InventoryNode* findBestOfType(PlayerInventory* inv, Squad* squad, const char* type) {
    InventoryNode* best = NULL;
    InventoryNode* current = inv->head;
    while (current != NULL) {
        if (strcasecmp(current->type, type) == 0) {
            bool already_in = false;
            for (int k = 0; k < SQUAD_SIZE; k++) {
                if (squad->slots[k].is_filled && strcasecmp(squad->slots[k].player_name, current->name) == 0) {
                    already_in = true; break;
                }
            }
            if (!already_in) {
                if (best == NULL || current->rating > best->rating) best = current;
            }
        }
        current = current->next;
    }
    return best;
}

void autoSelectBestSquad(Squad* squad, PlayerInventory* inv) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        squad->slots[i].player_name[0] = '\0';
        squad->slots[i].player_rating  = 0;
        squad->slots[i].is_filled      = false;
    }
    for (int i = 0; i < SQUAD_SIZE; i++) {
        InventoryNode* best = findBestOfType(inv, squad, squad->slots[i].position_type);
        if (best != NULL) {
            strncpy(squad->slots[i].player_name, best->name, MAX_NAME_LEN - 1);
            squad->slots[i].player_name[MAX_NAME_LEN - 1] = '\0';
            strncpy(squad->slots[i].native_type, best->type, 3);
            squad->slots[i].native_type[3] = '\0';
            squad->slots[i].player_rating = best->rating;
            squad->slots[i].is_filled     = true;
        }
    }
    printf("Auto-selection complete.\n");
    displaySquad(squad);
}

void displaySquad(Squad* squad) {
    printf("\n=== Current Squad ===\n");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "Slot", "Pos", "Player", "P Pos", "Rating");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "----", "-----", "-------------------------", "------", "-------");
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            printf("%-5d %-5s %-25s %-6s %-7d\n", i + 1, squad->slots[i].position_type, squad->slots[i].player_name, squad->slots[i].native_type, squad->slots[i].player_rating);
        } else {
            printf("%-5d %-5s %-25s %-6s %-7s\n", i + 1, squad->slots[i].position_type, "(empty)", "-", "-");
        }
    }
    printf("\nAverage Squad Rating: %d\n\n", getSquadAverageRating(squad));
}

int getSquadAverageRating(Squad* squad) {
    int total = 0, filled = 0;
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            total += squad->slots[i].player_rating;
            filled++;
        }
    }
    return (filled == 0) ? 0 : total / filled;
}

bool isPlayerInSquad(Squad* squad, const char* name) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled && isPartialMatch(squad->slots[i].player_name, name)) return true;
    }
    return false;
}

typedef struct TempCard {
    char name[MAX_NAME_LEN];
    char type[4];
    int rating;
    bool in_squad;
    int squad_slot;
} TempCard;

int compareCards(const void* a, const void* b) {
    TempCard* c1 = (TempCard*)a;
    TempCard* c2 = (TempCard*)b;
    // position sort: FW -> MF -> DF -> GK
    int pos1 = 0, pos2 = 0;
    if (strcmp(c1->type, "FW") == 0) pos1 = 1;
    else if (strcmp(c1->type, "MF") == 0) pos1 = 2;
    else if (strcmp(c1->type, "DF") == 0) pos1 = 3;
    else if (strcmp(c1->type, "GK") == 0) pos1 = 4;
    
    if (strcmp(c2->type, "FW") == 0) pos2 = 1;
    else if (strcmp(c2->type, "MF") == 0) pos2 = 2;
    else if (strcmp(c2->type, "DF") == 0) pos2 = 3;
    else if (strcmp(c2->type, "GK") == 0) pos2 = 4;
    
    if (pos1 != pos2) return pos1 - pos2;
    return c2->rating - c1->rating;
}

int compareCardsRating(const void* a, const void* b) {
    TempCard* c1 = (TempCard*)a;
    TempCard* c2 = (TempCard*)b;
    return c2->rating - c1->rating;
}

void displayAllUserCards(Squad* squad, PlayerInventory* inv) {
    TempCard cards[200];
    int count = 0;
    
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            strcpy(cards[count].name, squad->slots[i].player_name);
            strcpy(cards[count].type, squad->slots[i].native_type);
            cards[count].rating = squad->slots[i].player_rating;
            cards[count].in_squad = true;
            cards[count].squad_slot = i;
            count++;
        }
    }
    
    int squad_count = count;
    
    InventoryNode* curr = inv->head;
    while (curr != NULL) {
        bool already_in = false;
        for (int i = 0; i < SQUAD_SIZE; i++) {
            if (squad->slots[i].is_filled && strcmp(squad->slots[i].player_name, curr->name) == 0) {
                already_in = true; break;
            }
        }
        if (!already_in) {
            strcpy(cards[count].name, curr->name);
            strcpy(cards[count].type, curr->type);
            cards[count].rating = curr->rating;
            cards[count].in_squad = false;
            cards[count].squad_slot = -1;
            count++;
        }
        curr = curr->next;
    }
    
    // Lineup gets ordered by position FW -> MF -> DF -> GK
    qsort(cards, squad_count, sizeof(TempCard), compareCards);
    
    // Other players ordered strictly by rating descending
    if (count > squad_count) {
        qsort(cards + squad_count, count - squad_count, sizeof(TempCard), compareCardsRating);
    }
    
    printf("\n=== All Player Cards ===\n");
    printf("\n--- LINEUP ---\n");
    printf("%-3s %-25s %-6s %-7s\n", "#", "Player", "P Pos", "Rating");
    printf("%-3s %-25s %-6s %-7s\n", "---", "-------------------------", "------", "-------");
    
    for (int i = 0; i < squad_count; i++) {
        printf("%-3d %-25s %-6s %-7d\n", i + 1, cards[i].name, cards[i].type, cards[i].rating);
    }
    
    if (count > squad_count) {
        printf("\n--- OTHER PLAYERS ---\n");
        printf("--------------------------------------------\n");
        for (int i = squad_count; i < count; i++) {
            printf("%-3d %-25s %-6s %-7d\n", i + 1, cards[i].name, cards[i].type, cards[i].rating);
        }
    }
    printf("\n");
}

void swapPlayerCards(Squad* squad, PlayerInventory* inv, const char* name1, const char* name2) {
    int s1 = -1, s2 = -1;
    
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            if (s1 == -1 && isPartialMatch(squad->slots[i].player_name, name1)) s1 = i;
            if (s2 == -1 && isPartialMatch(squad->slots[i].player_name, name2)) s2 = i;
        }
    }
    
    InventoryNode* i1 = NULL;
    InventoryNode* i2 = NULL;
    InventoryNode* curr = inv->head;
    while (curr != NULL) {
        if (i1 == NULL && isPartialMatch(curr->name, name1)) i1 = curr;
        if (i2 == NULL && isPartialMatch(curr->name, name2)) i2 = curr;
        curr = curr->next;
    }
    
    if ((s1 == -1 && i1 == NULL) || (s2 == -1 && i2 == NULL)) {
        printf("One or both players not found.\n");
        return;
    }
    
    if (s1 != -1 && s2 != -1) {
        char temp_name[MAX_NAME_LEN];
        char temp_type[4];
        int temp_rating;
        
        strcpy(temp_name, squad->slots[s1].player_name);
        strcpy(temp_type, squad->slots[s1].native_type);
        temp_rating = squad->slots[s1].player_rating;
        
        strcpy(squad->slots[s1].player_name, squad->slots[s2].player_name);
        strcpy(squad->slots[s1].native_type, squad->slots[s2].native_type);
        squad->slots[s1].player_rating = squad->slots[s2].player_rating;
        
        strcpy(squad->slots[s2].player_name, temp_name);
        strcpy(squad->slots[s2].native_type, temp_type);
        squad->slots[s2].player_rating = temp_rating;
        
        printf("Swapped %s and %s in the squad.\n", squad->slots[s2].player_name, squad->slots[s1].player_name);
        return;
    }
    
    if (s1 != -1 && s2 == -1 && i2 != NULL) {
        printf("Replaced %s with %s in the squad.\n", squad->slots[s1].player_name, i2->name);
        strcpy(squad->slots[s1].player_name, i2->name);
        strcpy(squad->slots[s1].native_type, i2->type);
        squad->slots[s1].player_rating = i2->rating;
        return;
    }
    
    if (s2 != -1 && s1 == -1 && i1 != NULL) {
        printf("Replaced %s with %s in the squad.\n", squad->slots[s2].player_name, i1->name);
        strcpy(squad->slots[s2].player_name, i1->name);
        strcpy(squad->slots[s2].native_type, i1->type);
        squad->slots[s2].player_rating = i1->rating;
        return;
    }
    
    printf("At least one player must be in the squad to swap.\n");
}
