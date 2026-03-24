#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

// ==================== inventory.h ====================
#ifndef INVENTORY_H
#define INVENTORY_H


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

void seePlayers(PlayerInventory* inv);

#endif 

// ==================== squad.h ====================
#ifndef SQUAD_H
#define SQUAD_H


#define SQUAD_SIZE 11
#define MAX_NAME_LEN 50

typedef struct SquadSlot {
    char position_type[4];
    char player_name[MAX_NAME_LEN];
    char native_type[4];
    int  player_rating;
    bool is_filled;
} SquadSlot;

typedef struct Squad {
    SquadSlot slots[SQUAD_SIZE];
} Squad;

void initSquad(Squad* squad);

void autoSelectBestSquad(Squad* squad, PlayerInventory* inv);

void displaySquad(Squad* squad);

int getSquadAverageRating(Squad* squad);

bool isPlayerInSquad(Squad* squad, const char* name);

void swapPlayerCards(Squad* squad, PlayerInventory* inv, const char* name1, const char* name2);

void displayAllUserCards(Squad* squad, PlayerInventory* inv);

#endif

// ==================== matchhistory.h ====================
#ifndef MATCHHISTORY_H
#define MATCHHISTORY_H


#define MAX_NAME_LEN  50
#define RESULT_WIN    'W'
#define RESULT_LOSS   'L'
#define RESULT_DRAW   'D'

typedef struct MatchRecord {
    char opponent_name[MAX_NAME_LEN];
    char result;        
    int  coins_earned;
    int  week;
    int  match_number;
    int  my_goals;
    int  opp_goals;
    int  my_shots;
    int  opp_shots;
    int  my_assists;
    int  opp_assists;
    int  my_fouls;
    int  opp_fouls;
    int  my_offsides;
    int  opp_offsides;
    int  my_red_cards;
    int  opp_red_cards;
    int  my_yellow_cards;
    int  opp_yellow_cards;
    struct MatchRecord* next;
} MatchRecord;

typedef struct MatchStack {
    MatchRecord* top;
    int total_matches;
    int total_wins;
    int total_losses;
    int total_draws;
    int week_wins;
    int week_matches;
    int current_week;
} MatchStack;

void initMatchStack(MatchStack* stack);

bool isStackEmpty(MatchStack* stack);

void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week,
               int my_goals, int opp_goals,
               int my_shots, int opp_shots,
               int my_assists, int opp_assists,
               int my_fouls, int opp_fouls,
               int my_offsides, int opp_offsides,
               int my_red_cards, int opp_red_cards,
               int my_yellow_cards, int opp_yellow_cards);
void displayMatchHistory(MatchStack* stack);

void displayStatistics(MatchStack* stack, int week, int match_num, const char* user_name);

float getWinPercentage(MatchStack* stack);

float getWeeklyWinPercentage(MatchStack* stack);

void resetWeeklyStats(MatchStack* stack);

void freeMatchStack(MatchStack* stack);

#endif 

// ==================== user_registration.h ====================
#ifndef USER_REGISTRATION_H
#define USER_REGISTRATION_H


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

// ==================== marketplace.h ====================
#ifndef MARKETPLACE_H
#define MARKETPLACE_H


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

void addToMarketplace(Marketplace* market, const char* name, const char* type, int rating, int price);

MarketNode* removeFromMarketplace(Marketplace* market, const char* name);

MarketNode* searchMarketByName(Marketplace* market, const char* name);

void displayMarketplace(Marketplace* market);

void freeMarketplace(Marketplace* market);

void seedMarketplace(Marketplace* market);

bool searchMarketAdvanced(Marketplace* market, const char* name, const char* type, int min_rating, int max_rating);

#endif

// ==================== matchmaking.h ====================
#ifndef MATCHMAKING_H
#define MATCHMAKING_H


typedef struct MatchQueueNode {
    int user_id;
    int user_level;
    struct MatchQueueNode* next;
} MatchQueueNode;

typedef struct MatchQueue {
    MatchQueueNode* front;
    MatchQueueNode* rear;
    int size;
} MatchQueue;

void initMatchQueue(MatchQueue* queue);

bool isQueueEmpty(MatchQueue* queue);

void enqueueUser(MatchQueue* queue, int user_id, int user_level);

int dequeueUser(MatchQueue* queue);

int peekQueue(MatchQueue* queue);

bool isUserInQueue(MatchQueue* queue, int user_id);


void displayMatchQueue(MatchQueue* queue, UserRegistry* registry);

int findSameLevelOpponent(MatchQueue* queue, int user_id, int user_level);

void removeFromQueue(MatchQueue* queue, int user_id);

#endif 

// ==================== leaderboard.h ====================
#ifndef LEADERBOARD_H
#define LEADERBOARD_H


#define MAX_NAME_LEN 50

typedef struct LBNode {
    int  user_id;
    char user_name[MAX_NAME_LEN];
    int  wins;
    int  losses;
    int  draws;
    float win_percentage;
    float week_win_percentage;
    int  level;
    struct LBNode* next;
} LBNode;

typedef struct Leaderboard {
    LBNode* head;
    int count;
} Leaderboard;

void initLeaderboard(Leaderboard* lb);

void addToLeaderboard(Leaderboard* lb, int user_id, const char* name,
                      int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level);

void removeFromLeaderboard(Leaderboard* lb, int user_id);

void updateLeaderboardEntry(Leaderboard* lb, int user_id,
                             int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level);

void sortLeaderboard(Leaderboard* lb);

void displayLeaderboard(Leaderboard* lb, int current_user_id);

int getUserRank(Leaderboard* lb, int user_id);

const char* getLevelNameLB(int level);

void freeLeaderboard(Leaderboard* lb);

#endif 

// ==================== inventory.c ====================

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

static int getTypeOrder(const char* t) {
    if(strcmp(t, "FW")==0) return 1;
    if(strcmp(t, "MF")==0) return 2;
    if(strcmp(t, "DF")==0) return 3;
    if(strcmp(t, "GK")==0) return 4;
    return 5;
}

void addPlayer(PlayerInventory* inv, const char* name, const char* type,        
               int rating, int price) {
    InventoryNode* node = createInventoryNode(name, type, rating, price);       

    if (inv->head == NULL) {
        inv->head = node;
        inv->tail = node;
        inv->count++;
        return;
    }
    
    InventoryNode* curr = inv->head;
    while(curr != NULL) {
        int orderNew = getTypeOrder(type);
        int orderCurr = getTypeOrder(curr->type);
        if(orderNew < orderCurr || (orderNew == orderCurr && rating > curr->rating)) {
            node->next = curr;
            node->prev = curr->prev;
            if(curr->prev != NULL) { curr->prev->next = node; } else { inv->head = node; }
            curr->prev = node;
            inv->count++;
            return;
        }
        curr = curr->next;
    }
    
    node->prev = inv->tail;
    inv->tail->next = node;
    inv->tail = node;
    inv->count++;
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

void seePlayers(PlayerInventory* inv){
    if (inv == NULL || inv->head == NULL) {
        printf("\nNo players available in inventory.\n");
        return;
    }

    char movement;
    InventoryNode* current = inv->head;

    printf("\n%-25s %-5s %-7s %-7s\n", "Name", "Type", "Rating", "Price");
    printf("%-25s %-5s %-7s %-7s\n", "-------------------------", "-----", "-------", "-------");

    while (1){
        printf("%-25s %-5s %-7d %-7d\n", current->name, current->type, current->rating, current->price);
        printf("%-25s %-5s %-7s %-7s\n", "-------------------------", "-----", "-------", "-------");

        printf("Enter direction (Next: D, Previous: A, Exit: 0): ");
        if (scanf(" %c", &movement) != 1) {
            printf("\nInput cannot be recognised. Please enter a valid input...\n");
            continue;
        }

        if (movement == 'D' || movement == 'd'){
            current = (current == inv->tail) ? inv->head : current->next;
        }
        else if (movement == 'A' || movement == 'a'){
            current = (current == inv->head) ? inv->tail : current->prev;
        }
        else if (movement == '0' || movement == 'o' || movement == 'O'){
            break;
        }
        else {
            printf("\nInput cannot be recognised. Please enter a valid input...\n");
        }
    }
}

// ==================== squad.c ====================

void initSquad(Squad* squad) {
    const char* types[SQUAD_SIZE] = {
        "FW","FW","FW",
        "MF","MF","MF",
        "DF","DF","DF","DF",
        "GK"
    };
    for (int i = 0; i < SQUAD_SIZE; i++) {
        strncpy(squad->slots[i].position_type, types[i], 3);
        squad->slots[i].position_type[3] = '\0';
        squad->slots[i].player_name[0]   = '\0';
        squad->slots[i].native_type[0]   = '\0';
        squad->slots[i].player_rating    = 0;
        squad->slots[i].is_filled        = false;
    }
}

static InventoryNode* findBestOfType(PlayerInventory* inv, Squad* squad, const char* type) {
    InventoryNode* best = NULL;
    InventoryNode* current = inv->head;
    while (current != NULL) {
        if (strcasecmp(current->type, type) == 0) {
            bool already_in = false;
            for (int k = 0; k < SQUAD_SIZE; k++) {
                if (squad->slots[k].is_filled && strcasecmp(squad->slots[k].player_name, current->name) == 0) {
                    already_in = true; break;
                }
            }
            if (!already_in) {
                if (best == NULL || current->rating > best->rating) best = current;
            }
        }
        current = current->next;
    }
    return best;
}

void autoSelectBestSquad(Squad* squad, PlayerInventory* inv) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        squad->slots[i].player_name[0] = '\0';
        squad->slots[i].player_rating  = 0;
        squad->slots[i].is_filled      = false;
    }
    for (int i = 0; i < SQUAD_SIZE; i++) {
        InventoryNode* best = findBestOfType(inv, squad, squad->slots[i].position_type);
        if (best != NULL) {
            strncpy(squad->slots[i].player_name, best->name, MAX_NAME_LEN - 1);
            squad->slots[i].player_name[MAX_NAME_LEN - 1] = '\0';
            strncpy(squad->slots[i].native_type, best->type, 3);
            squad->slots[i].native_type[3] = '\0';
            squad->slots[i].player_rating = best->rating;
            squad->slots[i].is_filled     = true;
        }
    }
    printf("Auto-selection complete.\n");
    displaySquad(squad);
}

