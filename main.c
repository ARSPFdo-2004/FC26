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

/* ======================================================
 * Global state
 * ====================================================== */
static UserRegistry  g_registry;
static Marketplace   g_market;
static Leaderboard   g_leaderboard;
static MatchQueue    g_matchQueue;
static int           g_current_week = 1;

/* ======================================================
 * Helper utilities
 * ====================================================== */

/* Clear input buffer */
static void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* Read a line safely */
static void readLine(char* buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        /* Strip trailing newline */
        int len = (int)strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    }
}

/* ======================================================
 * Initialize a new user's inventory, squad, match history
 * ====================================================== */
static void initUserModules(UserNode* user) {
    /* Inventory */
    user->inventory = (PlayerInventory*)malloc(sizeof(PlayerInventory));
    if (user->inventory == NULL) { printf("Memory error\n"); exit(1); }
    initInventory(user->inventory);
    giveInitialPlayers(user->inventory, user->id);

    /* Squad */
    user->squad = (Squad*)malloc(sizeof(Squad));
    if (user->squad == NULL) { printf("Memory error\n"); exit(1); }
    initSquad(user->squad);

    /* Match history */
    user->match_history = (MatchStack*)malloc(sizeof(MatchStack));
    if (user->match_history == NULL) { printf("Memory error\n"); exit(1); }
    initMatchStack(user->match_history);
}

/* ======================================================
 * Helper: push a match record and keep UserNode totals in sync
 * ====================================================== */
static void addMatchRecord(UserNode* user, const char* opponent,
                           char result, int coins, int week) {
    pushMatch(user->match_history, opponent, result, coins, week);
    user->total_wins   = user->match_history->total_wins;
    user->total_losses = user->match_history->total_losses;
    user->total_draws  = user->match_history->total_draws;
    user->coins += coins;
}

/* ======================================================
 * Seed 5 predefined users with usernames, match history,
 * and leaderboard entries
 * ====================================================== */
