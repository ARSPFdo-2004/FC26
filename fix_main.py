import re

with open('main.c', 'r') as f:
    text = f.read()

squad_menu_new = """static void squadMenu(UserNode* user) {
    int choice;
    char existing[MAX_NAME_LEN], new_name[MAX_NAME_LEN];

    while (true) {
        printf("\\\\n--- Squad Menu (%s) ---\\\\n", user->name);
        printf("1. LineUp\\\\n");
        printf("2. Auto\\\\n");
        printf("3. Swap Players\\\\n");
        printf("0. Back\\\\n");
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
            displayAllUserCards(user->squad, user->inventory);
            printf("Enter first player name to swap: ");
            readLine(existing, sizeof(existing));
            printf("Enter second player name to swap: ");
            readLine(new_name, sizeof(new_name));
            swapPlayerCards(user->squad, user->inventory, existing, new_name);
        } else {
            printf("Invalid choice.\\\\n");
        }
    }
}"""

text = re.sub(r'static void squadMenu\(UserNode\* user\) \{.*?(?=\nstatic void marketplaceMenu)', squad_menu_new + '\n', text, flags=re.DOTALL)


market_menu_new = """static void marketplaceMenu(UserNode* user) {
    int choice;
    char input[MAX_NAME_LEN];
    char type[TYPE_LEN];
    int min_r, max_r, target_rating;

    while (true) {
        printf("\\\\n--- Marketplace Menu ---\\\\n");
        printf("1. Purchase Player\\\\n");
        printf("2. Search by Rating range\\\\n");
        printf("3. Search by Rating range and Type\\\\n");
        printf("4. Search by Name, Rating range and Type\\\\n");
        printf("0. Back\\\\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice >= 1 && choice <= 4) {
            type[0] = '\\0';
            min_r = 0; max_r = 999;
            input[0] = '\\0';

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
                    printf("No players matching the above criteria found\\\\n");
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
                    printf("Player not found with given Name and Rating.\\\\n");
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
                        free(card);
                        printf("Successfully bought %s for %d coins!\\\\n", input, card->price);
                    } else {
                        printf("Not enough coins.\\\\n");
                    }
                }
            }
        } else {
            printf("Invalid choice.\\\\n");
        }
    }
}"""

text = re.sub(r'static void marketplaceMenu\(UserNode\* user\) \{.*?(?=\nstatic void matchmakingMenu)', market_menu_new + '\n', text, flags=re.DOTALL)


sim_match_new = """static void simulateMatch(UserNode* u1, UserNode* u2) {
    int rating1 = getSquadAverageRating(u1->squad);
    int rating2 = getSquadAverageRating(u2->squad);

    int r1 = rating1 + (rand() % 21) - 10;
    int r2 = rating2 + (rand() % 21) - 10;

    printf("\\\\n%s VS %s\\\\n", u1->name, u2->name);
    printf("Match Ongoing.....\\\\n");
    for(int i=0; i<30000000; i++); // tiny sleep simulation
    printf("Match ended.\\\\n\\\\n");
    
    int goals1 = rand() % 5;
    int goals2 = rand() % 5;
    if (r1 > r2 + 10) goals1 += 2;
    else if (r2 > r1 + 10) goals2 += 2;
    
    if (r1 > r2 && goals1 <= goals2) goals1 = goals2 + 1;
    if (r2 > r1 && goals2 <= goals1) goals2 = goals1 + 1;
    if (r1 == r2) goals1 = goals2;

    int shots1 = goals1 + rand() % 5 + 2;
    int shots2 = goals2 + rand() % 5 + 2;
    int assist1 = goals1 > 0 ? rand() % goals1 : 0;
    int assist2 = goals2 > 0 ? rand() % goals2 : 0;
    int fouls1 = rand() % 10;
    int fouls2 = rand() % 10;
    int off1 = rand() % 5;
    int off2 = rand() % 5;

    printf("=== Scorecard ===\\\\n");
    printf("%-15s %-5s %s\\\\n", u1->name, "", u2->name);
    printf("%-15d %-5s %d\\\\n", goals1, "Goals", goals2);
    printf("%-15d %-5s %d\\\\n", shots1, "Shots", shots2);
    printf("%-15d %-5s %d\\\\n", assist1, "Assts", assist2);
    printf("%-15d %-5s %d\\\\n", fouls1, "Fouls", fouls2);
    printf("%-15d %-5s %d\\\\n", off1, "Offs", off2);
    printf("=================\\\\n");

    char result1, result2;
    int  coins1, coins2;

    if (goals1 > goals2) {
        result1 = RESULT_WIN; result2 = RESULT_LOSS;
        coins1  = 500; coins2  = 0;
        printf("Result: %s WINS!\\\\n", u1->name);
        u1->total_wins++; u2->total_losses++;
    } else if (goals2 > goals1) {
        result1 = RESULT_LOSS; result2 = RESULT_WIN;
        coins1  = 0; coins2  = 500;
        printf("Result: %s WINS!\\\\n", u2->name);
        u2->total_wins++; u1->total_losses++;
    } else {
        result1 = RESULT_DRAW; result2 = RESULT_DRAW;
        coins1  = 250; coins2  = 250;
        printf("Result: DRAW!\\\\n");
        u1->total_draws++; u2->total_draws++;
    }

    u1->coins += coins1; u2->coins += coins2;
    updateUserLevel(u1); updateUserLevel(u2);

    addMatchRecord(u1, u2->name, result1, coins1, g_current_week);
    addMatchRecord(u2, u1->name, result2, coins2, g_current_week);
    
    updateLeaderboardNode(&g_leaderboard, u1->id, u1->name, u1->total_wins, u1->level);
    updateLeaderboardNode(&g_leaderboard, u2->id, u2->name, u2->total_wins, u2->level);
}"""

