#ifndef USER_REGISTRATION_H
#define USER_REGISTRATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50

typedef struct PlayerInventory PlayerInventory;
typedef struct Squad Squad;
typedef struct MatchStack MatchStack;

typedef struct UserNode {
    char name[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    int id;
    int level;        
    int coins;
    int total_wins;
    int total_losses;
    int total_draws;
    int week_wins;
    int week_matches;
    int past_week_levels[5];
    int past_week_ranks[5];
    int past_week_rewards[5];
    int past_weeks[5];
    int reward_history_count;
    PlayerInventory* inventory;
    Squad* squad;
    MatchStack* match_history;
    struct UserNode* next;
    struct UserNode* prev;
} UserNode;

typedef struct UserRegistry {
    UserNode* head;
    UserNode* tail;
    int user_count;
} UserRegistry;

void initRegistry(UserRegistry* registry);

int newIDProvider(UserRegistry* registry);

UserNode* addUser(UserRegistry* registry, const char* name, const char* password);

void deleteAccount(UserRegistry* registry, UserNode* node);

void traverseUsers(UserRegistry* registry);

UserNode* findUserByID(UserRegistry* registry, int id);

UserNode* findUserByName(UserRegistry* registry, const char* name);

const char* getLevelName(int level);

void updateUserLevel(UserNode* user, int rank);

int getWeeklyReward(int level);

bool isPartialMatch(const char* str, const char* substr);

#endif 
