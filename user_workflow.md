# Football Card Game — User Workflow

## Overview

This is a 7-member group project implementing a football card game in C.  
Each feature is implemented as a separate `.c` file using a specific data structure, and all modules are combined through `main.c`.

---

## Modules & Data Structures

| File | Data Structure | Responsibility |
|------|---------------|----------------|
| `user_registration.c` | Doubly Linked List | User accounts & profiles |
| `inventory.c` | Doubly Linked List | Player card inventory per user |
| `squad.c` | Array (fixed size 11) | User's active football squad |
| `marketplace.c` | Circular Doubly Linked List | Global player card trading |
| `matchmaking.c` | Queue (FIFO) | Match pairing between users |
| `leaderboard.c` | Circular Linked List | Global rankings (bubble sort) |
| `matchhistory.c` | Stack (LIFO) | Per-user match records |

---

## Levels

Players advance through levels based on total wins:

| Level | Name | Wins Required | Weekly Reward |
|-------|------|--------------|---------------|
| 0 | Amateur 3 | 0+ | 10,000 coins |
| 1 | Amateur 2 | 3+ | 40,000 coins |
| 2 | Amateur 1 | 6+ | 70,000 coins |
| 3 | Pro 3 | 10+ | 100,000 coins |
| 4 | Pro 2 | 15+ | 130,000 coins |
| 5 | Pro 1 | 21+ | 160,000 coins |
| 6 | World Class 2 | 28+ | 190,000 coins |
| 7 | World Class 1 | 35+ | 220,000 coins |
| 8 | Legendary | 40+ | 250,000 coins |

---

## Complete User Workflow

### 1. Registration

```
Main Menu → [1] Register
  → Enter username
  → System assigns a unique ID and 5,000 starting coins
  → User receives 11 starter player cards (Rating 60–69):
      • 3 × FW (Forward)
      • 3 × MF (Midfielder)
      • 4 × DF (Defender)
      • 1 × GK (Goalkeeper)
  → User is added to the global leaderboard at Amateur 3
```

### 2. Login

```
Main Menu → [2] Login
  → Enter username
  → Redirected to User Dashboard
```

---

### 3. Inventory Management

```
Dashboard → [1] Inventory
  → [1] View Inventory        — Lists all player cards (name, type, rating, price)
  → [2] Search by Type        — Filter cards by FW / MF / DF / GK
  → [3] Search by Rating      — Filter cards within a rating range
  → [4] Sell Player           — Move a card to the Marketplace
       → Select player name
       → Enter selling price (coins)
       → Confirm → Card removed from inventory, listed on marketplace
```

### 4. Squad Management

```
Dashboard → [2] Squad
  → [1] View Squad            — Shows all 11 slots (position, player, rating)
  → [2] Auto-Select Best Squad
       → System picks the highest-rated available card for each position
       → Formation: FW(×3), MF(×3), DF(×4), GK(×1)
  → [3] Replace Player
       → Enter name of player currently in squad
       → Enter name of replacement player (must be in inventory)
       → Player is replaced in that squad slot
  → [4] Find Best Lineup      — Rebuilds squad with highest-rated cards
```

### 5. Marketplace

```
Dashboard → [3] Marketplace
  → [1] View All Listings     — Shows all cards for sale (name, type, rating, price, seller)
  → [2] Search by Name        — Find a specific card
  → [3] Search by Type        — Filter listings by position type
  → [4] Buy Player
       → Browse/search listings
       → Select a card (not your own listing)
       → Confirm purchase
       → Coins deducted from buyer
       → Coins credited to seller
       → Card added to buyer's inventory
       → Card removed from marketplace
```

### 6. Matchmaking

```
Dashboard → [4] Matchmaking
  → [1] Join Queue            — Add yourself to the FIFO matchmaking queue
                                 (requires squad to be set up)
  → [2] View Queue            — See all players currently waiting
  → [3] Process Match
       → System finds an opponent at the SAME level (FIFO order)
       → Both players are removed from the queue
       → Match is simulated based on average squad ratings (+ randomness)
       → Result: WIN / LOSS / DRAW
       → Rewards: Win=500 coins, Draw=250 coins each, Loss=0 coins
       → User levels update automatically
       → Leaderboard updates in real-time
       → Match recorded in both users' match histories
```

**Match Simulation:**
- Each user's squad average rating is calculated
- A random factor (±10) is added to each rating
- Higher adjusted rating wins; equal ratings result in a draw

### 7. Leaderboard

```
Dashboard → [5] Leaderboard
  → [1] View Global Leaderboard  — Ranked list sorted by wins (bubble sort)
  → [2] View My Rank             — Your current position and stats
  → [3] Distribute Weekly Rewards
       → All users receive coins based on their current level
       → Weekly match statistics reset for all users
       → Week counter advances
```

**Weekly Rewards:**  
Level Amateur 3 earns 10,000 coins; each subsequent level earns 30,000 more coins, up to 250,000 coins at Legendary.

### 8. Match History

```
Dashboard → [6] Match History
  → [1] View Match History       — Stack display (most recent match first)
                                     Shows: opponent, result, coins earned, week
  → [2] Overall Win Percentage   — Win% across all matches ever played
  → [3] This Week's Win %        — Win% for the current week only
                                     Resets every time weekly rewards are distributed
```

---

## How Modules Connect

```
Registration
    │
    ├─→ Inventory (11 starter cards given)
    ├─→ Squad (empty, then auto-select or manually build)
    └─→ Leaderboard (user added at Amateur 3)
         │
         └─→ Matchmaking
                  │
                  ├─→ Match result → Coins updated
                  ├─→ Match result → Match History (stack push)
                  ├─→ Match result → User level updated
                  └─→ Match result → Leaderboard re-sorted (bubble sort)

Inventory ←→ Marketplace
    │              │
    └──────────────┘
    (sell: inventory → marketplace)
    (buy:  marketplace → inventory, coins transferred)
```

---

## Build & Run

```bash
gcc -o football main.c user_registration.c inventory.c squad.c \
    marketplace.c matchmaking.c leaderboard.c matchhistory.c -lm

./football
```

---

## Squad Formation

```
        [FW1] [FW2] [FW3]
     [MF1]  [MF2]  [MF3]
  [DF1] [DF2] [DF3] [DF4]
              [GK1]
```

Slot index mapping:
- Slots 0–2: Forward (FW)
- Slots 3–5: Midfielder (MF)
- Slots 6–9: Defender (DF)
- Slot 10: Goalkeeper (GK)
