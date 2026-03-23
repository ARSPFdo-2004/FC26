import re

with open('main.c', 'r') as f:
    text = f.read()

market_menu_new = r'''static void marketplaceMenu(UserNode* user) {
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
}'''

text = re.sub(r'static void marketplaceMenu\(UserNode\* user\) \{.*?(?=\nstatic void matchmakingMenu)', market_menu_new, text, flags=re.DOTALL)

with open('main.c', 'w') as f:
    f.write(text)
