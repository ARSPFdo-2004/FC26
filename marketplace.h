#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50
#define TYPE_LEN     4

/* Structure for a marketplace node (circular doubly linked list) */
typedef struct MarketNode {
    char name[MAX_NAME_LEN];
    char type[TYPE_LEN];   /* "FW", "MF", "DF", "GK" */
    int  rating;
    int  price;            /* Set by the selling user */
    int  seller_id;
    struct MarketNode* next;
    struct MarketNode* prev;
} MarketNode;

/* Structure for the marketplace (circular doubly linked list) */
typedef struct Marketplace {
    MarketNode* head;
    int count;
} Marketplace;

/* INIT_MARKETPLACE(market : REFERENCE TO Marketplace) */
void initMarketplace(Marketplace* market);

/* ADD_TO_MARKETPLACE(market : REFERENCE TO Marketplace,
                      name, type : STRING, rating, price, seller_id : INTEGER) */
void addToMarketplace(Marketplace* market, const char* name, const char* type,
                      int rating, int price, int seller_id);

/* REMOVE_FROM_MARKETPLACE(market : REFERENCE TO Marketplace, name : STRING)
   RETURNS POINTER TO MarketNode (caller must free) */
MarketNode* removeFromMarketplace(Marketplace* market, const char* name);

/* SEARCH_MARKET_BY_NAME(market : REFERENCE TO Marketplace, name : STRING)
   RETURNS POINTER TO MarketNode */
MarketNode* searchMarketByName(Marketplace* market, const char* name);

/* SEARCH_MARKET_BY_TYPE(market : REFERENCE TO Marketplace, type : STRING) */
void searchMarketByType(Marketplace* market, const char* type);

/* DISPLAY_MARKETPLACE(market : REFERENCE TO Marketplace) */
void displayMarketplace(Marketplace* market);

/* FREE_MARKETPLACE(market : REFERENCE TO Marketplace) */
void freeMarketplace(Marketplace* market);

/* SEED_MARKETPLACE(market : REFERENCE TO Marketplace)
   Pre-populates the marketplace with well-known players (seller_id = 0). */
void seedMarketplace(Marketplace* market);

#endif /* MARKETPLACE_H */
