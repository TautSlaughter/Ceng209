#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>      // For random number generation

#define MAX_INVENTORY 5
#define MAX_DESC_LENGTH 256

typedef struct {
    char name[50];
    int health;
    int strength;
    int gold;  // Added gold field
    char inventory[MAX_INVENTORY][50];
    int inventory_count;
} Player;

typedef struct Room {
    int id; // Room ID
    char description[MAX_DESC_LENGTH];
    char item[50];
    char creature[50];
    int creature_health;
    char trap[50];  // Added trap field
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
} Room;

// Global variables
extern Room **rooms;    // Global array to store rooms
extern int NUM_ROOMS;   // Number of rooms, can change dynamically

// Function prototypes
void initialize_game(Player *player, Room **start_room);
void parse_command(Player *player, Room **current_room, char *command);
void look(Room *current_room);
void move(Player *player, Room **current_room, char *direction);
void pickup(Player *player, Room *current_room, char *item);
void attack(Player *player, Room *current_room);
void inventory(Player *player);
void save_game(Player *player, Room *current_room, const char *filepath);
void load_game(Player *player, Room **current_room, const char *filepath);
void free_memory();
Room *get_room_by_id(int id);
void main_menu(Player *player, Room **start_room, Room **current_room);
void store(Player *player);
void handle_trap(Player *player, Room *current_room);
void drop_item(Player *player, Room *current_room);
void show_help();  // Added help function prototype

#endif // GAME_H

