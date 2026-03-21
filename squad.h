#ifndef SQUAD_H
#define SQUAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "inventory.h"

#define SQUAD_SIZE 11
#define MAX_NAME_LEN 50

typedef struct SquadSlot {
    char position_type[4];   
    char player_name[MAX_NAME_LEN];
    int  player_rating;
    bool is_filled;
} SquadSlot;

typedef struct Squad {
    SquadSlot slots[SQUAD_SIZE];
} Squad;

void initSquad(Squad* squad);

void autoSelectBestSquad(Squad* squad, PlayerInventory* inv);

void replacePlayer(Squad* squad, PlayerInventory* inv,
                   const char* existing_name, const char* new_name);

void displaySquad(Squad* squad);

void findBestLineup(Squad* squad, PlayerInventory* inv);

int getSquadAverageRating(Squad* squad);

#endif 
