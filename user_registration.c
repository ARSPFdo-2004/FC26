#include "user_registration.h"

static const char* LEVEL_NAMES[] = {
    "Amateur 3", "Amateur 2", "Amateur 1",
    "Pro 3",     "Pro 2",     "Pro 1",
    "World Class 2", "World Class 1",
    "Legendary"
};

static const int WEEKLY_REWARDS[] = {
    10000, 40000, 70000,
    100000, 130000, 160000,
    190000, 220000, 250000
};

void initRegistry(UserRegistry* registry) {
    registry -> head = NULL;
    registry -> tail = NULL;
    registry -> user_count = 0;
}

int newIDProvider(UserRegistry* registry) {
    return registry -> user_count + 1;
}

UserNode* addUser(UserRegistry* registry, const char* name, const char* password) {
    
    UserNode* check = registry -> head;
    while (check != NULL) {
        if (strcmp(check -> name, name) == 0) {
            printf("User '%s' already exists.\n", name);
            return NULL;
        }
        check = check -> next;
    }

    UserNode* newNode = (UserNode *) malloc(sizeof(UserNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(newNode -> name, name, MAX_NAME_LEN - 1);
    newNode -> name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newNode -> password, password, MAX_NAME_LEN - 1);
    newNode -> password[MAX_NAME_LEN - 1] = '\0';
    newNode -> id           = newIDProvider(registry);
    newNode -> level        = 0;    
    newNode -> coins        = 15000; 
    newNode -> total_wins   = 0;
    newNode -> total_losses = 0;
    newNode -> total_draws  = 0;
    newNode -> week_wins    = 0;
    newNode -> week_matches = 0;
    newNode -> inventory    = NULL;
    newNode -> squad        = NULL;
    newNode -> match_history = NULL;
    newNode -> next         = NULL;
    newNode -> prev         = NULL;

    if (registry -> head == NULL) {
        registry -> head = newNode;
        registry -> tail = newNode;
    } else {
        newNode -> prev          = registry -> tail;
        registry -> tail->next   = newNode;
        registry -> tail         = newNode;
    }

    registry -> user_count++;
    return newNode;
}

void deleteAccount(UserRegistry* registry, UserNode* node) {
    if (node == NULL) {
        return;
    }

    if (node -> prev != NULL) {
        node -> prev->next = node -> next;
    } else {
        registry -> head = node -> next;
    }

    if (node -> next != NULL) {
        node -> next->prev = node -> prev;
    } else {
        registry -> tail = node -> prev;
    }

    registry -> user_count--;
    free(node);
}

void traverseUsers(UserRegistry* registry) {
    if (registry -> head == NULL) {
        printf("No users registered.\n");
        return;
    }

    printf("\n%-5s %-20s %-15s %-10s %-6s %-6s %-6s\n",
           "ID", "Name", "Level", "Coins", "Wins", "Loss", "Draw");
    printf("%-5s %-20s %-15s %-10s %-6s %-6s %-6s\n",
           "---", "--------------------", "---------------",
           "----------", "------", "------", "------");

    UserNode* current = registry -> head;
    while (current != NULL) {
        printf("%-5d %-20s %-15s %-10d %-6d %-6d %-6d\n",
               current -> id,
               current -> name,
               getLevelName(current -> level),
               current -> coins,
               current -> total_wins,
               current -> total_losses,
               current -> total_draws);
        current = current -> next;
    }
    printf("\n");
}

UserNode* findUserByID(UserRegistry* registry, int id) {
    UserNode* current = registry -> head;
    while (current != NULL) {
        if (current -> id == id) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

UserNode* findUserByName(UserRegistry* registry, const char* name) {
    UserNode* current = registry -> head;
    while (current != NULL) {
        if (strcmp(current -> name, name) == 0) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

const char* getLevelName(int level) {
    if (level < 0 || level > 8) {
        return "Unknown";
    }
    return LEVEL_NAMES[level];
}

void updateUserLevel(UserNode* user) {
    int wins = user -> total_wins;
    int new_level;

    if (wins >= 40) new_level = 8;
    else if (wins >= 35) new_level = 7;
    else if (wins >= 28) new_level = 6;
    else if (wins >= 21) new_level = 5;
    else if (wins >= 15) new_level = 4;
    else if (wins >= 10) new_level = 3;
    else if (wins >= 6)  new_level = 2;
    else if (wins >= 3)  new_level = 1;
    else                 new_level = 0;

    if (new_level != user -> level) {
        printf("  >> Level changed: %s -> %s\n",
               getLevelName(user -> level), getLevelName(new_level));
        user -> level = new_level;
    }
}

int getWeeklyReward(int level) {
    if (level < 0 || level > 8) {
        return 0;
    }
    return WEEKLY_REWARDS[level];
}