void displaySquad(Squad* squad) {
    printf("\n=== Current Squad ===\n");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "Slot", "Pos", "Player", "P Pos", "Rating");
    printf("%-5s %-5s %-25s %-6s %-7s\n", "----", "-----", "-------------------------", "------", "-------");
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            printf("%-5d %-5s %-25s %-6s %-7d\n", i + 1, squad->slots[i].position_type, squad->slots[i].player_name, squad->slots[i].native_type, squad->slots[i].player_rating);
        } else {
            printf("%-5d %-5s %-25s %-6s %-7s\n", i + 1, squad->slots[i].position_type, "(empty)", "-", "-");
        }
    }
    printf("\nAverage Squad Rating: %d\n\n", getSquadAverageRating(squad));
}

int getSquadAverageRating(Squad* squad) {
    int total = 0, filled = 0;
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            total += squad->slots[i].player_rating;
            filled++;
        }
    }
    return (filled == 0) ? 0 : total / filled;
}

bool isPlayerInSquad(Squad* squad, const char* name) {
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled && isPartialMatch(squad->slots[i].player_name, name)) return true;
    }
    return false;
}

typedef struct TempCard {
    char name[MAX_NAME_LEN];
    char type[4];
    int rating;
    bool in_squad;
    int squad_slot;
} TempCard;

int compareCards(const void* a, const void* b) {
    TempCard* c1 = (TempCard*)a;
    TempCard* c2 = (TempCard*)b;
    // position sort: FW -> MF -> DF -> GK
    int pos1 = 0, pos2 = 0;
    if (strcmp(c1->type, "FW") == 0) pos1 = 1;
    else if (strcmp(c1->type, "MF") == 0) pos1 = 2;
    else if (strcmp(c1->type, "DF") == 0) pos1 = 3;
    else if (strcmp(c1->type, "GK") == 0) pos1 = 4;
    
    if (strcmp(c2->type, "FW") == 0) pos2 = 1;
    else if (strcmp(c2->type, "MF") == 0) pos2 = 2;
    else if (strcmp(c2->type, "DF") == 0) pos2 = 3;
    else if (strcmp(c2->type, "GK") == 0) pos2 = 4;
    
    if (pos1 != pos2) return pos1 - pos2;
    return c2->rating - c1->rating;
}

int compareCardsRating(const void* a, const void* b) {
    TempCard* c1 = (TempCard*)a;
    TempCard* c2 = (TempCard*)b;
    return c2->rating - c1->rating;
}

void displayAllUserCards(Squad* squad, PlayerInventory* inv) {
    TempCard cards[200];
    int count = 0;
    
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            strcpy(cards[count].name, squad->slots[i].player_name);
            strcpy(cards[count].type, squad->slots[i].native_type);
            cards[count].rating = squad->slots[i].player_rating;
            cards[count].in_squad = true;
            cards[count].squad_slot = i;
            count++;
        }
    }
    
    int squad_count = count;
    
    InventoryNode* curr = inv->head;
    while (curr != NULL) {
        bool already_in = false;
        for (int i = 0; i < SQUAD_SIZE; i++) {
            if (squad->slots[i].is_filled && strcmp(squad->slots[i].player_name, curr->name) == 0) {
                already_in = true; break;
            }
        }
        if (!already_in) {
            strcpy(cards[count].name, curr->name);
            strcpy(cards[count].type, curr->type);
            cards[count].rating = curr->rating;
            cards[count].in_squad = false;
            cards[count].squad_slot = -1;
            count++;
        }
        curr = curr->next;
    }
    
    // Lineup gets ordered by position FW -> MF -> DF -> GK
    qsort(cards, squad_count, sizeof(TempCard), compareCards);
    
    // Other players ordered strictly by rating descending
    if (count > squad_count) {
        qsort(cards + squad_count, count - squad_count, sizeof(TempCard), compareCardsRating);
    }
    
    printf("\n=== All Player Cards ===\n");
    printf("\n--- LINEUP ---\n");
    printf("%-3s %-25s %-6s %-7s\n", "#", "Player", "P Pos", "Rating");
    printf("%-3s %-25s %-6s %-7s\n", "---", "-------------------------", "------", "-------");
    
    for (int i = 0; i < squad_count; i++) {
        printf("%-3d %-25s %-6s %-7d\n", i + 1, cards[i].name, cards[i].type, cards[i].rating);
    }
    
    if (count > squad_count) {
        printf("\n--- OTHER PLAYERS ---\n");
        printf("--------------------------------------------\n");
        for (int i = squad_count; i < count; i++) {
            printf("%-3d %-25s %-6s %-7d\n", i + 1, cards[i].name, cards[i].type, cards[i].rating);
        }
    }
    printf("\n");
}

void swapPlayerCards(Squad* squad, PlayerInventory* inv, const char* name1, const char* name2) {
    int s1 = -1, s2 = -1;
    
    for (int i = 0; i < SQUAD_SIZE; i++) {
        if (squad->slots[i].is_filled) {
            if (s1 == -1 && isPartialMatch(squad->slots[i].player_name, name1)) s1 = i;
            if (s2 == -1 && isPartialMatch(squad->slots[i].player_name, name2)) s2 = i;
        }
    }
    
    InventoryNode* i1 = NULL;
    InventoryNode* i2 = NULL;
    InventoryNode* curr = inv->head;
    while (curr != NULL) {
        if (i1 == NULL && isPartialMatch(curr->name, name1)) i1 = curr;
        if (i2 == NULL && isPartialMatch(curr->name, name2)) i2 = curr;
        curr = curr->next;
    }
    
    if ((s1 == -1 && i1 == NULL) || (s2 == -1 && i2 == NULL)) {
        printf("One or both players not found.\n");
        return;
    }
    
    if (s1 != -1 && s2 != -1) {
        char temp_name[MAX_NAME_LEN];
        char temp_type[4];
        int temp_rating;
        
        strcpy(temp_name, squad->slots[s1].player_name);
        strcpy(temp_type, squad->slots[s1].native_type);
        temp_rating = squad->slots[s1].player_rating;
        
        strcpy(squad->slots[s1].player_name, squad->slots[s2].player_name);
        strcpy(squad->slots[s1].native_type, squad->slots[s2].native_type);
        squad->slots[s1].player_rating = squad->slots[s2].player_rating;
        
        strcpy(squad->slots[s2].player_name, temp_name);
        strcpy(squad->slots[s2].native_type, temp_type);
        squad->slots[s2].player_rating = temp_rating;
        
        printf("Swapped %s and %s in the squad.\n", squad->slots[s2].player_name, squad->slots[s1].player_name);
        return;
    }
    
    if (s1 != -1 && s2 == -1 && i2 != NULL) {
        printf("Replaced %s with %s in the squad.\n", squad->slots[s1].player_name, i2->name);
        strcpy(squad->slots[s1].player_name, i2->name);
        strcpy(squad->slots[s1].native_type, i2->type);
        squad->slots[s1].player_rating = i2->rating;
        return;
    }
    
    if (s2 != -1 && s1 == -1 && i1 != NULL) {
        printf("Replaced %s with %s in the squad.\n", squad->slots[s2].player_name, i1->name);
        strcpy(squad->slots[s2].player_name, i1->name);
        strcpy(squad->slots[s2].native_type, i1->type);
        squad->slots[s2].player_rating = i1->rating;
        return;
    }
    
    printf("At least one player must be in the squad to swap.\n");
}

// ==================== matchhistory.c ====================

void initMatchStack(MatchStack* stack) {
    stack -> top           = NULL;
    stack -> total_matches = 0;
    stack -> total_wins    = 0;
    stack -> total_losses  = 0;
    stack -> total_draws   = 0;
    stack -> week_wins     = 0;
    stack -> week_matches  = 0;
    stack -> current_week  = 1;
}

bool isStackEmpty(MatchStack* stack) {
    return stack -> top == NULL;
}

