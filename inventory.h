#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50
#define TYPE_LEN     4

typedef struct InventoryNode {
    char name[MAX_NAME_LEN];
    char type[TYPE_LEN];   
    int  rating;           
    int  price;            
    struct InventoryNode* next;
    struct InventoryNode* prev;
} InventoryNode;

typedef struct PlayerInventory {
    InventoryNode* head;
    InventoryNode* tail;
    int count;
} PlayerInventory;

void initInventory(PlayerInventory* inv);

InventoryNode* createInventoryNode(const char* name, const char* type,
                                   int rating, int price);

void addPlayer(PlayerInventory* inv, const char* name, const char* type,
               int rating, int price);

InventoryNode* removePlayerByName(PlayerInventory* inv, const char* name);

InventoryNode* searchInventoryByName(PlayerInventory* inv, const char* name);

void searchInventoryByType(PlayerInventory* inv, const char* type);

void searchInventoryByRating(PlayerInventory* inv, int min, int max);

void displayInventory(PlayerInventory* inv);

void giveInitialPlayers(PlayerInventory* inv, int user_id);

void freeInventory(PlayerInventory* inv);

#endif 
