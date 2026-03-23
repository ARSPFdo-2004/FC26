#include "marketplace.h"
#include "user_registration.h"

void initMarketplace(Marketplace* market) {
    market->head = NULL;
    market->count = 0;
}

void addToMarketplace(Marketplace* market, const char* name, const char* type, int rating, int price) {
    MarketNode* node = (MarketNode *) malloc(sizeof(MarketNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(node->type, type, TYPE_LEN - 1);
    node->type[TYPE_LEN - 1] = '\0';
    node->rating = rating;
    node->price = price;

    if (market->head == NULL) {
        market->head = node;
        node->next = node;
        node->prev = node;
    } else {
        MarketNode* current = market->head;
        bool inserted = false;
        do {
            if (node->rating > current->rating) {
                node->next = current;
                node->prev = current->prev;
                current->prev->next = node;
                current->prev = node;
                if (current == market->head) {
                    market->head = node;
                }
                inserted = true;
                break;
            }
            current = current->next;
        } while (current != market->head);

        if (!inserted) {
            MarketNode* tail = market->head->prev;
            tail->next = node;
            node->prev = tail;
            node->next = market->head;
            market->head->prev = node;
        }
    }
    market->count++;
}

MarketNode* removeFromMarketplace(Marketplace* market, const char* name) {
    if (market->head == NULL) return NULL;
    MarketNode* current = market->head;
    for (int i = 0; i < market->count; i++) {
        if (strcasecmp(current->name, name) == 0) {
            if (market->count == 1) {
                market->head = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                if (current == market->head) market->head = current->next;
            }
            current->next = NULL;
            current->prev = NULL;
            market->count--;
            return current;
        }
        current = current->next;
    }
    return NULL;
}

MarketNode* searchMarketByName(Marketplace* market, const char* name) {
    if (market->head == NULL) return NULL;
    MarketNode* current = market->head;
    for (int i = 0; i < market->count; i++) {
        if (isPartialMatch(current->name, name)) return current;
        current = current->next;
    }
    return NULL;
}

void displayMarketplace(Marketplace* market) {
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return;
    }
    printf("\n%-3s %-25s %-5s %-7s %-8s\n", "#", "Name", "Type", "Rating", "Price");
    printf("%-3s %-25s %-5s %-7s %-8s\n", "---", "-------------------------", "-----", "-------", "--------");
    MarketNode* current = market->head;
    for (int i = 0; i < market->count; i++) {
        printf("%-3d %-25s %-5s %-7d %-8d\n", i + 1, current->name, current->type, current->rating, current->price);
        current = current->next;
    }
    printf("\nTotal listings: %d\n\n", market->count);
}

void seedMarketplace(Marketplace* market) {
    const char* fw_names[] = {"Lionel Messi", "Cristiano Ronaldo", "Kylian Mbappe", "Erling Haaland", "Neymar Jr", "Mohamed Salah", "Harry Kane", "Vinicius Junior", "Lautaro Martinez", "Marcus Rashford"};
    const char* mf_names[] = {"Kevin De Bruyne", "Luka Modric", "Toni Kroos", "Bruno Fernandes", "Pedri", "Jude Bellingham", "Bernardo Silva", "Declan Rice", "Frenkie De Jong", "Rodri"};
    const char* df_names[] = {"Joao Cancelo", "Kyle Walker", "Trent Alexander Arnold", "Virgil Van Dijk", "Ruben Dias", "Antonio Rudiger", "Kalidou Koulibaly", "Eder Militao", "Marquinhos", "Achraf Hakimi", "Alphonso Davies", "Andrew Robertson", "Reece James", "Dani Carvajal", "Milan Skriniar", "Mats Hummels", "Aymeric Laporte", "Jules Kounde", "Alessandro Bastoni", "Pau Torres"};
    const char* gk_names[] = {"Emiliano Martinez", "Thibaut Courtois", "Ederson Moraes", "Alisson Becker", "Mike Maignan", "Gianluigi Donnarumma", "Andre Onana", "Keylor Navas", "Wojciech Szczesny"};
    for (int i = 0; i < 10; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, fw_names[i], "FW", r, p);
        r = 60 + rand() % 51;
        p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, fw_names[i], "FW", r, p);
    }
    for (int i = 0; i < 10; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, mf_names[i], "MF", r, p);
    }
    for (int i = 0; i < 20; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, df_names[i], "DF", r, p);
    }
    for (int i = 0; i < 9; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, gk_names[i], "GK", r, p);
    }
}

void freeMarketplace(Marketplace* market) {
    if (market->head == NULL) return;
    market->head->prev->next = NULL;
    MarketNode* current = market->head;
    while (current != NULL) {
        MarketNode* temp = current;
        current = current->next;
        free(temp);
    }
    market->head = NULL;
    market->count = 0;
}

bool searchMarketAdvanced(Marketplace* market, const char* name, const char* type, int min_rating, int max_rating) {
    if (market->head == NULL) return false;
    bool found = false;
    MarketNode* current = market->head;
    
    // Check if any match before printing header
    bool any_match = false;
    MarketNode* t = current;
    for (int i = 0; i < market->count; i++) {
        bool match = true;
        if (name && strlen(name) > 0 && !isPartialMatch(t->name, name)) match = false;
        if (type && strlen(type) > 0 && strcasecmp(t->type, type) != 0) match = false;
        if (t->rating < min_rating || t->rating > max_rating) match = false;
        if (match) { any_match = true; break; }
        t = t->next;
    }
    
    if (!any_match) return false;

    printf("\n%-25s %-5s %-7s %-8s\n", "Name", "Type", "Rating", "Price");
    printf("%-25s %-5s %-7s %-8s\n", "-------------------------", "-----", "-------", "--------");

    for (int i = 0; i < market->count; i++) {
        bool match = true;
        if (name && strlen(name) > 0 && !isPartialMatch(current->name, name)) match = false;
        if (type && strlen(type) > 0 && strcasecmp(current->type, type) != 0) match = false;
        if (current->rating < min_rating || current->rating > max_rating) match = false;
        
        if (match) {
            printf("%-25s %-5s %-7d %-8d\n", current->name, current->type, current->rating, current->price);
            found = true;
        }
        current = current->next;
    }
    printf("\n");
    return found;
}