static void seedPredefinedUsers(void) {
    static const char* names[5] = {
        "AlphaStrike", "BetaForce", "GammaKnight", "DeltaBlitz", "EpsilonStar"
    };
    UserNode* u[5];
    int i;

    /* Create users */
    for (i = 0; i < 5; i++) {
        u[i] = addUser(&g_registry, names[i]);
        if (u[i] == NULL) {
            u[i] = findUserByName(&g_registry, names[i]);
            continue;
        }
        initUserModules(u[i]);
    }

    /* --------------------------------------------------
     * AlphaStrike — 15W  3L  2D  (20 matches, 5 weeks)
     * -------------------------------------------------- */
    /* Week 1 */
    addMatchRecord(u[0], "BetaForce",   RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "GammaKnight", RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "EpsilonStar", RESULT_WIN,  500, 1);
    addMatchRecord(u[0], "DeltaBlitz",  RESULT_DRAW, 250, 1);
    resetWeeklyStats(u[0]->match_history);
    /* Week 2 */
    addMatchRecord(u[0], "BotPlayer1",  RESULT_WIN,  500, 2);
    addMatchRecord(u[0], "GammaKnight", RESULT_WIN,  500, 2);
    addMatchRecord(u[0], "BetaForce",   RESULT_LOSS, 0,   2);
    addMatchRecord(u[0], "BotPlayer2",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[0]->match_history);
    /* Week 3 */
    addMatchRecord(u[0], "DeltaBlitz",  RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "EpsilonStar", RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "BotPlayer3",  RESULT_WIN,  500, 3);
    addMatchRecord(u[0], "GammaKnight", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[0]->match_history);
    /* Week 4 */
    addMatchRecord(u[0], "BetaForce",   RESULT_WIN,  500, 4);
    addMatchRecord(u[0], "BotPlayer1",  RESULT_LOSS, 0,   4);
    addMatchRecord(u[0], "DeltaBlitz",  RESULT_WIN,  500, 4);
    addMatchRecord(u[0], "EpsilonStar", RESULT_WIN,  500, 4);
    resetWeeklyStats(u[0]->match_history);
    /* Week 5 */
    addMatchRecord(u[0], "BotPlayer2",  RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "GammaKnight", RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "BotPlayer3",  RESULT_WIN,  500, 5);
    addMatchRecord(u[0], "BetaForce",   RESULT_LOSS, 0,   5);

    /* --------------------------------------------------
     * BetaForce — 10W  5L  2D  (17 matches, 5 weeks)
     * -------------------------------------------------- */
    /* Week 1 */
    addMatchRecord(u[1], "AlphaStrike", RESULT_LOSS, 0,   1);
    addMatchRecord(u[1], "GammaKnight", RESULT_WIN,  500, 1);
    addMatchRecord(u[1], "DeltaBlitz",  RESULT_WIN,  500, 1);
    addMatchRecord(u[1], "EpsilonStar", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[1]->match_history);
    /* Week 2 */
    addMatchRecord(u[1], "AlphaStrike", RESULT_WIN,  500, 2);
    addMatchRecord(u[1], "BotPlayer1",  RESULT_DRAW, 250, 2);
    addMatchRecord(u[1], "DeltaBlitz",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[1]->match_history);
    /* Week 3 */
    addMatchRecord(u[1], "GammaKnight", RESULT_LOSS, 0,   3);
    addMatchRecord(u[1], "BotPlayer2",  RESULT_WIN,  500, 3);
    addMatchRecord(u[1], "EpsilonStar", RESULT_WIN,  500, 3);
    resetWeeklyStats(u[1]->match_history);
    /* Week 4 */
    addMatchRecord(u[1], "AlphaStrike", RESULT_LOSS, 0,   4);
    addMatchRecord(u[1], "BotPlayer3",  RESULT_WIN,  500, 4);
    addMatchRecord(u[1], "GammaKnight", RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[1]->match_history);
    /* Week 5 */
    addMatchRecord(u[1], "DeltaBlitz",  RESULT_WIN,  500, 5);
    addMatchRecord(u[1], "AlphaStrike", RESULT_LOSS, 0,   5);
    addMatchRecord(u[1], "EpsilonStar", RESULT_LOSS, 0,   5);
    addMatchRecord(u[1], "BotPlayer1",  RESULT_WIN,  500, 5);

    /* --------------------------------------------------
     * GammaKnight — 6W  5L  3D  (14 matches, 5 weeks)
     * -------------------------------------------------- */
    /* Week 1 */
    addMatchRecord(u[2], "AlphaStrike", RESULT_LOSS, 0,   1);
    addMatchRecord(u[2], "BetaForce",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[2], "DeltaBlitz",  RESULT_WIN,  500, 1);
    addMatchRecord(u[2], "EpsilonStar", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[2]->match_history);
    /* Week 2 */
    addMatchRecord(u[2], "AlphaStrike", RESULT_LOSS, 0,   2);
    addMatchRecord(u[2], "BotPlayer1",  RESULT_WIN,  500, 2);
    addMatchRecord(u[2], "DeltaBlitz",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[2]->match_history);
    /* Week 3 */
    addMatchRecord(u[2], "BetaForce",   RESULT_WIN,  500, 3);
    addMatchRecord(u[2], "EpsilonStar", RESULT_DRAW, 250, 3);
    addMatchRecord(u[2], "AlphaStrike", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[2]->match_history);
    /* Week 4 */
    addMatchRecord(u[2], "BotPlayer2",  RESULT_DRAW, 250, 4);
    addMatchRecord(u[2], "BetaForce",   RESULT_WIN,  500, 4);
    resetWeeklyStats(u[2]->match_history);
    /* Week 5 */
    addMatchRecord(u[2], "DeltaBlitz",  RESULT_LOSS, 0,   5);
    addMatchRecord(u[2], "BotPlayer3",  RESULT_LOSS, 0,   5);

    /* --------------------------------------------------
     * DeltaBlitz — 3W  6L  2D  (11 matches, 5 weeks)
     * -------------------------------------------------- */
    /* Week 1 */
    addMatchRecord(u[3], "AlphaStrike", RESULT_DRAW, 250, 1);
    addMatchRecord(u[3], "BetaForce",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[3], "GammaKnight", RESULT_LOSS, 0,   1);
    addMatchRecord(u[3], "EpsilonStar", RESULT_WIN,  500, 1);
    resetWeeklyStats(u[3]->match_history);
    /* Week 2 */
    addMatchRecord(u[3], "BetaForce",   RESULT_LOSS, 0,   2);
    addMatchRecord(u[3], "GammaKnight", RESULT_LOSS, 0,   2);
    addMatchRecord(u[3], "BotPlayer1",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[3]->match_history);
    /* Week 3 */
    addMatchRecord(u[3], "AlphaStrike", RESULT_LOSS, 0,   3);
    resetWeeklyStats(u[3]->match_history);
    /* Week 4 */
    addMatchRecord(u[3], "BotPlayer2",  RESULT_WIN,  500, 4);
    addMatchRecord(u[3], "EpsilonStar", RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[3]->match_history);
    /* Week 5 */
    addMatchRecord(u[3], "BetaForce",   RESULT_LOSS, 0,   5);

    /* --------------------------------------------------
     * EpsilonStar — 1W  6L  2D  (9 matches, 5 weeks)
     * -------------------------------------------------- */
    /* Week 1 */
    addMatchRecord(u[4], "AlphaStrike", RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "BetaForce",   RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "GammaKnight", RESULT_LOSS, 0,   1);
    addMatchRecord(u[4], "DeltaBlitz",  RESULT_LOSS, 0,   1);
    resetWeeklyStats(u[4]->match_history);
    /* Week 2 */
    addMatchRecord(u[4], "BotPlayer1",  RESULT_WIN,  500, 2);
    resetWeeklyStats(u[4]->match_history);
    /* Week 3 */
    addMatchRecord(u[4], "AlphaStrike", RESULT_LOSS, 0,   3);
    addMatchRecord(u[4], "GammaKnight", RESULT_DRAW, 250, 3);
    resetWeeklyStats(u[4]->match_history);
    /* Week 4 */
    addMatchRecord(u[4], "DeltaBlitz",  RESULT_DRAW, 250, 4);
    resetWeeklyStats(u[4]->match_history);
    /* Week 5 */
    addMatchRecord(u[4], "BetaForce",   RESULT_LOSS, 0,   5);

    /* Update levels and populate leaderboard */
    for (i = 0; i < 5; i++) {
        if (u[i] == NULL) continue;
        updateUserLevel(u[i]);
        addToLeaderboard(&g_leaderboard, u[i]->id, u[i]->name,
                         u[i]->total_wins, u[i]->level);
    }

    /* Advance global week counter to reflect pre-seeded history */
    g_current_week = 6;

    printf("5 predefined users loaded with match history and leaderboard entries.\n");
}