void pushMatch(MatchStack* stack, const char* opponent,
               char result, int coins, int week,
               int my_goals, int opp_goals,
               int my_shots, int opp_shots,
               int my_assists, int opp_assists,
               int my_fouls, int opp_fouls,
               int my_offsides, int opp_offsides,
               int my_red_cards, int opp_red_cards,
               int my_yellow_cards, int opp_yellow_cards) {
    MatchRecord* record = (MatchRecord *) malloc(sizeof(MatchRecord));
    if (record == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strncpy(record -> opponent_name, opponent, MAX_NAME_LEN - 1);
    record -> opponent_name[MAX_NAME_LEN - 1] = '\0';
    record -> result       = result;
    record -> coins_earned = coins;
    record -> week         = week;
    
    int match_count = 0;
    MatchRecord* curr = stack->top;
    while (curr) {
        if (curr->week == week) match_count++;
        curr = curr->next;
    }
    record -> match_number = match_count + 1;
    
    record -> my_goals = my_goals;
    record -> opp_goals = opp_goals;
    record -> my_shots = my_shots;
    record -> opp_shots = opp_shots;
    record -> my_assists = my_assists;
    record -> opp_assists = opp_assists;
    record -> my_fouls = my_fouls;
    record -> opp_fouls = opp_fouls;
    record -> my_offsides = my_offsides;
    record -> opp_offsides = opp_offsides;
    record -> my_red_cards = my_red_cards;
    record -> opp_red_cards = opp_red_cards;
    record -> my_yellow_cards = my_yellow_cards;
    record -> opp_yellow_cards = opp_yellow_cards;

    record -> next         = stack -> top;

    stack -> top = record;
    stack -> total_matches++;

    if (result == RESULT_WIN) {
        stack -> total_wins++;
        stack -> week_wins++;
        stack -> week_matches++;
    } else if (result == RESULT_LOSS) {
        stack -> total_losses++;
        stack -> week_matches++;
    } else if (result == RESULT_DRAW) {
        stack -> total_draws++;
        stack -> week_matches++;
    }
}

void popMatch(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("Match history is empty.\n");
        return;
    }

    MatchRecord* temp = stack -> top;
    stack -> top        = stack -> top->next;

    stack -> total_matches--;
    if (temp -> result == RESULT_WIN) {
        stack -> total_wins--;
    } else if (temp -> result == RESULT_LOSS) {
        stack -> total_losses--;
    } else if (temp -> result == RESULT_DRAW) {
        stack -> total_draws--;
    }

    free(temp);
}

void displayMatchHistory(MatchStack* stack) {
    if (isStackEmpty(stack)) {
        printf("No match history available.\n");
        return;
    }

    printf("\n=== Match History ===\n");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "#", "Opponent", "Result", "Match No", "Week");
    printf("%-5s %-20s %-7s %-10s %-5s\n",
           "---", "--------------------", "------", "--------", "----");

    MatchRecord* current = stack -> top;
    int num = 1;
    
    int min_week = stack->current_week - 3;
    if (min_week < 1) min_week = 1;

    while (current != NULL) {
        if (current->week >= min_week && current->week <= stack->current_week) {
            const char* result_str;
            if (current -> result == RESULT_WIN) {
                result_str = "WIN";
            } else if (current -> result == RESULT_LOSS) {
                result_str = "LOSS";
            } else {
                result_str = "DRAW";
            }

            printf("%-5d %-20s %-7s %-10d %-5d\n",
                   num, current -> opponent_name,
                   result_str, current -> match_number, current -> week);
            num++;
        }
        current = current -> next;
    }

    printf("\nTotal: %d matches | %d wins | %d losses | %d draws\n",
           stack -> total_matches, stack -> total_wins,
           stack -> total_losses, stack -> total_draws);
}

float getWinPercentage(MatchStack* stack) {
    if (stack -> total_matches == 0) {
        return 0.0f;
    }
    return (float)stack -> total_wins / (float)stack -> total_matches * 100.0f;
}

float getWeeklyWinPercentage(MatchStack* stack) {
    if (stack -> week_matches == 0) {
        return 0.0f;
    }
    return (float)stack -> week_wins / (float)stack -> week_matches * 100.0f;
}

void resetWeeklyStats(MatchStack* stack) {
    stack -> week_wins    = 0;
    stack -> week_matches = 0;
    stack -> current_week++;
}

void freeMatchStack(MatchStack* stack) {
    while (!isStackEmpty(stack)) {
        popMatch(stack);
    }
}

void displayStatistics(MatchStack* stack, int week, int match_num, const char* user_name) {
    if (week < stack->current_week - 3 || week > stack->current_week) {
        printf("Statistics are only available for the last few weeks (Weeks %d to %d).\n", 
               stack->current_week - 3, stack->current_week);
        return;
    }
    
    MatchRecord* curr = stack->top;
    while (curr) {
        if (curr->week == week && curr->match_number == match_num) {
            printf("\n");
            printf("                        === Match Statistics ===\n");
            printf("                       Opponent: %s\n", curr->opponent_name);
            printf("-----------------------------------------------------------------------\n");
            printf("             %20s | %-20s\n", user_name, curr->opponent_name);
            printf("-----------------------------------------------------------------------\n");
            printf("      Goals: %20d | %-20d\n", curr->my_goals, curr->opp_goals);
            printf("Shots on Goal: %18d | %-20d\n", curr->my_shots, curr->opp_shots);
            printf("    Assists: %20d | %-20d\n", curr->my_assists, curr->opp_assists);
            printf("      Fouls: %20d | %-20d\n", curr->my_fouls, curr->opp_fouls);
            printf("   Offsides: %20d | %-20d\n", curr->my_offsides, curr->opp_offsides);
            printf("  Red Cards: %20d | %-20d\n", curr->my_red_cards, curr->opp_red_cards);
            printf("Yellow Cards: %19d | %-20d\n", curr->my_yellow_cards, curr->opp_yellow_cards);
            printf("-----------------------------------------------------------------------\n");
            return;
        }
        curr = curr->next;
    }
    printf("Match not found for Week %d, Match Number %d.\n", week, match_num);
}

// ==================== user_registration.c ====================

static const char* LEVEL_NAMES[] = {
    "Amateur", "Pro", "World Class", "Legendary"
};

static const int WEEKLY_REWARDS[] = {
    15000, 30000, 75000, 100000
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
    newNode -> reward_history_count = 5;
    
    // Default 5 weeks history simulating previously varying ranks across 25 total users
    int default_ranks[5] = {4, 8, 14, 19, 2};
    int default_levels[5] = {3, 2, 1, 0, 3};
    for(int i=0; i<5; i++) {
        newNode -> past_week_levels[i] = default_levels[i];
        newNode -> past_week_ranks[i] = default_ranks[i];
        newNode -> past_week_rewards[i] = getWeeklyReward(default_levels[i]);
        newNode -> past_weeks[i] = 5 - i; 
    }
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
    if (level < 0 || level > 3) {
        return "Unknown";
    }
    return LEVEL_NAMES[level];
}

void updateUserLevel(UserNode* user, int rank) {
    if (rank <= 0) return;
    int new_level;

    if (rank <= 5) new_level = 3;
    else if (rank <= 10) new_level = 2;
    else if (rank <= 15) new_level = 1;
    else new_level = 0;

    if (new_level != user -> level) {
        printf("  >> Level changed: %s -> %s\n",
               getLevelName(user -> level), getLevelName(new_level));
        user -> level = new_level;
    }
}

int getWeeklyReward(int level) {
    if (level < 0 || level > 3) {
        return 0;
    }
    return WEEKLY_REWARDS[level];
}

bool isPartialMatch(const char* str, const char* substr) {
    if (!str || !substr) return false;
    char lowerStr[150], lowerSub[150];
    strncpy(lowerStr, str, 149); lowerStr[149]='\0';
    strncpy(lowerSub, substr, 149); lowerSub[149]='\0';
    for(int i=0; lowerStr[i]; i++) lowerStr[i] = tolower((unsigned char)lowerStr[i]);
    for(int i=0; lowerSub[i]; i++) lowerSub[i] = tolower((unsigned char)lowerSub[i]);
    return strstr(lowerStr, lowerSub) != NULL;
}

// ==================== marketplace.c ====================

void initMarketplace(Marketplace* market) {
    market->head = NULL;
    market->count = 0;
}

