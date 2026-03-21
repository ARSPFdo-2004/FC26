#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50
#define TYPE_LEN     4

/* Structure for a player card node (doubly linked list) */
typedef struct InventoryNode {
    char name[MAX_NAME_LEN];
    char type[TYPE_LEN];   /* "FW", "MF", "DF", "GK" */
    int  rating;           /* 60-99 */
    int  price;            /* coin value */
    struct InventoryNode* next;
    struct InventoryNode* prev;
} InventoryNode;

/* Structure for a player inventory (doubly linked list) */
typedef struct PlayerInventory {
    InventoryNode* head;
    InventoryNode* tail;
    int count;
} PlayerInventory;

/* INIT_INVENTORY(inv : REFERENCE TO PlayerInventory) */
void initInventory(PlayerInventory* inv);

/* CREATE_INVENTORY_NODE(name, type, rating, price) RETURNS POINTER TO InventoryNode */
InventoryNode* createInventoryNode(const char* name, const char* type,
                                   int rating, int price);

/* ADD_PLAYER(inv : REFERENCE TO PlayerInventory, name, type, rating, price) */
void addPlayer(PlayerInventory* inv, const char* name, const char* type,
               int rating, int price);

/* REMOVE_PLAYER_BY_NAME(inv : REFERENCE TO PlayerInventory, name : STRING)
   RETURNS POINTER TO InventoryNode (removed node, caller must free) */
InventoryNode* removePlayerByName(PlayerInventory* inv, const char* name);

/* SEARCH_BY_NAME(inv : REFERENCE TO PlayerInventory, name : STRING)
   RETURNS POINTER TO InventoryNode */
InventoryNode* searchInventoryByName(PlayerInventory* inv, const char* name);

/* SEARCH_BY_TYPE(inv : REFERENCE TO PlayerInventory, type : STRING) */
void searchInventoryByType(PlayerInventory* inv, const char* type);

/* SEARCH_BY_RATING(inv : REFERENCE TO PlayerInventory, min : INTEGER, max : INTEGER) */
void searchInventoryByRating(PlayerInventory* inv, int min, int max);

/* DISPLAY_INVENTORY(inv : REFERENCE TO PlayerInventory) */
void displayInventory(PlayerInventory* inv);

/* GIVE_INITIAL_PLAYERS(inv : REFERENCE TO PlayerInventory, user_id : INTEGER) */
void giveInitialPlayers(PlayerInventory* inv, int user_id);

/* FREE_INVENTORY(inv : REFERENCE TO PlayerInventory) */
void freeInventory(PlayerInventory* inv);

#endif /* INVENTORY_H */