/* ======================================================
 * Simulate a match between two users
 * ====================================================== */
static void simulateMatch(UserNode* u1, UserNode* u2) {
    int rating1 = getSquadAverageRating(u1->squad);
    int rating2 = getSquadAverageRating(u2->squad);

    /* Add randomness ±10 */
    int r1 = rating1 + (rand() % 21) - 10;
    int r2 = rating2 + (rand() % 21) - 10;

    printf("\n=== Match: %s (Rating~%d) vs %s (Rating~%d) ===\n",
           u1->name, rating1, u2->name, rating2);

    char result1, result2;
    int  coins1, coins2;

    if (r1 > r2) {
        result1 = RESULT_WIN;
        result2 = RESULT_LOSS;
        coins1  = 500;
        coins2  = 0;
        printf("Result: %s WINS!\n", u1->name);
        u1->total_wins++;
        u2->total_losses++;
    } else if (r2 > r1) {
        result1 = RESULT_LOSS;
        result2 = RESULT_WIN;
        coins1  = 0;
        coins2  = 500;
        printf("Result: %s WINS!\n", u2->name);
        u2->total_wins++;
        u1->total_losses++;
    } else {
        result1 = RESULT_DRAW;
        result2 = RESULT_DRAW;
        coins1  = 250;
        coins2  = 250;
        printf("Result: DRAW!\n");
        u1->total_draws++;
        u2->total_draws++;
    }

    u1->coins += coins1;
    u2->coins += coins2;
    printf("%s earned %d coins. %s earned %d coins.\n",
           u1->name, coins1, u2->name, coins2);

    /* Record in match history */
    pushMatch(u1->match_history, u2->name, result1, coins1, g_current_week);
    pushMatch(u2->match_history, u1->name, result2, coins2, g_current_week);

    /* Update levels */
    updateUserLevel(u1);
    updateUserLevel(u2);

    /* Update leaderboard */
    updateLeaderboardEntry(&g_leaderboard, u1->id,
                           u1->total_wins, u1->level);
    updateLeaderboardEntry(&g_leaderboard, u2->id,
                           u2->total_wins, u2->level);
}

