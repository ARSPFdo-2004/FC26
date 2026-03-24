#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

#include "user_registration.h"

void displayMatchQueue(MatchQueue* queue, UserRegistry* registry);

int findSameLevelOpponent(MatchQueue* queue, int user_id, int user_level);

void removeFromQueue(MatchQueue* queue, int user_id);

#endif 
