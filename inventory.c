#include "inventory.h"
#include "user_registration.h"

void initInventory(PlayerInventory* inv) {
    inv -> head  = NULL;
    inv -> tail  = NULL;
    inv -> count = 0;
}

InventoryNode* createInventoryNode(const char* name, const char* type,
                                   int rating, int price) {
    InventoryNode* node = (InventoryNode *) malloc(sizeof(InventoryNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strncpy(node -> name, name, MAX_NAME_LEN - 1);
    node -> name[MAX_NAME_LEN - 1] = '\0';
    strncpy(node -> type, type, TYPE_LEN - 1);
    node -> type[TYPE_LEN - 1] = '\0';
    node -> rating = rating;
    node -> price  = price;
    node -> next   = NULL;
    node -> prev   = NULL;
    return node;
}

void addPlayer(PlayerInventory* inv, const char* name, const char* type,
               int rating, int price) {
    InventoryNode* node = createInventoryNode(name, type, rating, price);

    if (inv -> head == NULL) {
        inv -> head = node;
        inv -> tail = node;
    } else {
        node -> prev       = inv -> tail;
        inv -> tail->next  = node;
        inv -> tail        = node;
    }
    inv -> count++;
}

InventoryNode* removePlayerByName(PlayerInventory* inv, const char* name) {
    InventoryNode* current = inv -> head;

    while (current != NULL) {
        if (strcasecmp(current -> name, name) == 0) {
            if (current -> prev != NULL) {
                current -> prev->next = current -> next;
            } else {
                inv -> head = current -> next;
            }

            if (current -> next != NULL) {
                current -> next->prev = current -> prev;
            } else {
                inv -> tail = current -> prev;
            }

            current -> next = NULL;
            current -> prev = NULL;
            inv -> count--;
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

InventoryNode* searchInventoryByName(PlayerInventory* inv, const char* name) {
    InventoryNode* current = inv -> head;
    while (current != NULL) {
        if (isPartialMatch(current -> name, name)) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

void searchInventoryByType(PlayerInventory* inv, const char* type) {
    bool found = false;
    InventoryNode* current = inv -> head;

    printf("\n%-25s %-5s %-7s\n", "Name", "Type", "Rating");
    printf("%-25s %-5s %-7s\n",
           "-------------------------", "-----", "-------");

    while (current != NULL) {
        if (strcasecmp(current -> type, type) == 0) {
            printf("%-25s %-5s %-7d\n",
                   current -> name, current -> type,
                   current -> rating);
            found = true;
        }
        current = current -> next;
    }

    if (!found) {
        printf("No players found with type '%s'.\n", type);
    }
    printf("\n");
}

void searchInventoryByRating(PlayerInventory* inv, int min, int max) {
    bool found = false;
    InventoryNode* current = inv -> head;

    printf("\n%-25s %-5s %-7s\n", "Name", "Type", "Rating");
    printf("%-25s %-5s %-7s\n",
           "-------------------------", "-----", "-------");

    while (current != NULL) {
        if (current -> rating >= min && current -> rating <= max) {
            printf("%-25s %-5s %-7d\n",
                   current -> name, current -> type,
                   current -> rating);
            found = true;
        }
        current = current -> next;
    }

    if (!found) {
        printf("No players found with rating between %d and %d.\n", min, max);
    }
    printf("\n");
}

void displayInventory(PlayerInventory* inv) {
    if (inv -> head == NULL) {
        printf("Inventory is empty.\n");
        return;
    }

    printf("\n%-3s %-25s %-5s %-7s\n",
           "#", "Name", "Type", "Rating");
    printf("%-3s %-25s %-5s %-7s\n",
           "---", "-------------------------", "-----", "-------");

    int i = 1;
    InventoryNode* current = inv -> head;
    while (current != NULL) {
        printf("%-3d %-25s %-5s %-7d\n",
               i, current -> name, current -> type,
               current -> rating);
        current = current -> next;
        i++;
    }
    printf("\nTotal players: %d\n\n", inv -> count);
}

void giveInitialPlayers(PlayerInventory* inv, int user_id) {
    const char* fw_names[] = {"Lionel Messi", "Cristiano Ronaldo", "Kylian Mbappe", "Erling Haaland", "Neymar Jr", "Mohamed Salah", "Harry Kane", "Vinicius Junior", "Lautaro Martinez", "Marcus Rashford"};
    const char* mf_names[] = {"Kevin De Bruyne", "Luka Modric", "Toni Kroos", "Bruno Fernandes", "Pedri", "Jude Bellingham", "Bernardo Silva", "Declan Rice", "Frenkie De Jong", "Rodri"};
    const char* df_names[] = {"Joao Cancelo", "Kyle Walker", "Trent Alexander Arnold", "Virgil Van Dijk", "Ruben Dias", "Antonio Rudiger", "Kalidou Koulibaly", "Eder Militao", "Marquinhos", "Achraf Hakimi", "Alphonso Davies", "Andrew Robertson", "Reece James", "Dani Carvajal", "Milan Skriniar", "Mats Hummels", "Aymeric Laporte", "Jules Kounde", "Alessandro Bastoni", "Pau Torres"};
    const char* gk_names[] = {"Emiliano Martinez", "Thibaut Courtois", "Ederson Moraes", "Alisson Becker", "Mike Maignan", "Gianluigi Donnarumma", "Andre Onana", "Keylor Navas", "Wojciech Szczesny"};

    int fw_count = 10, mf_count = 10, df_count = 20, gk_count = 9;

    bool fw_picked[10] = {false};
    bool mf_picked[10] = {false};
    bool df_picked[20] = {false};
    bool gk_picked[9]  = {false};

    for (int i = 0; i < 3; i++) {
        int idx;
        do { idx = rand() % fw_count; } while (fw_picked[idx]);
        fw_picked[idx] = true;
        addPlayer(inv, fw_names[idx], "FW", 60 + rand() % 10, 500);
    }

    for (int i = 0; i < 3; i++) {
        int idx;
        do { idx = rand() % mf_count; } while (mf_picked[idx]);
        mf_picked[idx] = true;
        addPlayer(inv, mf_names[idx], "MF", 60 + rand() % 10, 500);
    }

    for (int i = 0; i < 4; i++) {
        int idx;
        do { idx = rand() % df_count; } while (df_picked[idx]);
        df_picked[idx] = true;
        addPlayer(inv, df_names[idx], "DF", 60 + rand() % 10, 500);
    }

    int idx = rand() % gk_count;
    gk_picked[idx] = true;
    addPlayer(inv, gk_names[idx], "GK", 60 + rand() % 10, 500);
}

void freeInventory(PlayerInventory* inv) {
    InventoryNode* current = inv -> head;
    while (current != NULL) {
        InventoryNode* temp = current;
        current = current -> next;
        free(temp);
    }
    inv -> head  = NULL;
    inv -> tail  = NULL;
    inv -> count = 0;
}
