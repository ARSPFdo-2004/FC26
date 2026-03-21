#ifndef USER_REGISTRATION_H
#define USER_REGISTRATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50

/* Forward declarations of types defined in other modules */
typedef struct PlayerInventory PlayerInventory;
typedef struct Squad Squad;
typedef struct MatchStack MatchStack;

/* Structure for a user node (doubly linked list) */
typedef struct UserNode {
    char name[MAX_NAME_LEN];
    int id;
    int level;        /* 0=Amateur3 ... 8=Legendary */
    int coins;
    int total_wins;
    int total_losses;
    int total_draws;
    int week_wins;
    int week_matches;
    PlayerInventory* inventory;
    Squad* squad;
    MatchStack* match_history;
    struct UserNode* next;
    struct UserNode* prev;
} UserNode;

/* Structure for the user registry (doubly linked list) */
typedef struct UserRegistry {
    UserNode* head;
    UserNode* tail;
    int user_count;
} UserRegistry;

/* INITIALIZE_REGISTRY(registry : REFERENCE TO UserRegistry) */
void initRegistry(UserRegistry* registry);

/* NEW_ID_PROVIDER(registry : REFERENCE TO UserRegistry) RETURNS INTEGER */
int newIDProvider(UserRegistry* registry);

/* ADD_USER(registry : REFERENCE TO UserRegistry, name : STRING)
   RETURNS POINTER TO UserNode */
UserNode* addUser(UserRegistry* registry, const char* name);

/* DELETE_ACCOUNT(registry : REFERENCE TO UserRegistry, node : POINTER TO UserNode) */
void deleteAccount(UserRegistry* registry, UserNode* node);

/* TRAVERSE_USERS(registry : REFERENCE TO UserRegistry) */
void traverseUsers(UserRegistry* registry);

/* FIND_USER_BY_ID(registry : REFERENCE TO UserRegistry, id : INTEGER)
   RETURNS POINTER TO UserNode */
UserNode* findUserByID(UserRegistry* registry, int id);

/* FIND_USER_BY_NAME(registry : REFERENCE TO UserRegistry, name : STRING)
   RETURNS POINTER TO UserNode */
UserNode* findUserByName(UserRegistry* registry, const char* name);

/* GET_LEVEL_NAME(level : INTEGER) RETURNS STRING */
const char* getLevelName(int level);

/* UPDATE_USER_LEVEL(user : POINTER TO UserNode) */
void updateUserLevel(UserNode* user);

/* GET_WEEKLY_REWARD(level : INTEGER) RETURNS INTEGER */
int getWeeklyReward(int level);

#endif /* USER_REGISTRATION_H */
