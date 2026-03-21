#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Structure for a matchmaking queue node */
typedef struct MatchQueueNode {
    int user_id;
    int user_level;
    struct MatchQueueNode* next;
} MatchQueueNode;

/* Structure for the matchmaking queue (FIFO) */
typedef struct MatchQueue {
    MatchQueueNode* front;
    MatchQueueNode* rear;
    int size;
} MatchQueue;

/* INIT_MATCH_QUEUE(queue : REFERENCE TO MatchQueue) */
void initMatchQueue(MatchQueue* queue);

/* IS_QUEUE_EMPTY(queue : REFERENCE TO MatchQueue) RETURNS BOOLEAN */
bool isQueueEmpty(MatchQueue* queue);

/* ENQUEUE_USER(queue : REFERENCE TO MatchQueue, user_id, user_level : INTEGER) */
void enqueueUser(MatchQueue* queue, int user_id, int user_level);

/* DEQUEUE_USER(queue : REFERENCE TO MatchQueue) RETURNS INTEGER (user_id, or -1) */
int dequeueUser(MatchQueue* queue);

/* PEEK_QUEUE(queue : REFERENCE TO MatchQueue) RETURNS INTEGER (user_id, or -1) */
int peekQueue(MatchQueue* queue);

/* IS_USER_IN_QUEUE(queue : REFERENCE TO MatchQueue, user_id : INTEGER)
   RETURNS BOOLEAN */
bool isUserInQueue(MatchQueue* queue, int user_id);

/* DISPLAY_QUEUE(queue : REFERENCE TO MatchQueue) */
void displayMatchQueue(MatchQueue* queue);

/* FIND_SAME_LEVEL_OPPONENT(queue : REFERENCE TO MatchQueue, user_id, user_level : INTEGER)
   RETURNS INTEGER (opponent_id, or -1 if none) */
int findSameLevelOpponent(MatchQueue* queue, int user_id, int user_level);

/* REMOVE_FROM_QUEUE(queue : REFERENCE TO MatchQueue, user_id : INTEGER) */
void removeFromQueue(MatchQueue* queue, int user_id);

#endif /* MATCHMAKING_H */
