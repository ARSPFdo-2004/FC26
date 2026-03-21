#include "squad.h"

/* INIT_SQUAD(squad : REFERENCE TO Squad) */
void initSquad(Squad* squad) {
    /* Slots 0-2: FW, 3-5: MF, 6-9: DF, 10: GK */
    const char* types[SQUAD_SIZE] = {
        "FW","FW","FW",
        "MF","MF","MF",
        "DF","DF","DF","DF",
        "GK"
    };

    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        strncpy(squad->slots[i].position_type, types[i], 3);
        squad->slots[i].position_type[3] = '\0';
        squad->slots[i].player_name[0]   = '\0';
        squad->slots[i].player_rating    = 0;
        squad->slots[i].is_filled        = false;
    }
}

/* Helper: find best player of a given type in inventory (not already in squad) */
static InventoryNode* findBestOfType(PlayerInventory* inv, Squad* squad,
                                     const char* type) {
    InventoryNode* best    = NULL;
    InventoryNode* current = inv->head;

    while (current != NULL) {
        if (strcasecmp(current->type, type) == 0) {
            /* Check not already assigned to a slot */
            bool already_in = false;
            int k;
            for (k = 0; k < SQUAD_SIZE; k++) {
                if (squad->slots[k].is_filled &&
                    strcasecmp(squad->slots[k].player_name, current->name) == 0) {
                    already_in = true;
                    break;
                }
            }
            if (!already_in) {
                if (best == NULL || current->rating > best->rating) {
                    best = current;
                }
            }
        }
        current = current->next;
    }
    return best;
}

/* AUTO_SELECT_BEST_SQUAD(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory) */
void autoSelectBestSquad(Squad* squad, PlayerInventory* inv) {
    /* Clear current squad */
    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        squad->slots[i].player_name[0] = '\0';
        squad->slots[i].player_rating  = 0;
        squad->slots[i].is_filled      = false;
    }

    /* Fill each slot with the best available player of the required type */
    for (i = 0; i < SQUAD_SIZE; i++) {
        InventoryNode* best = findBestOfType(inv, squad,
                                             squad->slots[i].position_type);
        if (best != NULL) {
            strncpy(squad->slots[i].player_name, best->name, MAX_NAME_LEN - 1);
            squad->slots[i].player_name[MAX_NAME_LEN - 1] = '\0';
            squad->slots[i].player_rating = best->rating;
            squad->slots[i].is_filled     = true;
        }
    }

    printf("Auto-selection complete.\n");
    displaySquad(squad);
}

/* REPLACE_PLAYER(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory,
                  existing_name : STRING, new_name : STRING) */
void replacePlayer(Squad* squad, PlayerInventory* inv,
                   const char* existing_name, const char* new_name) {
    /* Find the slot with existing_name */
    int slot_idx = -1;
    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled &&
            strcasecmp(squad->slots[i].player_name, existing_name) == 0) {
            slot_idx = i;
            break;
        }
    }

    if (slot_idx == -1) {
        printf("Player '%s' not found in squad.\n", existing_name);
        return;
    }

    /* Find new_name in inventory */
    InventoryNode* new_player = searchInventoryByName(inv, new_name);
    if (new_player == NULL) {
        printf("Player '%s' not found in inventory.\n", new_name);
        return;
    }

    /* Replace */
    strncpy(squad->slots[slot_idx].player_name,
            new_player->name, MAX_NAME_LEN - 1);
    squad->slots[slot_idx].player_name[MAX_NAME_LEN - 1] = '\0';
    squad->slots[slot_idx].player_rating  = new_player->rating;
    squad->slots[slot_idx].is_filled      = true;

    printf("Replaced '%s' with '%s' (Rating: %d) in slot %d [%s].\n",
           existing_name, new_player->name, new_player->rating,
           slot_idx, squad->slots[slot_idx].position_type);
}

/* DISPLAY_SQUAD(squad : REFERENCE TO Squad) */
void displaySquad(Squad* squad) {
    printf("\n=== Current Squad ===\n");
    printf("%-5s %-5s %-25s %-7s\n", "Slot", "Pos", "Player", "Rating");
    printf("%-5s %-5s %-25s %-7s\n",
           "----", "-----", "-------------------------", "-------");

    int i;
    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            printf("%-5d %-5s %-25s %-7d\n",
                   i + 1,
                   squad->slots[i].position_type,
                   squad->slots[i].player_name,
                   squad->slots[i].player_rating);
        } else {
            printf("%-5d %-5s %-25s %-7s\n",
                   i + 1,
                   squad->slots[i].position_type,
                   "(empty)", "-");
        }
    }

    printf("\nAverage Squad Rating: %d\n\n", getSquadAverageRating(squad));
}

/* FIND_BEST_LINEUP(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory) */
void findBestLineup(Squad* squad, PlayerInventory* inv) {
    printf("\nBuilding best lineup from inventory...\n");
    autoSelectBestSquad(squad, inv);
}

/* GET_SQUAD_AVERAGE_RATING(squad : REFERENCE TO Squad) RETURNS INTEGER */
int getSquadAverageRating(Squad* squad) {
    int total  = 0;
    int filled = 0;
    int i;

    for (i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            total += squad->slots[i].player_rating;
            filled++;
        }
    }

    if (filled == 0) {
        return 0;
    }
    return total / filled;
}