void addToMarketplace(Marketplace* market, const char* name, const char* type,  
                      int rating, int price) {
    MarketNode* node = (MarketNode *) malloc(sizeof(MarketNode));
    if (!node) return;
    strncpy(node->name, name, MAX_NAME_LEN - 1); node->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(node->type, type, TYPE_LEN - 1); node->type[TYPE_LEN - 1] = '\0';
    node->rating = rating;
    node->price = price;
    node->next = node->prev = NULL;

    if (market->head == NULL) {
        market->head = node;
        node->next = node;
        node->prev = node;
    } else {
        MarketNode* curr = market->head;
        bool inserted = false;
        
        do {
            if (rating > curr->rating) {
                node->prev = curr->prev;
                node->next = curr;
                curr->prev->next = node;
                curr->prev = node;
                if (curr == market->head) market->head = node;
                inserted = true;
                break;
            }
            curr = curr->next;
        } while (curr != market->head);
        
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
    if (market->head == NULL) {
        return NULL;
    }

    MarketNode* current = market->head;

    for (int i = 0; i < market->count; i++) {
        if (strcasecmp(current->name, name) == 0) {
            if (market->count == 1) {
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

MarketNode* searchMarketByName(Marketplace* market, const char* name) {
    if (market->head == NULL) {
        return NULL;
    }

    MarketNode* current = market->head;

    for (int i = 0; i < market->count; i++) {
        if (isPartialMatch(current->name, name)) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void searchMarketByType(Marketplace* market, const char* type) {
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return;
    }

    bool found = false;
    MarketNode* current = market->head;

    printf("\n%-25s %-5s %-7s %-8s\n",
           "Name", "Type", "Rating", "Price");
    printf("%-25s %-5s %-7s %-8s\n",
           "-------------------------", "-----", "-------", "--------");

    for (int i = 0; i < market->count; i++) {
        if (strcasecmp(current->type, type) == 0) {
            printf("%-25s %-5s %-7d %-8d\n",
                   current->name, current->type,
                   current->rating, current->price);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("No players of type '%s' found in marketplace.\n", type);
    }
    printf("\n");
}

void displayMarketplace(Marketplace* market) {
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return;
    }

    printf("\n%-3s %-25s %-5s %-7s %-8s\n",
           "#", "Name", "Type", "Rating", "Price");
    printf("%-3s %-25s %-5s %-7s %-8s\n",
           "---", "-------------------------", "-----", "-------", "--------");

    MarketNode* current = market->head;
    for (int i = 0; i < market->count; i++) {
        printf("%-3d %-25s %-5s %-7d %-8d\n",
               i + 1, current->name, current->type,
               current->rating, current->price);
        current = current->next;
    }
    printf("\nTotal listings: %d\n\n", market->count);
}

void seedMarketplace(Marketplace* market) {
    const char* fw_names[] = {"Lionel Messi", "Cristiano Ronaldo", "Kylian Mbappe", "Erling Haaland", "Neymar Jr", "Mohamed Salah", "Harry Kane", "Vinicius Junior", "Lautaro Martinez", "Marcus Rashford"};
    const char* mf_names[] = {"Kevin De Bruyne", "Luka Modric", "Toni Kroos", "Bruno Fernandes", "Pedri", "Jude Bellingham", "Bernardo Silva", "Declan Rice", "Frenkie De Jong", "Rodri"};
    const char* df_names[] = {"Joao Cancelo", "Kyle Walker", "Trent Alexander Arnold", "Virgil Van Dijk", "Ruben Dias", "Antonio Rudiger", "Kalidou Koulibaly", "Eder Militao", "Marquinhos", "Achraf Hakimi", "Alphonso Davies", "Andrew Robertson", "Reece James", "Dani Carvajal", "Milan Skriniar", "Mats Hummels", "Aymeric Laporte", "Jules Kounde", "Alessandro Bastoni", "Pau Torres"};
    const char* gk_names[] = {"Emiliano Martinez", "Thibaut Courtois", "Ederson Moraes", "Alisson Becker", "Mike Maignan", "Gianluigi Donnarumma", "Andre Onana", "Keylor Navas", "Wojciech Szczesny"};

    // Generate FW variants
    for (int i = 0; i < 10; i++) {
        int r = 60 + rand() % 51; // 60 to 110
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, fw_names[i], "FW", r, p);
        r = 60 + rand() % 51;
        p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, fw_names[i], "FW", r, p);
    }
    // Generate MF variants
    for (int i = 0; i < 10; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, mf_names[i], "MF", r, p);
    }
    // Generate DF variants
    for (int i = 0; i < 20; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, df_names[i], "DF", r, p);
    }
    // Generate GK variants
    for (int i = 0; i < 9; i++) {
        int r = 60 + rand() % 51;
        int p = 5000 + ((r - 60) * (r - 60) * 198);
        addToMarketplace(market, gk_names[i], "GK", r, p);
    }
}

void freeMarketplace(Marketplace* market) {
    if (market->head == NULL) {
        return;
    }

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
    if (market->head == NULL) {
        printf("Marketplace is empty.\n");
        return false;
    }
    printf("\n--- Search Results ---\n");
    printf("%-20s | %-4s | %-6s | %-6s\n", "Name", "Type", "Rating", "Price");
    printf("--------------------------------------------------\n");
    
    MarketNode* curr = market->head;
    bool found = false;
    
    for (int i = 0; i < market->count; i++) {
        bool match = true;
        if (curr->rating < min_rating || curr->rating > max_rating) match = false;
        if (strlen(type) > 0 && strcasecmp(curr->type, type) != 0) match = false;
        if (strlen(name) > 0 && !isPartialMatch(curr->name, name)) match = false;
        
        if (match) {
            printf("%-20s | %-4s | %-6d | %-6d\n", curr->name, curr->type, curr->rating, curr->price);
            found = true;
        }
        curr = curr->next;
    }
    
    return found;
}

// ==================== matchmaking.c ====================

void initMatchQueue(MatchQueue* queue) {
    queue -> front = NULL;
    queue -> rear  = NULL;
    queue -> size  = 0;
}

bool isQueueEmpty(MatchQueue* queue) {
    return queue -> front == NULL;
}

void enqueueUser(MatchQueue* queue, int user_id, int user_level) {
    MatchQueueNode* newNode = (MatchQueueNode *) malloc(sizeof(MatchQueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode -> user_id    = user_id;
    newNode -> user_level = user_level;
    newNode -> next       = NULL;

    if (isQueueEmpty(queue)) {
        queue -> front = newNode;
        queue -> rear  = newNode;
    } else {
        queue -> rear->next = newNode;
        queue -> rear       = newNode;
    }

    queue -> size++;
}

int dequeueUser(MatchQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return -1;
    }

    MatchQueueNode* temp = queue -> front;
    int user_id          = temp -> user_id;

    if (queue -> front == queue -> rear) {
        queue -> front = NULL;
        queue -> rear  = NULL;
    } else {
        queue -> front = queue -> front->next;
    }

    free(temp);
    queue -> size--;
    return user_id;
}

int peekQueue(MatchQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return -1;
    }
    return queue -> front->user_id;
}

bool isUserInQueue(MatchQueue* queue, int user_id) {
    MatchQueueNode* current = queue -> front;
    while (current != NULL) {
        if (current -> user_id == user_id) {
            return true;
        }
        current = current -> next;
    }
    return false;
}

void displayMatchQueue(MatchQueue* queue, UserRegistry* registry) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return;
    }

    printf("\n=== Matchmaking Queue ===\n");
    printf("%-5s %-12s %-5s\n", "Pos", "User Name", "Level");
    printf("%-5s %-12s %-5s\n", "---", "---------", "-----");

    MatchQueueNode* current = queue -> front;
    int pos = 1;

    // Use a dummy user registry logic block wrapper since it might not be fully passed securely by context in some spots
    while (current != NULL) {
        UserNode* u = findUserByID(registry, current->user_id);
        char* d_name = u ? u->name : "Unknown";
        
        printf("%-5d %-12s %-5s\n", pos, d_name, getLevelName(current -> user_level));
        current = current -> next;
        pos++;
    }
    printf("Queue size: %d\n\n", queue -> size);
}

int findSameLevelOpponent(MatchQueue* queue, int user_id, int user_level) {
    MatchQueueNode* current = queue -> front;
    while (current != NULL) {
        if (current -> user_id != user_id &&
            current -> user_level == user_level) {
            return current -> user_id;
        }
        current = current -> next;
    }
    return -1;
}

void removeFromQueue(MatchQueue* queue, int user_id) {
    MatchQueueNode* current  = queue -> front;
    MatchQueueNode* previous = NULL;

    while (current != NULL) {
        if (current -> user_id == user_id) {
            if (previous == NULL) {
                queue -> front = current -> next;
            } else {
                previous -> next = current -> next;
            }

            if (current == queue -> rear) {
                queue -> rear = previous;
            }

            free(current);
            queue -> size--;
            return;
        }
        previous = current;
        current  = current -> next;
    }
}

// ==================== leaderboard.c ====================

static const char* LB_LEVEL_NAMES[] = {
    "Amateur", "Pro", "World Class", "Legendary"
};

const char* getLevelNameLB(int level) {
    if (level < 0 || level > 3) {
        return "Unknown";
    }
    return LB_LEVEL_NAMES[level];
}

void initLeaderboard(Leaderboard* lb) {
    lb -> head  = NULL;
    lb -> count = 0;
}

void addToLeaderboard(Leaderboard* lb, int user_id, const char* name,
                      int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level) {
    LBNode* newNode = (LBNode *) malloc(sizeof(LBNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode -> user_id = user_id;
    strncpy(newNode -> user_name, name, MAX_NAME_LEN - 1);
    newNode -> user_name[MAX_NAME_LEN - 1] = '\0';
    newNode -> wins  = wins;
    newNode -> losses = losses;
    newNode -> draws = draws;
    newNode -> win_percentage = win_percentage;
    newNode -> week_win_percentage = week_win_percentage;
    newNode -> level = level;
    newNode -> next  = NULL;

    if (lb -> head == NULL) {
        newNode -> next = newNode;  
        lb -> head = newNode;
    } else {
        
        LBNode* tail = lb -> head;
        while (tail -> next != lb -> head) {
            tail = tail -> next;
        }
        tail -> next    = newNode;
        newNode -> next = lb -> head;
    }

    lb -> count++;
    sortLeaderboard(lb);
}

void removeFromLeaderboard(Leaderboard* lb, int user_id) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* current  = lb -> head;
    LBNode* previous = NULL;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            if (lb -> count == 1) {
                lb -> head = NULL;
            } else {
                if (previous == NULL) {
                    
                    LBNode* tail = lb -> head;
                    while (tail -> next != lb -> head) {
                        tail = tail -> next;
                    }
                    lb -> head   = current -> next;
                    tail -> next = lb -> head;
                } else {
                    previous -> next = current -> next;
                }
            }
            free(current);
            lb -> count--;
            return;
        }
        previous = current;
        current  = current -> next;
    }
}

void updateLeaderboardEntry(Leaderboard* lb, int user_id,
                             int wins, int losses, int draws, float win_percentage, float week_win_percentage, int level) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* current = lb -> head;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            current -> wins  = wins;
            current -> losses = losses;
            current -> draws = draws;
            current -> win_percentage = win_percentage;
            current -> week_win_percentage = week_win_percentage;
            current -> level = level;
            sortLeaderboard(lb);
            return;
        }
        current = current -> next;
    }
}