text = re.sub(r'static void simulateMatch\(UserNode\* u1, UserNode\* u2\) \{.*?(?=\nstatic void leaderboardMenu)', sim_match_new + '\n', text, flags=re.DOTALL)


match_menu_new = """static void matchmakingMenu(UserNode* user) {
    int choice;

    while (true) {
        printf("\\\\n--- Matchmaking Menu (%s | Level: %s) ---\\\\n",
               user->name, getLevelName(user->level));
        printf("1. Find Opponent\\\\n");
        printf("0. Back\\\\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = 0; clearInput(); }
        clearInput();

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            if (user->squad == NULL || getSquadAverageRating(user->squad) == 0) {
                printf("Please set up your squad before joining matchmaking.\\\\n");
                continue;
            }
            if (!isUserInQueue(&g_matchQueue, user->id)) {
                enqueueUser(&g_matchQueue, user->id, user->level);
            }
            
            int opp_id = -1;
            MatchQueueNode* current = g_matchQueue.front;
            while (current != NULL) {
                if (current->user_id != user->id) {
                    opp_id = current->user_id;
                    break;
                }
                current = current->next;
            }
            
            if (opp_id == -1) {
                printf("Waiting in queue... (No opponents available)\\\\n");
            } else {
                UserNode* opponent = findUserByID(&g_registry, opp_id);
                if (opponent != NULL) {
                    removeFromQueue(&g_matchQueue, user->id);
                    removeFromQueue(&g_matchQueue, opp_id);
                    simulateMatch(user, opponent);
                }
            }
        } else {
            printf("Invalid choice.\\\\n");
        }
    }
}"""

text = re.sub(r'static void matchmakingMenu\(UserNode\* user\) \{.*?(?=\nstatic void emulateMatch|static void emulate|static void leader|static void simulateMatch|static void main)', match_menu_new + '\n', text, flags=re.DOTALL)

seed_new = """    UserNode* p1 = addUser(&g_registry, "Player 1", "pass"); initUserModules(p1); enqueueUser(&g_matchQueue, p1->id, 0); autoSelectBestSquad(p1->squad, p1->inventory);
    UserNode* p2 = addUser(&g_registry, "Player 2", "pass"); initUserModules(p2); enqueueUser(&g_matchQueue, p2->id, 0); autoSelectBestSquad(p2->squad, p2->inventory);
    UserNode* p3 = addUser(&g_registry, "Player 3", "pass"); initUserModules(p3); enqueueUser(&g_matchQueue, p3->id, 0); autoSelectBestSquad(p3->squad, p3->inventory);
    
    printf("5 predefined users loaded with match history and leaderboard entries.\\\\n");
"""

text = re.sub(r'printf\("5 predefined users loaded with match history and leaderboard entries\.\\\\n"\);\n', seed_new, text)

with open('main.c', 'w') as f:
    f.write(text)

