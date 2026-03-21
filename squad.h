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

/* A single slot in the squad */
typedef struct SquadSlot {
    char position_type[4];   /* Expected type: "FW","MF","DF","GK" */
    char player_name[MAX_NAME_LEN];
    int  player_rating;
    bool is_filled;
} SquadSlot;

/* The squad (11 slots: index 0-2 FW, 3-5 MF, 6-9 DF, 10 GK) */
typedef struct Squad {
    SquadSlot slots[SQUAD_SIZE];
} Squad;

/* INIT_SQUAD(squad : REFERENCE TO Squad) */
void initSquad(Squad* squad);

/* AUTO_SELECT_BEST_SQUAD(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory) */
void autoSelectBestSquad(Squad* squad, PlayerInventory* inv);

/* REPLACE_PLAYER(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory,
                  existing_name : STRING, new_name : STRING) */
void replacePlayer(Squad* squad, PlayerInventory* inv,
                   const char* existing_name, const char* new_name);

/* DISPLAY_SQUAD(squad : REFERENCE TO Squad) */
void displaySquad(Squad* squad);

/* FIND_BEST_LINEUP(squad : REFERENCE TO Squad, inv : REFERENCE TO PlayerInventory) */
void findBestLineup(Squad* squad, PlayerInventory* inv);

/* GET_SQUAD_AVERAGE_RATING(squad : REFERENCE TO Squad) RETURNS INTEGER */
int getSquadAverageRating(Squad* squad);

#endif /* SQUAD_H */