void sortLeaderboard(Leaderboard* lb) {
    if (lb -> count <= 1) {
        return;
    }

    bool swapped;
    int pass;

    for (pass = 0; pass < lb -> count - 1; pass++) {
        swapped = false;
        LBNode* current = lb -> head;
        int i;

        for (i = 0; i < lb -> count - 1 - pass; i++) {
            LBNode* next_node = current -> next;

            if (current -> wins < next_node -> wins) {
                
                int  tmp_id    = current -> user_id;
                int  tmp_wins  = current -> wins;
                int  tmp_losses = current -> losses;
                int  tmp_draws  = current -> draws;
                float tmp_win_perc = current -> win_percentage;
                float tmp_week_win_perc = current -> week_win_percentage;
                int  tmp_level = current -> level;
                char tmp_name[MAX_NAME_LEN];
                strncpy(tmp_name, current -> user_name, MAX_NAME_LEN);

                current -> user_id = next_node -> user_id;
                current -> wins    = next_node -> wins;
                current -> losses  = next_node -> losses;
                current -> draws   = next_node -> draws;
                current -> win_percentage = next_node -> win_percentage;
                current -> week_win_percentage = next_node -> week_win_percentage;
                current -> level   = next_node -> level;
                strncpy(current -> user_name, next_node -> user_name, MAX_NAME_LEN);

                next_node -> user_id = tmp_id;
                next_node -> wins    = tmp_wins;
                next_node -> losses  = tmp_losses;
                next_node -> draws   = tmp_draws;
                next_node -> win_percentage = tmp_win_perc;
                next_node -> week_win_percentage = tmp_week_win_perc;
                next_node -> level   = tmp_level;
                strncpy(next_node -> user_name, tmp_name, MAX_NAME_LEN);

                swapped = true;
            }
            current = current -> next;
        }

        if (!swapped) {
            break;
        }
    }
}

static int getSimulatedRank(int level, int real_rank) {
    return real_rank;
}

void displayLeaderboard(Leaderboard* lb, int current_user_id) {
    if (lb -> head == NULL) {
        printf("Leaderboard is empty.\n");
        return;
    }

    printf("\n=== Global Leaderboard ===\n");
    printf("%-5s | %-20s | %-15s | %-5s | %-6s | %-5s | %-6s\n",
           "Rank", "Name", "Level", "Wins", "Losses", "Draws", "Win %");
    printf("--------------------------------------------------------------------------------\n");

    LBNode* current = lb -> head;
    int rank = 1;
    int i;
    int last_level = -1;
    int displayed_count = 0;
    
    LBNode* current_user_node = NULL;
    int current_user_rank = -1;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == current_user_id) {
            current_user_node = current;
            current_user_rank = rank;
        }

        if (displayed_count < 8) {
            if (last_level != -1 && last_level != current->level) {
                printf("--------------------------------------------------------------------------------\n");
            }
            
            float total_matches = current->wins + current->losses + current->draws;
            float win_perc = 0.0;
            if (total_matches > 0) {
                win_perc = (current->wins / total_matches) * 100.0;
            }
            
            int sim_rank = getSimulatedRank(current->level, rank);
            printf("%-5d | %-20s | %-15s | %-5d | %-6d | %-5d | %-5.1f%%\n",
                   sim_rank, current -> user_name,
                   getLevelNameLB(current -> level), current -> wins, current -> losses, current->draws, win_perc);
            
            last_level = current->level;
            displayed_count++;
        }
        
        current = current -> next;
        rank++;
    }
    
    if (current_user_node != NULL && current_user_rank > 8) {
        printf("--------------------------------------------------------------------------------\n");
        for (int j = 0; j < 5; j++) {
            printf("%-5s | %-20s | %-15s | %-5s | %-6s | %-5s | %-5s\n", "", "", "", "", "", "", "");
        }
        printf("--------------------------------------------------------------------------------\n");
        float total_matches = current_user_node->wins + current_user_node->losses + current_user_node->draws;
        float win_perc = 0.0;
        if (total_matches > 0) {
            win_perc = (current_user_node->wins / total_matches) * 100.0;
        }
        int sim_user_rank = getSimulatedRank(current_user_node->level, current_user_rank);
        printf("%-5d | %-20s | %-15s | %-5d | %-6d | %-5d | %-5.1f%%\n",
               sim_user_rank, current_user_node -> user_name,
               getLevelNameLB(current_user_node -> level), current_user_node -> wins, current_user_node -> losses, current_user_node->draws, win_perc);
    }
    
    printf("\n");
}

int getUserRank(Leaderboard* lb, int user_id) {
    if (lb -> head == NULL) {
        return -1;
    }

    LBNode* current = lb -> head;
    int rank = 1;
    int i;

    for (i = 0; i < lb -> count; i++) {
        if (current -> user_id == user_id) {
            return getSimulatedRank(current->level, rank);
        }
        current = current -> next;
        rank++;
    }
    return -1;
}

void freeLeaderboard(Leaderboard* lb) {
    if (lb -> head == NULL) {
        return;
    }

    LBNode* tail = lb -> head;
    while (tail -> next != lb -> head) {
        tail = tail -> next;
    }
    tail -> next = NULL;

    LBNode* current = lb -> head;
    while (current != NULL) {
        LBNode* temp = current;
        current = current -> next;
        free(temp);
    }
    lb -> head  = NULL;
    lb -> count = 0;
}

// ==================== main.c ====================


static UserRegistry  g_registry;
static Marketplace   g_market;
static Leaderboard   g_leaderboard;
static MatchQueue    g_matchQueue;
static int           g_current_week = 1;

static void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        
    }
}

static void readLine(char* buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        
        int len = (int)strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    }
}

static void initUserModules(UserNode* user) {
    
    user -> inventory = (PlayerInventory *) malloc(sizeof(PlayerInventory));
    if (user -> inventory == NULL) { printf("Memory error\n"); exit(1); }
    initInventory(user -> inventory);
    giveInitialPlayers(user -> inventory, user -> id);

    user -> squad = (Squad *) malloc(sizeof(Squad));
    if (user -> squad == NULL) { printf("Memory error\n"); exit(1); }
    initSquad(user -> squad);

    user -> match_history = (MatchStack *) malloc(sizeof(MatchStack));
    if (user -> match_history == NULL) { printf("Memory error\n"); exit(1); }
    initMatchStack(user -> match_history);
}

static void addMatchRecord(UserNode* user, const char* opponent,
                           char result, int coins, int week) {
    int goals_for = 0, goals_against = 0;
    if (result == RESULT_WIN) {
        goals_for = rand() % 4 + 1;
        goals_against = rand() % goals_for;
    } else if (result == RESULT_LOSS) {
        goals_against = rand() % 4 + 1;
        goals_for = rand() % goals_against;
    } else {
        goals_for = goals_against = rand() % 3;
    }
    int shots = goals_for + rand() % 5;
    int opp_shots = goals_against + rand() % 5;
    int assists = (goals_for > 0) ? rand() % goals_for : 0;
    int opp_assists = (goals_against > 0) ? rand() % goals_against : 0;
    int fouls = rand() % 6;
    int opp_fouls = rand() % 6;
    int offsides = rand() % 4;
    int opp_offsides = rand() % 4;
    int red_cards = rand() % 2;
    int opp_red_cards = rand() % 2;
    int yellow_cards = rand() % 4;
    int opp_yellow_cards = rand() % 4;

    pushMatch(user -> match_history, opponent, result, coins, week,
              goals_for, goals_against, shots, opp_shots, assists, opp_assists,
              fouls, opp_fouls, offsides, opp_offsides,
              red_cards, opp_red_cards, yellow_cards, opp_yellow_cards);
    user -> total_wins   = user -> match_history->total_wins;
    user -> total_losses = user -> match_history->total_losses;
    user -> total_draws  = user -> match_history->total_draws;
    user -> coins += coins;
}

static void updateAllGlobalLevels() {
    if (g_leaderboard.head == NULL) return;
    LBNode* curr = g_leaderboard.head;
    int rank = 1;
    int count = g_leaderboard.count;
    for (int i = 0; i < count; i++) {
        UserNode* user = findUserByID(&g_registry, curr->user_id);
        if (user) {
            updateUserLevel(user, rank);
            curr->level = user->level;
        }
        curr = curr->next;
        rank++;
    }
}

