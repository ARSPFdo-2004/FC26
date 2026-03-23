#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50
#define TYPE_LEN     4

typedef struct MarketNode {
    char name[MAX_NAME_LEN];
    char type[TYPE_LEN];   
    int  rating;
    int  price;            
    struct MarketNode* next;
    struct MarketNode* prev;
} MarketNode;

typedef struct Marketplace {
    MarketNode* head;
    int count;
} Marketplace;

void initMarketplace(Marketplace* market);

void addToMarketplace(Marketplace* market, const char* name, const char* type,

MarketNode* removeFromMarketplace(Marketplace* market, const char* name);

MarketNode* searchMarketByName(Marketplace* market, const char* name);

void searchMarketByType(Marketplace* market, const char* type);

void displayMarketplace(Marketplace* market);

void freeMarketplace(Marketplace* market);

void seedMarketplace(Marketplace* market);

bool searchMarketBasicOptions(Marketplace* market, int min_rating, int max_rating);
#endif

