#include "inventory.h"

void initInventory(PlayerInventory* inv) {
    inv->head  = NULL;
    inv->tail  = NULL;
    inv->count = 0;
}

InventoryNode* createInventoryNode(const char* name, const char* type,
                                   int rating, int price) {
    InventoryNode* node = (InventoryNode*)malloc(sizeof(InventoryNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(node->type, type, TYPE_LEN - 1);
    node->type[TYPE_LEN - 1] = '\0';
    node->rating = rating;
    node->price  = price;
    node->next   = NULL;
    node->prev   = NULL;
    return node;
}

void addPlayer(PlayerInventory* inv, const char* name, const char* type,
               int rating, int price) {
    InventoryNode* node = createInventoryNode(name, type, rating, price);

    if (inv->head == NULL) {
        inv->head = node;
        inv->tail = node;
    } else {
        node->prev       = inv->tail;
        inv->tail->next  = node;
        inv->tail        = node;
    }
    inv->count++;
}

InventoryNode* removePlayerByName(PlayerInventory* inv, const char* name) {
    InventoryNode* current = inv->head;

    while (current != NULL) {
        if (strcasecmp(current->name, name) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                inv->head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                inv->tail = current->prev;
            }

            current->next = NULL;
            current->prev = NULL;
            inv->count--;
            return current;
        }
        current = current->next;
    }
    return NULL;
}

InventoryNode* searchInventoryByName(PlayerInventory* inv, const char* name) {
    InventoryNode* current = inv->head;
    while (current != NULL) {
        if (strcasecmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchInventoryByType(PlayerInventory* inv, const char* type) {
    bool found = false;
    InventoryNode* current = inv->head;

    printf("\n%-25s %-5s %-7s %-8s\n", "Name", "Type", "Rating", "Price");
    printf("%-25s %-5s %-7s %-8s\n",
           "-------------------------", "-----", "-------", "--------");

    while (current != NULL) {
        if (strcasecmp(current->type, type) == 0) {
            printf("%-25s %-5s %-7d %-8d\n",
                   current->name, current->type,
                   current->rating, current->price);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No players found with type '%s'.\n", type);
    }
    printf("\n");
}

void searchInventoryByRating(PlayerInventory* inv, int min, int max) {
    bool found = false;
    InventoryNode* current = inv->head;

    printf("\n%-25s %-5s %-7s %-8s\n", "Name", "Type", "Rating", "Price");
    printf("%-25s %-5s %-7s %-8s\n",
           "-------------------------", "-----", "-------", "--------");

    while (current != NULL) {
        if (current->rating >= min && current->rating <= max) {
            printf("%-25s %-5s %-7d %-8d\n",
                   current->name, current->type,
                   current->rating, current->price);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No players found with rating between %d and %d.\n", min, max);
    }
    printf("\n");
}

void displayInventory(PlayerInventory* inv) {
    if (inv->head == NULL) {
        printf("Inventory is empty.\n");
        return;
    }

    printf("\n%-3s %-25s %-5s %-7s %-8s\n",
           "#", "Name", "Type", "Rating", "Price");
    printf("%-3s %-25s %-5s %-7s %-8s\n",
           "---", "-------------------------", "-----", "-------", "--------");

    int i = 1;
    InventoryNode* current = inv->head;
    while (current != NULL) {
        printf("%-3d %-25s %-5s %-7d %-8d\n",
               i, current->name, current->type,
               current->rating, current->price);
        current = current->next;
        i++;
    }
    printf("\nTotal players: %d\n\n", inv->count);
}

void giveInitialPlayers(PlayerInventory* inv, int user_id) {
    
    char name[MAX_NAME_LEN];
    int ratings[11] = {62, 65, 61, 67, 63, 60, 68, 64, 66, 61, 63};

    snprintf(name, sizeof(name), "FW1_U%d", user_id);
    addPlayer(inv, name, "FW", ratings[0], 500);

    snprintf(name, sizeof(name), "FW2_U%d", user_id);
    addPlayer(inv, name, "FW", ratings[1], 500);

    snprintf(name, sizeof(name), "FW3_U%d", user_id);
    addPlayer(inv, name, "FW", ratings[2], 500);

    snprintf(name, sizeof(name), "MF1_U%d", user_id);
    addPlayer(inv, name, "MF", ratings[3], 500);

    snprintf(name, sizeof(name), "MF2_U%d", user_id);
    addPlayer(inv, name, "MF", ratings[4], 500);

    snprintf(name, sizeof(name), "MF3_U%d", user_id);
    addPlayer(inv, name, "MF", ratings[5], 500);

    snprintf(name, sizeof(name), "DF1_U%d", user_id);
    addPlayer(inv, name, "DF", ratings[6], 500);

    snprintf(name, sizeof(name), "DF2_U%d", user_id);
    addPlayer(inv, name, "DF", ratings[7], 500);

    snprintf(name, sizeof(name), "DF3_U%d", user_id);
    addPlayer(inv, name, "DF", ratings[8], 500);

    snprintf(name, sizeof(name), "DF4_U%d", user_id);
    addPlayer(inv, name, "DF", ratings[9], 500);

    snprintf(name, sizeof(name), "GK1_U%d", user_id);
    addPlayer(inv, name, "GK", ratings[10], 500);
}

void freeInventory(PlayerInventory* inv) {
    InventoryNode* current = inv->head;
    while (current != NULL) {
        InventoryNode* temp = current;
        current = current->next;
        free(temp);
    }
    inv->head  = NULL;
    inv->tail  = NULL;
    inv->count = 0;
}