static void seedPredefinedUsers(void) {
    static const char* names[18] = {
        "ALPHASTRIKE", "BETAFORCE", "GAMMAKNIGHT", "DELTABLITZ", "EPSILONSTAR",
        "ZETASHIELD", "ETASTORM", "THETAFANG", "IOTABRINGER", "KAPPANULL",
        "LAMBDALION", "MURAVEN", "NUVIPER", "XIDRAGON", "OMICRONBEAR",
        "PIWOLF", "RHOEAGLE", "SIGMASHARK"
    };
    UserNode* u[18];
    int i;

    for (i = 0; i < 18; i++) {
        u[i] = addUser(&g_registry, names[i], "PASS123");
        if (u[i] == NULL) {
            u[i] = findUserByName(&g_registry, names[i]);
            continue;
        }
        initUserModules(u[i]);
    }

    addMatchRecord(u[0], "BETAFORCE",   RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "GAMMAKNIGHT", RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "EPSILONSTAR", RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "DELTABLITZ",  RESULT_DRAW, 250, 1);
    resetWeeklyStats(u[0]->match_history);
    
    addMatchRecord(u[0], "BotPlayer1",  RESULT_WIN,  500, 2);
    addMatchRecord(u[0], "GAMMAKNIGHT", RESULT_WIN,  500, 2);
    addMatchRecord(u[0], "BETAFORCE",   RESULT_LOSS, 0,   2);
    addMatchRecord(u[0], "BotPlayer2",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[0]->match_history);
    
    addMatchRecord(u[0], "DELTABLITZ",  RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "EPSILONSTAR", RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "BotPlayer3",  RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "GAMMAKNIGHT", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[0]->match_history);
    
    addMatchRecord(u[0], "BETAFORCE",   RESULT_WIN,  500, 4);
    addMatchRecord(u[0], "BotPlayer1",  RESULT_LOSS, 0,   4);
    addMatchRecord(u[0], "DELTABLITZ",  RESULT_WIN,  500, 4);
    addMatchRecord(u[0], "EPSILONSTAR", RESULT_WIN,  500, 4);
    resetWeeklyStats(u[0]->match_history);
    
    addMatchRecord(u[0], "BotPlayer2",  RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "GAMMAKNIGHT", RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "BotPlayer3",  RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "BETAFORCE",   RESULT_LOSS, 0,   5);

    addMatchRecord(u[1], "ALPHASTRIKE", RESULT_LOSS, 0,   1);
    addMatchRecord(u[1], "GAMMAKNIGHT", RESULT_WIN,  500, 1);
    addMatchRecord(u[1], "DELTABLITZ",  RESULT_WIN,  500, 1);
    addMatchRecord(u[1], "EPSILONSTAR", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[1]->match_history);
    
    addMatchRecord(u[1], "ALPHASTRIKE", RESULT_WIN,  500, 2);
    addMatchRecord(u[1], "BotPlayer1",  RESULT_DRAW, 250, 2);
    addMatchRecord(u[1], "DELTABLITZ",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[1]->match_history);
    
    addMatchRecord(u[1], "GAMMAKNIGHT", RESULT_LOSS, 0,   3);
    addMatchRecord(u[1], "BotPlayer2",  RESULT_WIN,  500, 3);
    addMatchRecord(u[1], "EPSILONSTAR", RESULT_WIN,  500, 3);
    resetWeeklyStats(u[1]->match_history);
    
    addMatchRecord(u[1], "ALPHASTRIKE", RESULT_LOSS, 0,   4);
    addMatchRecord(u[1], "BotPlayer3",  RESULT_WIN,  500, 4);
    addMatchRecord(u[1], "GAMMAKNIGHT", RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[1]->match_history);
    
    addMatchRecord(u[1], "DELTABLITZ",  RESULT_WIN,  500, 5);
    addMatchRecord(u[1], "ALPHASTRIKE", RESULT_LOSS, 0,   5);
    addMatchRecord(u[1], "EPSILONSTAR", RESULT_LOSS, 0,   5);
    addMatchRecord(u[1], "BotPlayer1",  RESULT_WIN,  500, 5);

    addMatchRecord(u[2], "ALPHASTRIKE", RESULT_LOSS, 0,   1);
    addMatchRecord(u[2], "BETAFORCE",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[2], "DELTABLITZ",  RESULT_WIN,  500, 1);
    addMatchRecord(u[2], "EPSILONSTAR", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[2]->match_history);
    
    addMatchRecord(u[2], "ALPHASTRIKE", RESULT_LOSS, 0,   2);
    addMatchRecord(u[2], "BotPlayer1",  RESULT_WIN,  500, 2);
    addMatchRecord(u[2], "DELTABLITZ",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[2]->match_history);
    
    addMatchRecord(u[2], "BETAFORCE",   RESULT_WIN,  500, 3);
    addMatchRecord(u[2], "EPSILONSTAR", RESULT_DRAW, 250, 3);
    addMatchRecord(u[2], "ALPHASTRIKE", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[2]->match_history);
    
    addMatchRecord(u[2], "BotPlayer2",  RESULT_DRAW, 250, 4);
    addMatchRecord(u[2], "BETAFORCE",   RESULT_WIN,  500, 4);
    resetWeeklyStats(u[2]->match_history);
    
    addMatchRecord(u[2], "DELTABLITZ",  RESULT_LOSS, 0,   5);
    addMatchRecord(u[2], "BotPlayer3",  RESULT_LOSS, 0,   5);

    addMatchRecord(u[3], "ALPHASTRIKE", RESULT_DRAW, 250, 1);
    addMatchRecord(u[3], "BETAFORCE",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[3], "GAMMAKNIGHT", RESULT_LOSS, 0,   1);
    addMatchRecord(u[3], "EPSILONSTAR", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[3]->match_history);
    
    addMatchRecord(u[3], "BETAFORCE",   RESULT_LOSS, 0,   2);
    addMatchRecord(u[3], "GAMMAKNIGHT", RESULT_LOSS, 0,   2);
    addMatchRecord(u[3], "BotPlayer1",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[3]->match_history);
    
    addMatchRecord(u[3], "ALPHASTRIKE", RESULT_LOSS, 0,   3);
    resetWeeklyStats(u[3]->match_history);
    
    addMatchRecord(u[3], "BotPlayer2",  RESULT_WIN,  500, 4);
    addMatchRecord(u[3], "EPSILONSTAR", RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[3]->match_history);
    
    addMatchRecord(u[3], "BETAFORCE",   RESULT_LOSS, 0,   5);

    addMatchRecord(u[4], "ALPHASTRIKE", RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "BETAFORCE",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "GAMMAKNIGHT", RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "DELTABLITZ",  RESULT_LOSS, 0,   1);
    resetWeeklyStats(u[4]->match_history);
    
    addMatchRecord(u[4], "BotPlayer1",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[4]->match_history);
    
    addMatchRecord(u[4], "ALPHASTRIKE", RESULT_LOSS, 0,   3);
    addMatchRecord(u[4], "GAMMAKNIGHT", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[4]->match_history);
    
    addMatchRecord(u[4], "DELTABLITZ",  RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[4]->match_history);
    
    addMatchRecord(u[4], "BETAFORCE",   RESULT_LOSS, 0,   5);

    addMatchRecord(u[5], "BotPlayer1",  RESULT_WIN,  500, 1);
    addMatchRecord(u[6], "BotPlayer2",  RESULT_WIN,  500, 1);
    addMatchRecord(u[7], "BotPlayer3",  RESULT_WIN,  500, 1);
    addMatchRecord(u[8], "BotPlayer1",  RESULT_WIN,  500, 1);
    addMatchRecord(u[9], "BotPlayer2",  RESULT_WIN,  500, 1);
    addMatchRecord(u[10], "BotPlayer1", RESULT_WIN,  500, 1);
    addMatchRecord(u[11], "BotPlayer2", RESULT_WIN,  500, 1);
    addMatchRecord(u[12], "BotPlayer3", RESULT_WIN,  500, 1);
    addMatchRecord(u[13], "BotPlayer1", RESULT_LOSS, 0, 1);
    addMatchRecord(u[14], "BotPlayer2", RESULT_LOSS, 0, 1);
    addMatchRecord(u[15], "BotPlayer3", RESULT_LOSS, 0, 1);
    addMatchRecord(u[16], "BotPlayer1", RESULT_DRAW, 250, 1);
    addMatchRecord(u[17], "BotPlayer2", RESULT_DRAW, 250, 1);

    for (i = 0; i < 18; i++) {
        if (u[i] == NULL) continue;

        if (u[i]->match_history) {
            u[i]->match_history->current_week = 6;
            u[i]->match_history->week_matches = 0;
            u[i]->match_history->week_wins = 0;
        }

        addToLeaderboard(&g_leaderboard, u[i]->id, u[i]->name,
                         u[i]->total_wins, u[i]->total_losses, u[i]->total_draws,
                         getWinPercentage(u[i]->match_history), getWeeklyWinPercentage(u[i]->match_history), u[i]->level);
    }
    updateAllGlobalLevels();

    g_current_week = 6;

    printf("18 predefined users loaded with match history and leaderboard entries.\n");
}

static void simulateMatch(UserNode* u1, UserNode* u2) {
    int rating1 = getSquadAverageRating(u1 -> squad);
    int rating2 = getSquadAverageRating(u2 -> squad);
    if (rating2 == 0) rating2 = rating1 + (rand() % 11) - 5; // Fallback for bots

    int r1 = rating1 + (rand() % 21) - 10;
    int r2 = rating2 + (rand() % 21) - 10;

    int goals1 = 0, goals2 = 0;
    if (r1 > r2) {
        goals1 = (rand() % 3) + 1;
        goals2 = rand() % goals1;
    } else if (r2 > r1) {
        goals2 = (rand() % 3) + 1;
        goals1 = rand() % goals2;
    } else {
        goals1 = goals2 = rand() % 3;
    }

    int shots1 = goals1 + (rand() % 6);
    int shots2 = goals2 + (rand() % 6);
    int assists1 = goals1 > 0 ? (rand() % goals1) : 0;
    int assists2 = goals2 > 0 ? (rand() % goals2) : 0;
    int fouls1 = rand() % 6;
    int fouls2 = rand() % 6;
    int offsides1 = rand() % 4;
    int offsides2 = rand() % 4;
    int yellow1 = rand() % 3;
    int yellow2 = rand() % 3;
    int red1 = (rand() % 10) == 0 ? 1 : 0;
    int red2 = (rand() % 10) == 0 ? 1 : 0;

    printf("\n=========================================\n");
    printf("               SCORECARD                 \n");
    printf("=========================================\n");
    printf("%15s  %2d - %-2d  %-15s\n", u1->name, goals1, goals2, u2->name);    
    printf("-----------------------------------------\n");
    printf("%15d   Shots   %-15d\n", shots1, shots2);
    printf("%15d  Assists  %-15d\n", assists1, assists2);
    printf("%15d   Fouls   %-15d\n", fouls1, fouls2);
    printf("%15d Offsides  %-15d\n", offsides1, offsides2);
    printf("=========================================\n");

    char result1, result2;
    int  coins1 = 0, coins2 = 0;

    if (goals1 > goals2) {
        result1 = RESULT_WIN; result2 = RESULT_LOSS;
        coins1 = 500; coins2 = 0;
        printf("Result: %s WINS!\n", u1 -> name);
        u1 -> total_wins++; u2 -> total_losses++;
    } else if (goals2 > goals1) {
        result1 = RESULT_LOSS; result2 = RESULT_WIN;
        coins1 = 0; coins2 = 500;
        printf("Result: %s WINS!\n", u2 -> name);
        u2 -> total_wins++; u1 -> total_losses++;
    } else {
        result1 = RESULT_DRAW; result2 = RESULT_DRAW;
        coins1 = 250; coins2 = 250;
        printf("Result: DRAW!\n");
        u1 -> total_draws++; u2 -> total_draws++;
    }

    u1 -> coins += coins1;
    u2 -> coins += coins2;
    printf("%s earned %d coins. %s earned %d coins.\n",
           u1 -> name, coins1, u2 -> name, coins2);

    pushMatch(u1 -> match_history, u2 -> name, result1, coins1, g_current_week,
              goals1, goals2, shots1, shots2, assists1, assists2,
              fouls1, fouls2, offsides1, offsides2,
              red1, red2, yellow1, yellow2);
    pushMatch(u2 -> match_history, u1 -> name, result2, coins2, g_current_week,
              goals2, goals1, shots2, shots1, assists2, assists1,
              fouls2, fouls1, offsides2, offsides1,
              red2, red1, yellow2, yellow1);

    updateLeaderboardEntry(&g_leaderboard, u1 -> id,
                           u1 -> total_wins, u1 -> match_history->total_losses, u1 -> total_draws, 
                           getWinPercentage(u1->match_history), getWeeklyWinPercentage(u1->match_history), u1 -> level);
    updateLeaderboardEntry(&g_leaderboard, u2 -> id,
                           u2 -> total_wins, u2 -> match_history->total_losses, u2 -> total_draws, 
                           getWinPercentage(u2->match_history), getWeeklyWinPercentage(u2->match_history), u2 -> level);
    
    updateAllGlobalLevels();
}

