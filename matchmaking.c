#include "matchmaking.h"

void initMatchQueue(MatchQueue* queue) {
    queue->front = NULL;
    queue->rear  = NULL;
    queue->size  = 0;
}

bool isQueueEmpty(MatchQueue* queue) {
    return queue->front == NULL;
}

void enqueueUser(MatchQueue* queue, int user_id, int user_level) {
    MatchQueueNode* newNode = (MatchQueueNode*)malloc(sizeof(MatchQueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode->user_id    = user_id;
    newNode->user_level = user_level;
    newNode->next       = NULL;

    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear  = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear       = newNode;
    }

    queue->size++;
}

int dequeueUser(MatchQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return -1;
    }

    MatchQueueNode* temp = queue->front;
    int user_id          = temp->user_id;

    if (queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear  = NULL;
    } else {
        queue->front = queue->front->next;
    }

    free(temp);
    queue->size--;
    return user_id;
}

int peekQueue(MatchQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return -1;
    }
    return queue->front->user_id;
}

bool isUserInQueue(MatchQueue* queue, int user_id) {
    MatchQueueNode* current = queue->front;
    while (current != NULL) {
        if (current->user_id == user_id) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void displayMatchQueue(MatchQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Matchmaking queue is empty.\n");
        return;
    }

    printf("\n=== Matchmaking Queue ===\n");
    printf("%-5s %-12s %-5s\n", "Pos", "User ID", "Level");
    printf("%-5s %-12s %-5s\n", "---", "-------", "-----");

    MatchQueueNode* current = queue->front;
    int pos = 1;
    while (current != NULL) {
        printf("%-5d %-12d %-5d\n", pos, current->user_id, current->user_level);
        current = current->next;
        pos++;
    }
    printf("Queue size: %d\n\n", queue->size);
}

int findSameLevelOpponent(MatchQueue* queue, int user_id, int user_level) {
    MatchQueueNode* current = queue->front;
    while (current != NULL) {
        if (current->user_id != user_id &&
            current->user_level == user_level) {
            return current->user_id;
        }
        current = current->next;
    }
    return -1;
}

void removeFromQueue(MatchQueue* queue, int user_id) {
    MatchQueueNode* current  = queue->front;
    MatchQueueNode* previous = NULL;

    while (current != NULL) {
        if (current->user_id == user_id) {
            if (previous == NULL) {
                queue->front = current->next;
            } else {
                previous->next = current->next;
            }

            if (current == queue->rear) {
                queue->rear = previous;
            }

            free(current);
            queue->size--;
            return;
        }
        previous = current;
        current  = current->next;
    }
}
