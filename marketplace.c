#include "marketplace.h"

/* INIT_MARKETPLACE(market : REFERENCE TO Marketplace) */
void initMarketplace(Marketplace* market) {
    market->head  = NULL;
    market->count = 0;
}

/* ADD_TO_MARKETPLACE(market : REFERENCE TO Marketplace,
                      name, type : STRING, rating, price, seller_id : INTEGER) */
void addToMarketplace(Marketplace* market, const char* name, const char* type,
                      int rating, int price, int seller_id) {
    MarketNode* node = (MarketNode*)malloc(sizeof(MarketNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(node->type, type, TYPE_LEN - 1);
    node->type[TYPE_LEN - 1] = '\0';
    node->rating    = rating;
    node->price     = price;
    node->seller_id = seller_id;
    node->next      = NULL;
    node->prev      = NULL;

    if (market->head == NULL) {
        /* Single node: points to itself (circular) */
        market->head = node;
        node->next   = node;
        node->prev   = node;
    } else {
        /* Insert before head (at tail position) */
        MarketNode* tail = market->head->prev;
        tail->next       = node;
        node->prev       = tail;
        node->next       = market->head;
        market->head->prev = node;
    }

    market->count++;
}

/* REMOVE_FROM_MARKETPLACE(market : REFERENCE TO Marketplace, name : STRING)
   RETURNS POINTER TO MarketNode (caller must free) */
MarketNode* removeFromMarketplace(Marketplace* market, const char* name) {
    if (market->head == NULL) {
        return NULL;
    }

    MarketNode* current = market->head;
    int i;

    for (i = 0; i < market->count; i++) {
        if (strcmp(current->name, name) == 0) {
            if (market->count == 1) {
                /* Only node */
                market->head = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                if (current == market->head) {
                    market->head = current->next;
                }
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

/* SEARCH_MARKET_BY_NAME(market : REFERENCE TO Marketplace, name : STRING)
   RETURNS POINTER TO MarketNode */
MarketNode* searchMarketByName(Marketplace* market, const char* name) {
    if (market->head == NULL) {
        return NULL;
    }

    MarketNode* current = market->head;
    int i;

    for (i = 0; i < market->count; i++) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* SEARCH_MARKET_BY_TYPE(market : REFERENCE TO Marketplace, type : STRING) */
void searchMarketByType(Marketplace* market, const char* type) {
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return;
    }

    bool found = false;
    MarketNode* current = market->head;
    int i;

    printf("\n%-25s %-5s %-7s %-8s %-10s\n",
           "Name", "Type", "Rating", "Price", "Seller ID");
    printf("%-25s %-5s %-7s %-8s %-10s\n",
           "-------------------------", "-----", "-------",
           "--------", "----------");

    for (i = 0; i < market->count; i++) {
        if (strcmp(current->type, type) == 0) {
            printf("%-25s %-5s %-7d %-8d %-10d\n",
                   current->name, current->type,
                   current->rating, current->price, current->seller_id);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No players of type '%s' found in marketplace.\n", type);
    }
    printf("\n");
}

/* DISPLAY_MARKETPLACE(market : REFERENCE TO Marketplace) */
void displayMarketplace(Marketplace* market) {
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return;
    }

    printf("\n%-3s %-25s %-5s %-7s %-8s %-10s\n",
           "#", "Name", "Type", "Rating", "Price", "Seller ID");
    printf("%-3s %-25s %-5s %-7s %-8s %-10s\n",
           "---", "-------------------------", "-----",
           "-------", "--------", "----------");

    MarketNode* current = market->head;
    int i;
    for (i = 0; i < market->count; i++) {
        printf("%-3d %-25s %-5s %-7d %-8d %-10d\n",
               i + 1, current->name, current->type,
               current->rating, current->price, current->seller_id);
        current = current->next;
    }
    printf("\nTotal listings: %d\n\n", market->count);
}

/* SEED_MARKETPLACE(market : REFERENCE TO Marketplace)
   Pre-populates the marketplace with well-known players (seller_id = 0). */
void seedMarketplace(Marketplace* market) {
    /* seller_id = 0 means system listing (no real user has id 0) */
    addToMarketplace(market, "Ronaldo",       "FW", 94, 5000, 0);
    addToMarketplace(market, "Messi",         "FW", 95, 5500, 0);
    addToMarketplace(market, "Mbappe",        "FW", 93, 4800, 0);
    addToMarketplace(market, "Marcelo",       "DF", 82, 2000, 0);
    addToMarketplace(market, "Suarez",        "MF", 88, 3500, 0);
    addToMarketplace(market, "Bellingham",    "MF", 91, 4000, 0);
    addToMarketplace(market, "Vini Jr",       "FW", 92, 4500, 0);
    addToMarketplace(market, "Valverde",      "MF", 88, 3500, 0);
    addToMarketplace(market, "Rodrygo",       "FW", 87, 3000, 0);
    addToMarketplace(market, "Camavinga",     "MF", 85, 2500, 0);
    addToMarketplace(market, "Zlatan",        "FW", 90, 4000, 0);
    addToMarketplace(market, "Pedri",         "MF", 89, 3500, 0);
    addToMarketplace(market, "Neymar Jr",     "FW", 91, 4200, 0);
    addToMarketplace(market, "Sergio Ramos",  "DF", 88, 3200, 0);
    addToMarketplace(market, "Nicole",        "FW", 80, 1500, 0);
    addToMarketplace(market, "Pele",          "MF", 98, 8000, 0);
    addToMarketplace(market, "David Beckham", "FW", 89, 3500, 0);
    addToMarketplace(market, "Lewandowski",   "FW", 92, 4500, 0);
    addToMarketplace(market, "Dani Olmo",     "MF", 87, 3000, 0);
    addToMarketplace(market, "Lamine Yamal",  "FW", 89, 3500, 0);
    addToMarketplace(market, "Raphinha",      "MF", 87, 3000, 0);
}

/* FREE_MARKETPLACE(market : REFERENCE TO Marketplace) */
void freeMarketplace(Marketplace* market) {
    if (market->head == NULL) {
        return;
    }

    /* Break circular link */
    market->head->prev->next = NULL;

    MarketNode* current = market->head;
    while (current != NULL) {
        MarketNode* temp = current;
        current = current->next;
        free(temp);
    }
    market->head  = NULL;
    market->count = 0;
}
