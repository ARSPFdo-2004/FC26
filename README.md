# FC26 — Football Card Game (Single‑File Version)

FC26 is a football card game implemented in C. It showcases multiple data structures (linked lists, queues, stacks, circular lists, etc.) inside a single consolidated source file.

Current project structure:

- [FC26.c](FC26.c) — all game logic and data structures
- [user_workflow.md](user_workflow.md) — detailed feature and menu walkthrough

## Requirements

- GCC or Clang with C99 support
- Any OS with a terminal (Linux, macOS, WSL, or Windows with MinGW/MSYS)

## Build

From the FC26 folder, compile the single source file:

```bash
gcc FC26.c -o FC26
```

On Windows with MinGW you might prefer:

```bash
gcc FC26.c -o FC26.exe
```

If you are using the provided VS Code task, run the
“C/C++: gcc build active file” task and it will produce the
`FC26` (or `FC26.exe`) executable next to the source file.

## Run

After building, run the executable:

```bash
./FC26      # Linux / macOS / WSL
FC26.exe    # Windows (MinGW)
```

## Logical Modules & Data Structures (inside FC26.c)

Even though everything lives in a single file, the code is
organized into logical modules, each using a specific
data structure:

| Logical Module        | Data Structure              | Responsibility                           |
|-----------------------|----------------------------|------------------------------------------|
| Inventory             | Doubly Linked List         | Per‑user player card inventory           |
| Squad                 | Fixed‑size Array (11 slots)| User’s active football squad (line‑up)   |
| Marketplace           | Circular Doubly Linked List| Global player card trading               |
| Matchmaking Queue     | Queue (FIFO)               | Match pairing between users / bots       |
| Match History         | Stack (LIFO)               | Per‑user match records & statistics      |
| User Registry         | Doubly Linked List         | User accounts, levels, and coins         |
| Leaderboard           | Circular Linked List       | Global rankings (bubble‑sorted by wins)  |

## Features

- User registration and login with per‑user inventories
- Automatic starter squad and card distribution
- Inventory search (by type, rating range, name)
- Squad management and auto best‑XI selection
- Marketplace for buying/selling player cards
- Matchmaking with level‑based opponents (including bots)
- Match simulation with rich statistics
- Global leaderboard and weekly rewards
- Per‑user match history and detailed match stats

For a step‑by‑step description of menus and flows, see
[user_workflow.md](user_workflow.md). Note that the
workflow still applies, but this version bundles all
modules into the single [FC26.c](FC26.c) file instead of
separate `.c` files.