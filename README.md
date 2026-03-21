# DSA — Football Card Game

A football card game implemented in C as a group project, using various data structures (linked lists, queues, stacks, etc.).

## Requirements

- GCC (or any C compiler supporting C99+)
- A Unix-like terminal (Linux, macOS, or WSL on Windows)

## Build

Compile all source files with:

```bash
gcc -o football main.c user_registration.c inventory.c squad.c \
    marketplace.c matchmaking.c leaderboard.c matchhistory.c -lm
```

## Run

After compiling, start the game with:

```bash
./football
```

## Modules

| File | Data Structure | Responsibility |
|------|---------------|----------------|
| `user_registration.c` | Doubly Linked List | User accounts & profiles |
| `inventory.c` | Doubly Linked List | Player card inventory per user |
| `squad.c` | Array (fixed size 11) | User's active football squad |
| `marketplace.c` | Circular Doubly Linked List | Global player card trading |
| `matchmaking.c` | Queue (FIFO) | Match pairing between users |
| `leaderboard.c` | Circular Linked List | Global rankings (bubble sort) |
| `matchhistory.c` | Stack (LIFO) | Per-user match records |

For a full walkthrough of the game's features and user workflow, see [user_workflow.md](user_workflow.md).