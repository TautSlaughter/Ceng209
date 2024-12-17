# Ceng209
Teoman Güven 23050151039
# Dungeon Adventure Game

Welcome to the **Dungeon Adventure Game**! This is a text-based adventure game where you explore rooms, collect items, fight creatures, and manage your inventory. The game is written in C and provides a simple yet engaging gameplay experience.

---

## Table of Contents

- [Gameplay](#gameplay)
  - [Starting the Game](#starting-the-game)
  - [Available Commands](#available-commands)
  - [Exploring Rooms](#exploring-rooms)
  - [Items and Inventory](#items-and-inventory)
  - [Combat](#combat)
  - [The Store](#the-store)
  - [Saving and Loading](#saving-and-loading)
- [Code Structure](#code-structure)
  - [Files](#files)
  - [Data Structures](#data-structures)
  - [Function Overview](#function-overview)
- [Game Logic](#game-logic)
  - [Initialization](#initialization)
  - [Main Game Loop](#main-game-loop)
  - [Room Connections](#room-connections)
  - [Player Actions](#player-actions)
  - [Combat Mechanics](#combat-mechanics)
  - [Inventory Management](#inventory-management)
  - [Saving and Loading Mechanics](#saving-and-loading-mechanics)
- [Compilation and Execution](#compilation-and-execution)

---

## Gameplay

### Starting the Game

After compiling the game, run the executable to start. You will see the main menu:

```
Welcome to the Dungeon Adventure Game!
1. New Game
2. Load Game
3. Exit
Please enter your choice:
```

- **New Game**: Starts a new adventure.
- **Load Game**: Allows loading a previously saved game state.
- **Exit**: Exits the game.

### Available Commands

Use the following commands during gameplay:

- `move <direction>`: Move to a different room (`up`, `down`, `left`, or `right`).
- `look`: Inspect the current room.
- `pickup <item>`: Collect an item in the room.
- `attack`: Fight a creature in the room.
- `inventory`: View your inventory and gold.
- `drop`: Drop an item from your inventory.
- `store`: Access the store to buy items.
- `save <filepath>`: Save your game.
- `load <filepath>`: Load a game.
- `help`: Display a list of commands.
- `menu`: Return to the main menu.
- `exit`: Exit the game.

### Exploring Rooms

Rooms are randomly generated and may contain items, creatures, or traps. Use `look` to inspect the room.

- **Navigation**: Move between rooms using the `move` command.
- **Traps**: Be cautious—traps can harm you.
- **Creatures**: Fight creatures to earn rewards.

### Items and Inventory

- **Picking Up Items**: Use `pickup <item>` to add items to your inventory.
- **Inventory Limit**: Carry up to 5 items.
- **Healthapple**: Restores health upon pickup.
- **Gold**: Used to purchase items in the store.

### Combat

Fight creatures with the `attack` command.

- **Player Damage**: Strength + weapon bonuses.
- **Creature Damage**: Randomized but reduced by defensive items (`Shield`, `Armor`).
- **Victory**: Defeating creatures may yield gold.

### The Store

Buy items to aid your journey with the `store` command:

- **Sword**: 100 gold
- **Shield**: 80 gold
- **Armor**: 120 gold
- **Magicwand**: 150 gold
- **Healthapple**: 50 gold

### Saving and Loading

- **Saving**: Use `save <filepath>` to save your game.
- **Loading**: Use `load <filepath>` to load a saved game.

---

## Code Structure

### Files

- **`game.h`**: Contains type definitions, global variables, and function prototypes.
- **`game.c`**: Implements game logic.

### Data Structures

#### `Player`
```c
typedef struct {
    char name[50];
    int health;
    int strength;
    int gold;
    char inventory[MAX_INVENTORY][50];
    int inventory_count;
} Player;
```
Represents the player, including stats and inventory.

#### `Room`
```c
typedef struct Room {
    int id;
    char description[MAX_DESC_LENGTH];
    char item[50];
    char creature[50];
    int creature_health;
    char trap[50];
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
} Room;
```
Represents a room, including items, creatures, traps, and connections to other rooms.

### Function Overview

- **Initialization**:
  - `initialize_game()`: Sets up rooms and the player.
  - `main_menu()`: Displays the main menu.

- **Gameplay**:
  - `main()`: Runs the game loop.
  - `parse_command()`: Executes player commands.

- **Player Actions**:
  - `move()`, `look()`, `pickup()`, `attack()`, `inventory()`, `drop_item()`, `store()`, `show_help()`.

- **Game State**:
  - `save_game()`, `load_game()`, `free_memory()`.

---

## Game Logic

### Initialization

- **Rooms**: Randomly generated with items, creatures, and traps.
- **Player**: Starts with default stats and an empty inventory.

### Main Game Loop

- Waits for player input.
- Executes commands to update the game state.

### Room Connections

- Rooms are connected via pointers (`up`, `down`, `left`, `right`).
- Movement is restricted to connected rooms.

### Combat Mechanics

- Players and creatures attack alternately.
- **Damage**: Weapons increase player damage, while armor reduces incoming damage.

### Inventory Management

- **Capacity**: 5 items.
- **Adding Items**: Use `pickup`.
- **Dropping Items**: Use `drop`.

### Saving and Loading Mechanics

- Saves the entire game state to a file.
- Restores the game state by reading from the file.

---

## Compilation and Execution

### Prerequisites

- A C compiler (e.g., GCC).
- Compatible with Windows, macOS, and Linux.

### Steps

1. Clone or download the repository.
2. Compile the game:
   ```bash
   gcc dungeon.c -o dugneon
   ```
3. Run the game:
   ```bash
   ./dungeon
   ```
   On Windows:
   ```bash
   dungeon.exe
   ```

---


Enjoy your dungeon adventure!