/* ======================================================
 * INVENTORY MENU
 * ====================================================== */
static void inventoryMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Inventory Menu (%s) ---\n", user->name);
        printf("1. View Inventory\n");
        printf("2. Search by Type\n");
        printf("3. Search by Rating Range\n");
        printf("4. Sell Player (move to Marketplace)\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayInventory(user->inventory);
        } else if (choice == 2) {
            printf("Enter type (FW/MF/DF/GK): ");
            readLine(input, sizeof(input));
            searchInventoryByType(user->inventory, input);
        } else if (choice == 3) {
            int min_r, max_r;
            printf("Enter min rating: ");
            if (scanf("%d", &min_r) != 1) { min_r = 0; clearInput(); }
            printf("Enter max rating: ");
            if (scanf("%d", &max_r) != 1) { max_r = 0; clearInput(); }
            clearInput();
            searchInventoryByRating(user->inventory, min_r, max_r);
        } else if (choice == 4) {
            /* Sell a player */
            displayInventory(user->inventory);
            printf("Enter player name to sell: ");
            readLine(input, sizeof(input));

            InventoryNode* found = searchInventoryByName(user->inventory, input);
            if (found == NULL) {
                printf("Player '%s' not found in inventory.\n", input);
                continue;
            }

            printf("Player: %s | Type: %s | Rating: %d\n",
                   found->name, found->type, found->rating);
            printf("Enter selling price (coins): ");
            int price;
            if (scanf("%d", &price) != 1) { price = -1; clearInput(); }
            clearInput();
            if (price <= 0) {
                printf("Invalid price.\n");
                continue;
            }

            printf("Confirm sell '%s' for %d coins? (1=Yes/0=No): ", input, price);
            int confirm;
            if (scanf("%d", &confirm) != 1) { confirm = 0; clearInput(); }
            clearInput();

            if (confirm == 1) {
                /* Remove from inventory */
                InventoryNode* removed = removePlayerByName(user->inventory, input);
                if (removed != NULL) {
                    /* Add to marketplace */
                    addToMarketplace(&g_market, removed->name, removed->type,
                                     removed->rating, price, user->id);
                    printf("'%s' listed on marketplace for %d coins.\n",
                           removed->name, price);
                    free(removed);
                }
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * SQUAD MENU
 * ====================================================== */
static void squadMenu(UserNode* user) {
    int choice;
    char existing[MAX_NAME_LEN], new_name[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Squad Menu (%s) ---\n", user->name);
        printf("1. View Squad\n");
        printf("2. Auto-Select Best Squad\n");
        printf("3. Replace Player\n");
        printf("4. Find Best Lineup\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displaySquad(user->squad);
        } else if (choice == 2) {
            autoSelectBestSquad(user->squad, user->inventory);
        } else if (choice == 3) {
            displaySquad(user->squad);
            printf("Enter existing player name in squad: ");
            readLine(existing, sizeof(existing));
            printf("Enter new player name from inventory: ");
            readLine(new_name, sizeof(new_name));
            replacePlayer(user->squad, user->inventory, existing, new_name);
        } else if (choice == 4) {
            findBestLineup(user->squad, user->inventory);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * MARKETPLACE MENU
 * ====================================================== */
static void marketplaceMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];

    while (true) {
        printf("\n--- Marketplace Menu (%s | Coins: %d) ---\n",
               user->name, user->coins);
        printf("1. View All Listings\n");
        printf("2. Search by Name\n");
        printf("3. Search by Type\n");
        printf("4. Buy Player\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayMarketplace(&g_market);
        } else if (choice == 2) {
            printf("Enter player name: ");
            readLine(input, sizeof(input));
            MarketNode* found = searchMarketByName(&g_market, input);
            if (found == NULL) {
                printf("Player '%s' not found.\n", input);
            } else {
                printf("\nFound: %s | Type: %s | Rating: %d | Price: %d | Seller ID: %d\n",
                       found->name, found->type,
                       found->rating, found->price, found->seller_id);
            }
        } else if (choice == 3) {
            printf("Enter type (FW/MF/DF/GK): ");
            readLine(input, sizeof(input));
            searchMarketByType(&g_market, input);
        } else if (choice == 4) {
            displayMarketplace(&g_market);
            printf("Enter player name to buy: ");
            readLine(input, sizeof(input));

            MarketNode* card = searchMarketByName(&g_market, input);
            if (card == NULL) {
                printf("Player '%s' not found in marketplace.\n", input);
                continue;
            }

            if (card->seller_id == user->id) {
                printf("You cannot buy your own listing.\n");
                continue;
            }

            if (user->coins < card->price) {
                printf("Insufficient coins. You have %d, need %d.\n",
                       user->coins, card->price);
                continue;
            }

            printf("Buy '%s' (Type:%s Rating:%d) for %d coins? (1=Yes/0=No): ",
                   card->name, card->type, card->rating, card->price);
            int confirm;
            if (scanf("%d", &confirm) != 1) { confirm = 0; clearInput(); }
            clearInput();

            if (confirm == 1) {
                int price     = card->price;
                int seller_id = card->seller_id;

                /* Remove from marketplace */
                MarketNode* bought = removeFromMarketplace(&g_market, input);
                if (bought != NULL) {
                    /* Add to buyer's inventory */
                    addPlayer(user->inventory, bought->name,
                              bought->type, bought->rating, bought->price);

                    /* Deduct from buyer */
                    user->coins -= price;

                    /* Credit to seller */
                    UserNode* seller = findUserByID(&g_registry, seller_id);
                    if (seller != NULL) {
                        seller->coins += price;
                        printf("Seller %s received %d coins.\n",
                               seller->name, price);
                    }

                    printf("Purchased '%s' for %d coins. Your balance: %d\n",
                           bought->name, price, user->coins);
                    free(bought);
                }
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * MATCHMAKING MENU
 * ====================================================== */
static void matchmakingMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\n--- Matchmaking Menu (%s | Level: %s) ---\n",
               user->name, getLevelName(user->level));
        printf("1. Join Matchmaking Queue\n");
        printf("2. View Queue\n");
        printf("3. Process Matches (find opponent & play)\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            if (isUserInQueue(&g_matchQueue, user->id)) {
                printf("You are already in the queue.\n");
            } else if (user->squad == NULL ||
                       getSquadAverageRating(user->squad) == 0) {
                printf("Please set up your squad before joining matchmaking.\n");
            } else {
                enqueueUser(&g_matchQueue, user->id, user->level);
                printf("Joined matchmaking queue. Position: %d\n",
                       g_matchQueue.size);
            }
        } else if (choice == 2) {
            displayMatchQueue(&g_matchQueue);
        } else if (choice == 3) {
            if (!isUserInQueue(&g_matchQueue, user->id)) {
                printf("Join the queue first.\n");
                continue;
            }

            int opp_id = findSameLevelOpponent(&g_matchQueue, user->id,
                                               user->level);
            if (opp_id == -1) {
                printf("No opponent at your level (%s) found yet.\n",
                       getLevelName(user->level));
                printf("Waiting in queue... (try again later)\n");
                continue;
            }

            UserNode* opponent = findUserByID(&g_registry, opp_id);
            if (opponent == NULL) {
                printf("Opponent data not found.\n");
                continue;
            }

            /* Remove both from queue */
            removeFromQueue(&g_matchQueue, user->id);
            removeFromQueue(&g_matchQueue, opp_id);

            simulateMatch(user, opponent);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * LEADERBOARD MENU
 * ====================================================== */
static void leaderboardMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\n--- Leaderboard Menu ---\n");
        printf("1. View Global Leaderboard\n");
        printf("2. View My Rank\n");
        printf("3. Distribute Weekly Rewards\n");
        printf("0. Back\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            displayLeaderboard(&g_leaderboard);
        } else if (choice == 2) {
            int rank = getUserRank(&g_leaderboard, user->id);
            if (rank == -1) {
                printf("You are not on the leaderboard yet.\n");
            } else {
                printf("\n%s — Rank: #%d | Level: %s | Wins: %d\n",
                       user->name, rank,
                       getLevelName(user->level), user->total_wins);
            }
        } else if (choice == 3) {
            printf("Distributing weekly rewards...\n");

            UserNode* current = g_registry.head;
            while (current != NULL) {
                int reward = getWeeklyReward(current->level);
                current->coins += reward;
                printf("  %s [%s] received %d coins as weekly reward.\n",
                       current->name, getLevelName(current->level), reward);

                /* Reset weekly match stats */
                if (current->match_history != NULL) {
                    resetWeeklyStats(current->match_history);
                }
                current = current->next;
            }

            g_current_week++;
            printf("Week %d rewards distributed!\n", g_current_week - 1);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * MATCH HISTORY MENU
 * ====================================================== */
static void matchHistoryMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\n--- Match History Menu (%s) ---\n", user->name);
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
            displayMatchHistory(user->match_history);
        } else if (choice == 2) {
            printf("\nOverall win rate for %s: %.1f%% (%d/%d)\n",
                   user->name,
                   getWinPercentage(user->match_history),
                   user->match_history->total_wins,
                   user->match_history->total_matches);
        } else if (choice == 3) {
            printf("\nWeek %d win rate for %s: %.1f%% (%d/%d)\n",
                   user->match_history->current_week,
                   user->name,
                   getWeeklyWinPercentage(user->match_history),
                   user->match_history->week_wins,
                   user->match_history->week_matches);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ======================================================
 * USER DASHBOARD (after login)
 * ====================================================== */
static void userDashboard(UserNode* user) {
    int choice;

    while (true) {
        printf("\n========================================\n");
        printf(" Welcome, %s!\n", user->name);
        printf(" ID: %d | Level: %s | Coins: %d\n",
               user->id, getLevelName(user->level), user->coins);
        printf(" Wins: %d | Losses: %d | Draws: %d\n",
               user->total_wins, user->total_losses, user->total_draws);
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
            printf("Logged out. Goodbye, %s!\n", user->name);
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

/* ======================================================
 * MAIN MENU
 * ====================================================== */
int main(void) {
    srand((unsigned int)time(NULL));

    /* Initialize global structures */
    initRegistry(&g_registry);
    initMarketplace(&g_market);
    seedMarketplace(&g_market);
    initLeaderboard(&g_leaderboard);
    initMatchQueue(&g_matchQueue);

    /* Load predefined users */
    seedPredefinedUsers();

    printf("===========================================\n");
    printf("   Football Card Game — DSA Group Project  \n");
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
            /* Register */
            char name[MAX_NAME_LEN];
            printf("Enter username: ");
            readLine(name, sizeof(name));

            UserNode* newUser = addUser(&g_registry, name);
            if (newUser != NULL) {
                initUserModules(newUser);
                /* Add to leaderboard */
                addToLeaderboard(&g_leaderboard, newUser->id,
                                 newUser->name, 0, 0);
                printf("Registration successful!\n");
                printf("User ID: %d | Starting coins: %d\n",
                       newUser->id, newUser->coins);
                printf("You have been given 11 starter players in your inventory.\n");
            }

        } else if (choice == 2) {
            /* Login */
            char name[MAX_NAME_LEN];
            printf("Enter username: ");
            readLine(name, sizeof(name));

            UserNode* user = findUserByName(&g_registry, name);
            if (user == NULL) {
                printf("User '%s' not found. Please register first.\n", name);
            } else {
                printf("Login successful! Welcome back, %s.\n", user->name);
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

    /* Cleanup */
    UserNode* current = g_registry.head;
    while (current != NULL) {
        UserNode* next = current->next;
        if (current->inventory) {
            freeInventory(current->inventory);
            free(current->inventory);
        }
        if (current->match_history) {
            freeMatchStack(current->match_history);
            free(current->match_history);
        }
        free(current->squad);
        free(current);
        current = next;
    }
    freeMarketplace(&g_market);
    freeLeaderboard(&g_leaderboard);

    return 0;
}
