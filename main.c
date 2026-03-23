#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "user_registration.h"
#include "inventory.h"
#include "squad.h"
#include "marketplace.h"
#include "matchmaking.h"
#include "leaderboard.h"
#include "matchhistory.h"

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

    pushMatch(user -> match_history, opponent, result, coins, week,
              goals_for, goals_against, shots, opp_shots, assists, opp_assists,
              fouls, opp_fouls, offsides, opp_offsides);
    user -> total_wins   = user -> match_history->total_wins;
    user -> total_losses = user -> match_history->total_losses;
    user -> total_draws  = user -> match_history->total_draws;
    user -> coins += coins;
}

static void seedPredefinedUsers(void) {
    static const char* names[5] = {
        "ALPHASTRIKE", "BETAFORCE", "GAMMAKNIGHT", "DELTABLITZ", "EPSILONSTAR"
    };
    UserNode* u[5];
    int i;

    for (i = 0; i < 5; i++) {
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

    for (i = 0; i < 5; i++) {
        if (u[i] == NULL) continue;
        updateUserLevel(u[i]);
        addToLeaderboard(&g_leaderboard, u[i]->id, u[i]->name,
                         u[i]->total_wins, u[i]->level);
    }

    g_current_week = 6;

    printf("5 predefined users loaded with match history and leaderboard entries.\n");
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
              fouls1, fouls2, offsides1, offsides2);
    pushMatch(u2 -> match_history, u1 -> name, result2, coins2, g_current_week,
              goals2, goals1, shots2, shots1, assists2, assists1,
              fouls2, fouls1, offsides2, offsides1);

    updateUserLevel(u1);
    updateUserLevel(u2);

    updateLeaderboardEntry(&g_leaderboard, u1 -> id,
                           u1 -> total_wins, u1 -> level);
    updateLeaderboardEntry(&g_leaderboard, u2 -> id,
                           u2 -> total_wins, u2 -> level);
}

static void inventoryMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Inventory Menu (%s) ---\n", user -> name);
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
            displayInventory(user -> inventory);
        } else if (choice == 2) {
            printf("Enter type (FW/MF/DF/GK): ");
            readLine(input, sizeof(input));
            searchInventoryByType(user -> inventory, input);
        } else if (choice == 3) {
            int min_r, max_r;
            printf("Enter min rating: ");
            if (scanf("%d", &min_r) != 1) { min_r = 0; clearInput(); }
            printf("Enter max rating: ");
            if (scanf("%d", &max_r) != 1) { max_r = 0; clearInput(); }
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

            displayInventory(user -> inventory);
            printf("Enter player name to sell: ");
            readLine(input, sizeof(input));

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
        printf("\n--- Squad Menu (%s) ---\n", user -> name);
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
            printf("Enter first player name to swap: ");
            readLine(existing, sizeof(existing));
            printf("Enter second player name to swap: ");
            readLine(new_name, sizeof(new_name));
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
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
                printf("Min Rating (0 for no limit): ");
                if (scanf("%d", &min_r) != 1) { min_r = 0; clearInput(); }
                printf("Max Rating (999 for no limit): ");
                if (scanf("%d", &max_r) != 1) { max_r = 999; clearInput(); }
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
                printf("Enter Player Name to purchase: ");
                readLine(input, sizeof(input));
                printf("Enter Player Rating: ");
                if (scanf("%d", &target_rating) != 1) { target_rating = 0; clearInput(); }
                clearInput();

                MarketNode* curr = g_market.head;
                MarketNode* card = NULL;
                if (curr != NULL) {
                    for (int i = 0; i < g_market.count; i++) {
                        if (strcasecmp(curr->name, input) == 0 && curr->rating == target_rating && strcasecmp(curr->type, type) == 0) {
                            card = curr; break;
                        }
                        curr = curr->next;
                    }
                }

                if (card == NULL) {
                    printf("Player not found with given Name, Rating and Type.\n");
                } else {
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
                        
                        printf("Successfully bought %s for %d coins!\n", input, card->price);
                        free(card);
                    } else {
                        printf("Not enough coins.\n");
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

    while (true) {
        printf("\n--- Matchmaking Menu (%s | Level: %s) ---\n",
               user -> name, getLevelName(user -> level));
        printf("1. Find Opponent\n");
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

            const char* botNames[3] = {"Player 1", "Player 2", "Player 3"};
            for (int i = 0; i < 3; i++) {
                UserNode* bot = findUserByName(&g_registry, botNames[i]);
                if (bot == NULL) {
                    bot = addUser(&g_registry, botNames[i], "botpass");
                    initUserModules(bot);
                }
                if (!isUserInQueue(&g_matchQueue, bot->id)) {
                    enqueueUser(&g_matchQueue, bot->id, user->level);
                }
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
            } else {
                printf("No opponent at your level found yet.\n");
            }
        } else {
            printf("Invalid choice.\n");
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
            displayLeaderboard(&g_leaderboard);
        } else if (choice == 2) {
            int rank = getUserRank(&g_leaderboard, user -> id);
            if (rank == -1) {
                printf("You are not on the leaderboard yet.\n");
            } else {
                printf("\n%s — Rank: #%d | Level: %s | Wins: %d\n",
                       user -> name, rank,
                       getLevelName(user -> level), user -> total_wins);
            }
        } else if (choice == 3) {
            printf("\n--- Your Weekly Rewards History ---\n");
            printf("%-6s | %-15s | %-6s | %-15s\n", "Week", "Level", "Rank", "Reward (Coins)");
            printf("-----------------------------------------------------\n");
            
            int current_rank = getUserRank(&g_leaderboard, user->id);
            if (current_rank <= 0) current_rank = 1;
            int sim_level = user->level;
            
            for (int i = 4; i >= 1; i--) {
                int wk = (g_current_week > i) ? (g_current_week - i) : i;
                int rank_sim = current_rank + (4 - i);
                if (rank_sim < 1) rank_sim = 1;
                
                int reward = getWeeklyReward(sim_level);
                printf("Week %-1d | %-15s | #%-5d | %d\n", wk, getLevelName(sim_level), rank_sim, reward);
                
                if (sim_level > 0 && rand() % 2 == 0) {
                    sim_level--;
                }
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
        printf("\n--- Match History Menu (%s) ---\n", user -> name);
        printf("1. View Match History\n");
        printf("2. View Overall Win Percentage\n");
        printf("3. View This Week's Win Percentage\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayMatchHistory(user -> match_history);
        } else if (choice == 2) {
            printf("\nOverall win rate for %s: %.1f%% (%d/%d)\n",
                   user -> name,
                   getWinPercentage(user -> match_history),
                   user -> match_history->total_wins,
                   user -> match_history->total_matches);
        } else if (choice == 3) {
            printf("\nWeek %d win rate for %s: %.1f%% (%d/%d)\n",
                   user -> match_history->current_week,
                   user -> name,
                   getWeeklyWinPercentage(user -> match_history),
                   user -> match_history->week_wins,
                   user -> match_history->week_matches);
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
        printf("4. View Leaderboard\n");
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
                
                addToLeaderboard(&g_leaderboard, newUser -> id,
                                 newUser -> name, 0, 0);
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

        } else if (choice == 4) {
            displayLeaderboard(&g_leaderboard);

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