static void inventoryMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Inventory Menu ---\n");
        printf("COINS: %d\n", user->coins);
        printf("1. View Inventory\n");
        printf("2. Search by Type\n");
        printf("3. Search by Rating Range\n");
        printf("4. Search by Name\n");
        printf("5. Sell Player\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayAllUserCards(user->squad, user->inventory);
        } else if (choice == 2) {
            printf("Enter type (FW/MF/DF/GK): ");
            readLine(input, sizeof(input));
            if(strcasecmp(input,"FW")!=0 && strcasecmp(input,"MF")!=0 && strcasecmp(input,"DF")!=0 && strcasecmp(input,"GK")!=0){
                  printf("invalid position\n");
                  continue;
              }
              searchInventoryByType(user -> inventory, input);
        } else if (choice == 3) {
            int min_r, max_r;
            printf("Enter min rating: ");
            if (scanf("%d", &min_r) != 1) { printf("invalid\n"); clearInput(); continue; }
            printf("Enter max rating: ");
            if (scanf("%d", &max_r) != 1) { printf("invalid\n"); clearInput(); continue; }
            clearInput();
            searchInventoryByRating(user -> inventory, min_r, max_r);
        } else if (choice == 4) {
            printf("Enter player name to search: ");
            readLine(input, sizeof(input));
            InventoryNode* found = searchInventoryByName(user -> inventory, input);
            if (found != NULL) {
                printf("\nPlayer found: %s | Type: %s | Rating: %d\n\n",
                       found->name, found->type, found->rating);
            } else {
                printf("Player '%s' not found in inventory.\n", input);
            }
        } else if (choice == 5) {
            
            if (user->inventory->count <= 11) {
                printf("You must have more than 11 players in your inventory to sell.\n");
                continue;
            }

            displayAllUserCards(user->squad, user->inventory);
            printf("Enter player name to sell (or '0' to go back): ");
            readLine(input, sizeof(input));
            if (strcmp(input, "0") == 0) {
                continue;
            }

            InventoryNode* found = searchInventoryByName(user -> inventory, input);
            if (found == NULL) {
                printf("Player '%s' not found in inventory.\n", input);
                continue;
            }

            if (isPlayerInSquad(user->squad, found->name)) {
                printf("Cannot sell '%s' because they are currently selected in your LineUp.\n", found->name);
                continue;
            }

            printf("Player: %s | Type: %s | Rating: %d\n",
                   found -> name, found -> type, found -> rating);
            printf("Enter selling price (coins): ");
            int price;
            if (scanf("%d", &price) != 1) { price = -1; clearInput(); }
            clearInput();
            if (price <= 0) {
                printf("Invalid price.\n");
                continue;
            }

            printf("Confirm sell '%s' for %d coins? (1=Yes/0=No): ", found->name, price);
            int confirm;
            if (scanf("%d", &confirm) != 1) { confirm = 0; clearInput(); }
            clearInput();

            if (confirm == 1) {
                
                InventoryNode* removed = removePlayerByName(user -> inventory, found->name);
                if (removed != NULL) {
                    
                    addToMarketplace(&g_market, removed -> name, removed -> type,
                                     removed -> rating, price);
                    printf("'%s' listed on marketplace for %d coins.\n",
                           removed -> name, price);
                    free(removed);
                }
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}

static void squadMenu(UserNode* user) {
    int choice;
    char existing[MAX_NAME_LEN], new_name[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Squad Menu ---\n");
        printf("1. LineUp\n");
        printf("2. Auto\n");
        printf("3. Swap Players\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displaySquad(user -> squad);
        } else if (choice == 2) {
            autoSelectBestSquad(user -> squad, user -> inventory);
        } else if (choice == 3) {
            displayAllUserCards(user -> squad, user -> inventory);
            printf("Enter first player name to swap (or '0' to go back): ");
            readLine(existing, sizeof(existing));
            if (strcmp(existing, "0") == 0) {
                continue;
            }
            printf("Enter second player name to swap (or '0' to go back): ");
            readLine(new_name, sizeof(new_name));
            if (strcmp(new_name, "0") == 0) {
                continue;
            }
            swapPlayerCards(user -> squad, user -> inventory, existing, new_name);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

static void marketplaceMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];
    char type[TYPE_LEN];
    int min_r, max_r, target_rating;

    while (true) {
        printf("\n--- Marketplace Menu ---\n");
        printf("COINS: %d\n", user->coins);
        printf("1. Purchase Player\n");
        printf("2. Search by Rating range\n");
        printf("3. Search by Rating range and Type\n");
        printf("4. Search by Name, Rating range and Type\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice >= 1 && choice <= 4) {
            type[0] = '\0';
            min_r = 0; max_r = 999;
            input[0] = '\0';

            if (choice == 1 || choice == 3 || choice == 4) {
                printf("Position/Type (FW/MF/DF/GK): ");
                readLine(type, sizeof(type));
            }
              if(strcasecmp(type,"FW")!=0 && strcasecmp(type,"MF")!=0 && strcasecmp(type,"DF")!=0 && strcasecmp(type,"GK")!=0 && strlen(type)>0){
                  printf("invalid position\n");
                  continue;
              }
              if(strcasecmp(type,"FW")!=0 && strcasecmp(type,"MF")!=0 && strcasecmp(type,"DF")!=0 && strcasecmp(type,"GK")!=0 && strlen(type)>0){
                  printf("invalid position\n");
                  continue;
              }
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
                printf("Min Rating: ");
                if (scanf("%d", &min_r) != 1) { printf("invalid\n"); clearInput(); continue; }
                printf("Max Rating: ");
                if (scanf("%d", &max_r) != 1) { printf("invalid\n"); clearInput(); continue; }
                clearInput();
            }
            if (choice == 4) {
                printf("Name: ");
                readLine(input, sizeof(input));
            }

            bool found = searchMarketAdvanced(&g_market, input, type, min_r, max_r);
            
            if (choice == 1) {
                if (!found) {
                    printf("No players matching the above criteria found\n");
                    continue;
                }
                
                MarketNode* matches[1000];
                int match_count = 0;
                MarketNode* curr = g_market.head;
                if (curr != NULL) {
                    for (int i = 0; i < g_market.count; i++) {
                        bool match = true;
                        if (curr->rating < min_r || curr->rating > max_r) match = false;
                        if (strlen(type) > 0 && strcasecmp(curr->type, type) != 0) match = false;
                        if (strlen(input) > 0 && !isPartialMatch(curr->name, input)) match = false;
                        
                        if (match) {
                            if (match_count < 1000) matches[match_count++] = curr;
                        }
                        curr = curr->next;
                    }
                }

                if (match_count > 0) {
                    int current_idx = 0;
                    while (true) {
                        printf("\nSelected Player: %-20s | %-4s | Rating: %-3d | Price: %-6d\n", 
                               matches[current_idx]->name, matches[current_idx]->type, 
                               matches[current_idx]->rating, matches[current_idx]->price);
                        printf("Options: [N]ext, [P]rev, [B]uy, [0] Back\n");
                        printf("Choice: ");
                        char nav_choice[256];
                        readLine(nav_choice, sizeof(nav_choice));
                        
                        if (strcasecmp(nav_choice, "0") == 0) {
                            break;
                        } else if (strcasecmp(nav_choice, "N") == 0 || strcasecmp(nav_choice, "Next") == 0) {
                            current_idx = (current_idx + 1) % match_count;
                        } else if (strcasecmp(nav_choice, "P") == 0 || strcasecmp(nav_choice, "Prev") == 0) {
                            current_idx = (current_idx - 1 + match_count) % match_count;
                        } else if (strcasecmp(nav_choice, "B") == 0 || strcasecmp(nav_choice, "Buy") == 0) {
                            MarketNode* card = matches[current_idx];
                            if (user->coins >= card->price) {
                                user->coins -= card->price;
                                addPlayer(user->inventory, card->name, card->type, card->rating, card->price);
                                
                                if (g_market.count == 1) {
                                    g_market.head = NULL;
                                } else {
                                    card->prev->next = card->next;
                                    card->next->prev = card->prev;
                                    if (card == g_market.head) g_market.head = card->next;
                                }
                                g_market.count--;
                                
                                printf("Successfully bought %s for %d coins!\n", card->name, card->price);
                                free(card);
                            } else {
                                printf("Not enough coins.\n");
                            }
                            break;
                        } else {
                            printf("Invalid choice.\n");
                        }
                    }
                }
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}
static void matchmakingMenu(UserNode* user) {
    int choice;
    static int bot_counter = 1;

    char botName[50];
    snprintf(botName, sizeof(botName), "Player %d", bot_counter);
    UserNode* current_bot = findUserByName(&g_registry, botName);
    if (current_bot == NULL) {
        current_bot = addUser(&g_registry, botName, "botpass");
        initUserModules(current_bot);
    }
    if (!isUserInQueue(&g_matchQueue, current_bot->id)) {
        enqueueUser(&g_matchQueue, current_bot->id, user->level);
    }
    
    while (true) {
        printf("\n--- Matchmaking Menu ---\n");
        printf("1. Find Opponent\n");
        printf("2. View Queue\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            if (user -> squad == NULL || getSquadAverageRating(user -> squad) == 0) {
                printf("Please set up your squad before finding an opponent.\n");
                continue;
            }

            printf("Match Ongoing.....\n");

            if (!isUserInQueue(&g_matchQueue, user->id)) {
                enqueueUser(&g_matchQueue, user->id, user->level);
            }

            int opp_id = findSameLevelOpponent(&g_matchQueue, user->id, user->level);
            if (opp_id != -1) {
                UserNode* opponent = findUserByID(&g_registry, opp_id);
                removeFromQueue(&g_matchQueue, user->id);
                removeFromQueue(&g_matchQueue, opp_id);

                printf("Match ended\n");
                simulateMatch(user, opponent);
                
                bot_counter++;
                snprintf(botName, sizeof(botName), "Player %d", bot_counter);
                current_bot = findUserByName(&g_registry, botName);
                if (current_bot == NULL) {
                    current_bot = addUser(&g_registry, botName, "botpass");
                    initUserModules(current_bot);
                }
                if (!isUserInQueue(&g_matchQueue, current_bot->id)) {
                    enqueueUser(&g_matchQueue, current_bot->id, user->level);
                }
                
            } else {
                printf("No opponent at your level found yet.\n");
            }
        } else if (choice == 2) {
            displayMatchQueue(&g_matchQueue, &g_registry);
        }
    }
}

static void leaderboardMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\n--- Leaderboard Menu ---\n");
        printf("1. Global Leaderboard\n");
        printf("2. My Rank\n");
        printf("3. Weekly Rewards\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayLeaderboard(&g_leaderboard, user != NULL ? user->id : -1);
        } else if (choice == 2) {
            int rank = getUserRank(&g_leaderboard, user -> id);
            if (rank == -1) {
                printf("You are not on the leaderboard yet.\n");
            } else {
                printf("\n%s FC26 Rank: #%d | Level: %s | Wins: %d\n",
                       user -> name, rank,
                       getLevelName(user -> level), user -> total_wins);
            }
        } else if (choice == 3) {
            printf("\n--- Your Weekly Rewards History ---\n");
            printf("%-6s | %-15s | %-6s | %-15s\n", "Week", "Level", "Rank", "Reward (Coins)");
            printf("-----------------------------------------------------\n");
            
            if (user->reward_history_count > 0) {
                for (int i = 0; i < user->reward_history_count; i++) {
                    printf("Week %-1d | %-15s | #%-5d | %d\n", 
                           user->past_weeks[i],
                           getLevelName(user->past_week_levels[i]), 
                           user->past_week_ranks[i], 
                           user->past_week_rewards[i]);
                }
            } else {
                printf("No weekly rewards history available.\n");
            }
            printf("-----------------------------------------------------\n");
        } else {
            printf("Invalid choice.\n");
        }
    }
}

static void matchHistoryMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\n--- Match History Menu ---\n");
        printf("1. Match History\n");
        printf("2. Statistics\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayMatchHistory(user -> match_history);
        } else if (choice == 2) {
            int cw = user->match_history->current_week;
            printf("\n(Current Week is %d. You can view stats from week %d to %d)\n", cw, cw-3 > 1 ? cw-3 : 1, cw);
            int week, match_no;
            printf("Enter Week: ");
            if (scanf("%d", &week) != 1) week = 0;
            clearInput();
            printf("Enter Match Number: ");
            if (scanf("%d", &match_no) != 1) match_no = 0;
            clearInput();
            displayStatistics(user->match_history, week, match_no, user->name);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

static void userDashboard(UserNode* user) {
    int choice;

    while (true) {
        printf("\n========================================\n");
        printf(" Welcome, %s!\n", user -> name);
        printf(" ID: %d | Level: %s | Coins: %d\n",
               user -> id, getLevelName(user -> level), user -> coins);
        printf(" Wins: %d | Losses: %d | Draws: %d\n",
               user -> total_wins, user -> total_losses, user -> total_draws);
        printf("========================================\n");
        printf("1. Inventory\n");
        printf("2. Squad\n");
        printf("3. Marketplace\n");
        printf("4. Matchmaking\n");
        printf("5. Leaderboard\n");
        printf("6. Match History\n");
        printf("0. Logout\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            printf("Logged out. Goodbye, %s!\n", user -> name);
            break;
        } else if (choice == 1) {
            inventoryMenu(user);
        } else if (choice == 2) {
            squadMenu(user);
        } else if (choice == 3) {
            marketplaceMenu(user);
        } else if (choice == 4) {
            matchmakingMenu(user);
        } else if (choice == 5) {
            leaderboardMenu(user);
        } else if (choice == 6) {
            matchHistoryMenu(user);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    initRegistry(&g_registry);
    initMarketplace(&g_market);
    seedMarketplace(&g_market);
    initLeaderboard(&g_leaderboard);
    initMatchQueue(&g_matchQueue);

    seedPredefinedUsers();

    printf("===========================================\n");
    printf("                  FC26                     \n");
    printf("===========================================\n");

    int choice;

    while (true) {
        printf("\n=== Main Menu ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. View All Users\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            printf("Exiting... Thank you for playing!\n");
            break;

        } else if (choice == 1) {
            
            char name[MAX_NAME_LEN];
            char password[MAX_NAME_LEN];
            printf("Enter username: ");
            readLine(name, sizeof(name));
            printf("Enter password: ");
            readLine(password, sizeof(password));

            UserNode* newUser = addUser(&g_registry, name, password);
            if (newUser != NULL) {
                initUserModules(newUser);
                  // Provide 3 weeks of default match history, 10 matches per week
                  const char* bot_names[10] = {
                      "ALPHASTRIKE", "BETAFORCE", "GAMMAKNIGHT", "DELTABLITZ", "EPSILONSTAR",
                      "ZETASHIELD", "ETASTORM", "THETAFANG", "IOTABRINGER", "KAPPANULL"
                  };
                  for (int w = g_current_week - 3; w < g_current_week; w++) {
                      if (w < 1) continue;
                      for (int m = 1; m <= 10; m++) {
                          const char* oppName = bot_names[rand() % 10];
                          char res = (m % 3 == 0) ? RESULT_WIN : ((m % 2 == 0) ? RESULT_DRAW : RESULT_LOSS);
                          int points = (res == RESULT_WIN) ? 500 : ((res == RESULT_DRAW) ? 250 : 0);
                          addMatchRecord(newUser, oppName, res, points, w);
                      }
                  }

                  // Reset leaderboard totals to keep the user effectively at 0/0/0
                  newUser->total_wins = 0;
                  newUser->total_losses = 0;
                  newUser->total_draws = 0;
                  newUser->coins = 15000;

                  if (newUser->match_history) {
                      newUser->match_history->current_week = g_current_week;
                      newUser->match_history->total_wins = 0;
                      newUser->match_history->total_losses = 0;
                      newUser->match_history->total_draws = 0;
                      newUser->match_history->total_matches = 0;
                      newUser->match_history->week_wins = 0;
                      newUser->match_history->week_matches = 0;
                  }
                addToLeaderboard(&g_leaderboard, newUser -> id,
                                   newUser -> name, newUser->total_wins, newUser->total_losses, newUser->total_draws, getWinPercentage(newUser->match_history), getWeeklyWinPercentage(newUser->match_history), newUser->level);
                printf("Registration successful!\n");
                printf("User ID: %d | Starting coins: %d\n",
                       newUser -> id, newUser -> coins);
                printf("You have been given 11 starter players in your inventory.\n");
            }

        } else if (choice == 2) {
            
            char name[MAX_NAME_LEN];
            char password[MAX_NAME_LEN];
            printf("Enter username: ");
            readLine(name, sizeof(name));
            printf("Enter password: ");
            readLine(password, sizeof(password));

            UserNode* user = findUserByName(&g_registry, name);
            if (user == NULL) {
                printf("User '%s' not found. Please register first.\n", name);
            } else if (strcmp(user->password, password) != 0) {
                printf("Incorrect password.\n");
            } else {
                printf("Login successful! Welcome back, %s.\n", user -> name);
                userDashboard(user);
            }

        } else if (choice == 3) {
            traverseUsers(&g_registry);

        } else {
            printf("Invalid choice.\n");
        }
    }

    UserNode* current = g_registry.head;
    while (current != NULL) {
        UserNode* next = current -> next;
        if (current -> inventory) {
            freeInventory(current -> inventory);
            free(current -> inventory);
        }
        if (current -> match_history) {
            freeMatchStack(current -> match_history);
            free(current -> match_history);
        }
        free(current -> squad);
        free(current);
        current = next;
    }
    freeMarketplace(&g_market);
    freeLeaderboard(&g_leaderboard);

    return 0;
}

