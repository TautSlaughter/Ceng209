#include "game.h"
#include <stdint.h>  

Room **rooms = NULL; // Global array to store rooms
int NUM_ROOMS = 5;   // Number of rooms, can change dynamically

int main() {
    Player player;
    Room *start_room = NULL, *current_room = NULL;

    main_menu(&player, &start_room, &current_room);

    while (1) {
        char command[100];
        printf(">> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        if (strcmp(command, "exit") == 0) {
            printf("Exiting game. Goodbye!\n");
            break;
        }

        if (strcmp(command, "menu") == 0) {
            main_menu(&player, &start_room, &current_room);
            continue;
        }

        parse_command(&player, &current_room, command);
    }

    free_memory();
    return 0;
}

void main_menu(Player *player, Room **start_room, Room **current_room) {
    char choice[10];

    while (1) {
        printf("Welcome to the Dungeon Adventure Game!\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Exit\n");
        printf("Please enter your choice: ");
        fgets(choice, sizeof(choice), stdin);

        if (choice[0] == '1') {
            // New game
            *player = (Player){"Hero", 100, 10, 0, {{0}}, 0};  // Initialize gold to 0
            initialize_game(player, start_room);
            *current_room = *start_room;
            look(*current_room);  // Show the description of the starting room
            break;
        } else if (choice[0] == '2') {
            // Load game
            printf("Enter the filepath to load the game: ");
            char filepath[256];
            fgets(filepath, sizeof(filepath), stdin);
            filepath[strcspn(filepath, "\n")] = '\0'; // Remove newline character

            // Load the game
            load_game(player, current_room, filepath);
            look(*current_room);  // Show the description after loading
            break;
        } else if (choice[0] == '3') {
            printf("Goodbye!\n");
            exit(0);
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void initialize_game(Player *player, Room **start_room) {
    // Seed the random number generator
    srand(time(NULL));

    // Free existing rooms if any
    free_memory();

    NUM_ROOMS = 5;

    rooms = (Room **)malloc(NUM_ROOMS * sizeof(Room *));
    if (!rooms) {
        perror("Failed to allocate memory for rooms array");
        exit(EXIT_FAILURE);
    }

    // Possible creatures, items, and traps
    char *creatures[] = {"Goblin", "Dragon", "Wizard", "Ghost", "None"};
    char *items[] = {"Sword", "Axe", "Shield", "Armor", "Magicwand", "Healthapple", ""};
    char *traps[] = {"Pitfall", "Poison Dart", "Fire Trap", "None"};

    // Allocate and initialize rooms
    for (int i = 0; i < NUM_ROOMS; ++i) {
        rooms[i] = (Room *)malloc(sizeof(Room));
        if (!rooms[i]) {
            perror("Failed to allocate memory for room");
            exit(EXIT_FAILURE);
        }

        rooms[i]->id = i + 1;
        sprintf(rooms[i]->description, "This is room number %d.", rooms[i]->id);

        // Randomly assign an item
        strcpy(rooms[i]->item, items[rand() % (sizeof(items) / sizeof(items[0]))]);

        // Randomly assign a creature
        strcpy(rooms[i]->creature, creatures[rand() % (sizeof(creatures) / sizeof(creatures[0]))]);

        // Set creature health if there is a creature
        if (strcmp(rooms[i]->creature, "None") != 0) {
            rooms[i]->creature_health = 20 + rand() % 31;  // Random health between 20 and 50
        } else {
            rooms[i]->creature_health = 0;
        }

        // Randomly assign a trap
        strcpy(rooms[i]->trap, traps[rand() % (sizeof(traps) / sizeof(traps[0]))]);

        // Initialize room connections to NULL
        rooms[i]->up = rooms[i]->down = rooms[i]->left = rooms[i]->right = NULL;
    }

    // Manually set up the room connections (you can randomize this as well)
    // Room 0 connections
    rooms[0]->right = rooms[1];

    // Room 1 connections
    rooms[1]->left = rooms[0];
    rooms[1]->right = rooms[2];

    // Room 2 connections
    rooms[2]->left = rooms[1];
    rooms[2]->up = rooms[3];

    // Room 3 connections
    rooms[3]->down = rooms[2];
    rooms[3]->right = rooms[4];

    // Room 4 connections
    rooms[4]->left = rooms[3];

    *start_room = rooms[0];
}

Room *get_room_by_id(int id) {
    if (id == -1)
        return NULL;
    for (int i = 0; i < NUM_ROOMS; i++) {
        if (rooms[i]->id == id)
            return rooms[i];
    }
    return NULL;
}

void parse_command(Player *player, Room **current_room, char *command) {
    char *token = strtok(command, " ");
    if (!token) return;

    if (strcmp(token, "move") == 0) {
        char *direction = strtok(NULL, " ");
        if (direction) move(player, current_room, direction);
        else printf("Usage: move <direction>\n");
    } else if (strcmp(token, "look") == 0) {
        look(*current_room);
    } else if (strcmp(token, "pickup") == 0) {
        char *item = strtok(NULL, " ");
        if (item) pickup(player, *current_room, item);
        else printf("Usage: pickup <item>\n");
    } else if (strcmp(token, "attack") == 0) {
        attack(player, *current_room);
    } else if (strcmp(token, "inventory") == 0) {
        inventory(player);
    } else if (strcmp(token, "save") == 0) {
        char *filepath = strtok(NULL, " ");
        if (filepath) save_game(player, *current_room, filepath);
        else printf("Usage: save <filepath>\n");
    } else if (strcmp(token, "load") == 0) {
        char *filepath = strtok(NULL, " ");
        if (filepath) load_game(player, current_room, filepath);
        else printf("Usage: load <filepath>\n");
    } else if (strcmp(token, "store") == 0) {
        store(player);
    } else if (strcmp(token, "drop") == 0) {
        drop_item(player, *current_room);
    } else if (strcmp(token, "help") == 0) {
        show_help();
    } else {
        printf("Unknown command: %s\n", token);
    }
}

void look(Room *current_room) {
    printf("Room description: %s\n", current_room->description);
    if (strlen(current_room->item) > 0 && strcmp(current_room->item, "") != 0)
        printf("You see a %s here.\n", current_room->item);
    if (strcmp(current_room->creature, "None") != 0)
        printf("There is a %s here with %d health.\n", current_room->creature, current_room->creature_health);

    // Show available directions
    printf("You can go to the following directions:\n");
    if (current_room->up) printf("- Up\n");
    if (current_room->down) printf("- Down\n");
    if (current_room->left) printf("- Left\n");
    if (current_room->right) printf("- Right\n");

    if (strcmp(current_room->trap, "None") != 0)
        printf("You sense something off about this room...\n");
}

void handle_trap(Player *player, Room *current_room) {
    if (strcmp(current_room->trap, "None") == 0) {
        return;
    }

    printf("Oh no! You've triggered a %s!\n", current_room->trap);

    if (strcmp(current_room->trap, "Pitfall") == 0) {
        int damage = 15;
        player->health -= damage;
        printf("You fell into a pit and took %d damage. Your health is now %d.\n", damage, player->health);
    } else if (strcmp(current_room->trap, "Poison Dart") == 0) {
        int damage = 10;
        player->health -= damage;
        printf("A poison dart hit you! You took %d damage. Your health is now %d.\n", damage, player->health);
    } else if (strcmp(current_room->trap, "Fire Trap") == 0) {
        int damage = 20;
        player->health -= damage;
        printf("Flames engulf you! You took %d damage. Your health is now %d.\n", damage, player->health);
    }

    // After triggering, the trap is deactivated
    strcpy(current_room->trap, "None");

    if (player->health <= 0) {
        printf("You have perished due to the trap. Game over.\n");
        exit(0);
    }
}

void move(Player *player, Room **current_room, char *direction) {
    Room *next_room = NULL;

    if (strcmp(direction, "up") == 0) next_room = (*current_room)->up;
    else if (strcmp(direction, "down") == 0) next_room = (*current_room)->down;
    else if (strcmp(direction, "left") == 0) next_room = (*current_room)->left;
    else if (strcmp(direction, "right") == 0) next_room = (*current_room)->right;

    if (next_room) {
        *current_room = next_room;
        printf("You moved %s.\n", direction);

        // Handle trap after moving
        handle_trap(player, *current_room);

        look(*current_room);
    } else {
        printf("You can't move %s.\n", direction);
    }
}

void pickup(Player *player, Room *current_room, char *item) {
    if (strcmp(current_room->item, item) == 0) {
        if (strcmp(item, "Healthapple") == 0) {
            // Increase player's health
            int health_gain = 20;
            player->health += health_gain;
            if (player->health > 100) player->health = 100;  // Max health is 100
            printf("You ate a Healthapple and gained %d health. Your health is now %d.\n", health_gain, player->health);
        } else {
            if (player->inventory_count < MAX_INVENTORY) {
                strcpy(player->inventory[player->inventory_count++], item);
                printf("You picked up a %s.\n", item);
            } else {
                printf("Your inventory is full.\n");
                return;
            }
        }
        strcpy(current_room->item, "");  // Remove item from room
    } else {
        printf("There is no %s here.\n", item);
    }
}

void attack(Player *player, Room *current_room) {
    if (strcmp(current_room->creature, "None") == 0) {
        printf("There is nothing to attack here.\n");
        return;
    }

    printf("You attack the %s!\n", current_room->creature);

    // Calculate player's damage modifier
    int player_damage = player->strength;
    for (int i = 0; i < player->inventory_count; ++i) {
        if (strcmp(player->inventory[i], "Sword") == 0 ||
            strcmp(player->inventory[i], "Axe") == 0 ||
            strcmp(player->inventory[i], "Magicwand") == 0) {
            player_damage += 10;  // Increase damage by 10
            break;  // Only apply once even if multiple items
        }
    }

    while (player->health > 0 && current_room->creature_health > 0) {
        // Player attacks creature
        current_room->creature_health -= player_damage;
        printf("You hit the %s for %d damage! Its health is now %d.\n", current_room->creature, player_damage, current_room->creature_health);

        if (current_room->creature_health > 0) {
            // Creature attacks player
            int creature_damage = 5 + rand() % 11;  // Random damage between 5 and 15

            // Reduce damage if player has Shield or Armor
            for (int i = 0; i < player->inventory_count; ++i) {
                if (strcmp(player->inventory[i], "Shield") == 0 ||
                    strcmp(player->inventory[i], "Armor") == 0) {
                    creature_damage -= 5;  // Reduce damage by 5
                    if (creature_damage < 0) creature_damage = 0;
                    break;  // Only apply once even if multiple items
                }
            }

            player->health -= creature_damage;
            printf("The %s fights back and deals %d damage! Your health: %d\n", current_room->creature, creature_damage, player->health);
        } else {
            printf("You defeated the %s!\n", current_room->creature);

            // Chance to drop gold upon defeating creature
            int gold_drop = 20 + rand() % 31;  // Between 20 and 50
            player->gold += gold_drop;
            printf("You found %d gold on the %s. You now have %d gold.\n", gold_drop, current_room->creature, player->gold);

            strcpy(current_room->creature, "None");
            current_room->creature_health = 0;
        }

        if (player->health <= 0) {
            printf("You have been defeated by the %s. Game over.\n", current_room->creature);
            exit(0);
        }
    }
}

void inventory(Player *player) {
    printf("Inventory:\n");
    for (int i = 0; i < player->inventory_count; ++i) {
        printf("- %s\n", player->inventory[i]);
    }
    if (player->inventory_count == 0) {
        printf("Your inventory is empty.\n");
    }
    printf("Gold: %d\n", player->gold);  // Display player's gold
}

void drop_item(Player *player, Room *current_room) {
    if (player->inventory_count == 0) {
        printf("Your inventory is empty. You have nothing to drop.\n");
        return;
    }

    printf("Your inventory:\n");
    for (int i = 0; i < player->inventory_count; ++i) {
        printf("%d. %s\n", i + 1, player->inventory[i]);
    }
    printf("Enter the number of the item you wish to drop: ");

    char input[10];
    fgets(input, sizeof(input), stdin);
    int choice = atoi(input);

    if (choice < 1 || choice > player->inventory_count) {
        printf("Invalid choice.\n");
        return;
    }

    // Get the item to drop
    char dropped_item[50];
    strcpy(dropped_item, player->inventory[choice - 1]);

    // Remove the item from inventory
    for (int i = choice - 1; i < player->inventory_count - 1; ++i) {
        strcpy(player->inventory[i], player->inventory[i + 1]);
    }
    player->inventory_count--;

    printf("You dropped %s.\n", dropped_item);

    // Place the item in the room if room has no item
    if (strlen(current_room->item) == 0 || strcmp(current_room->item, "") == 0) {
        strcpy(current_room->item, dropped_item);
        printf("%s is now on the ground in this room.\n", dropped_item);
    } else {
        printf("There is already an item in this room. %s is discarded.\n", dropped_item);
    }
}

void show_help() {
    printf("Available commands:\n");
    printf("move <direction> - Move to a different room (directions: up, down, left, right)\n");
    printf("look - Look around the current room\n");
    printf("pickup <item> - Pick up an item in the room\n");
    printf("attack - Attack a creature in the room\n");
    printf("inventory - View your inventory and gold\n");
    printf("drop - Drop an item from your inventory\n");
    printf("store - Access the store to purchase items\n");
    printf("save <filepath> - Save your game to the specified file\n");
    printf("load <filepath> - Load a game from the specified file\n");
    printf("help - Show this list of commands\n");
    printf("menu - Return to the main menu\n");
    printf("exit - Exit the game\n");
}

void store(Player *player) {
    printf("Welcome to the store!\n");
    printf("You have %d gold.\n", player->gold);
    printf("Available items for purchase:\n");
    printf("1. Sword (100 gold)\n");
    printf("2. Shield (80 gold)\n");
    printf("3. Armor (120 gold)\n");
    printf("4. Magicwand (150 gold)\n");
    printf("5. Healthapple (50 gold)\n");
    printf("6. Exit Store\n");
    printf("Enter the number of the item you wish to buy: ");

    char choice[10];
    fgets(choice, sizeof(choice), stdin);

    int item_price = 0;
    char item_name[50] = "";

    if (choice[0] == '1') {
        strcpy(item_name, "Sword");
        item_price = 100;
    } else if (choice[0] == '2') {
        strcpy(item_name, "Shield");
        item_price = 80;
    } else if (choice[0] == '3') {
        strcpy(item_name, "Armor");
        item_price = 120;
    } else if (choice[0] == '4') {
        strcpy(item_name, "Magicwand");
        item_price = 150;
    } else if (choice[0] == '5') {
        strcpy(item_name, "Healthapple");
        item_price = 50;
    } else if (choice[0] == '6') {
        printf("Exiting store.\n");
        return;
    } else {
        printf("Invalid choice.\n");
        return;
    }

    if (player->gold >= item_price) {
        player->gold -= item_price;
        printf("You purchased a %s.\n", item_name);

        if (strcmp(item_name, "Healthapple") == 0) {
            // Increase player's health
            int health_gain = 20;
            player->health += health_gain;
            if (player->health > 100) player->health = 100;
            printf("You consumed the Healthapple and gained %d health. Your health is now %d.\n", health_gain, player->health);
        } else {
            // Add item to inventory
            if (player->inventory_count < MAX_INVENTORY) {
                strcpy(player->inventory[player->inventory_count++], item_name);
            } else {
                printf("Your inventory is full. You can't carry more items.\n");
            }
        }
    } else {
        printf("You do not have enough gold to purchase %s.\n", item_name);
    }
}

void save_game(Player *player, Room *current_room, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        perror("Failed to open file for saving");
        return;
    }

    // Save the number of rooms
    fprintf(fp, "%d\n", NUM_ROOMS);

    // Save the rooms' data
    for (int i = 0; i < NUM_ROOMS; ++i) {
        Room *room = rooms[i];
        fprintf(fp, "ROOM\n");
        fprintf(fp, "%d\n", room->id);
        fprintf(fp, "%s\n", room->description);
        fprintf(fp, "%s\n", room->item);
        fprintf(fp, "%s\n", room->creature);
        fprintf(fp, "%d\n", room->creature_health);
        fprintf(fp, "%s\n", room->trap);  // Save trap information
        fprintf(fp, "%d\n", room->up ? room->up->id : -1);
        fprintf(fp, "%d\n", room->down ? room->down->id : -1);
        fprintf(fp, "%d\n", room->left ? room->left->id : -1);
        fprintf(fp, "%d\n", room->right ? room->right->id : -1);
    }

    // Save player's data
    fprintf(fp, "PLAYER\n");
    fprintf(fp, "%s\n", player->name);
    fprintf(fp, "%d\n", player->health);
    fprintf(fp, "%d\n", player->strength);
    fprintf(fp, "%d\n", player->gold);  // Save player's gold
    fprintf(fp, "%d\n", player->inventory_count);
    for (int i = 0; i < player->inventory_count; ++i) {
        fprintf(fp, "%s\n", player->inventory[i]);
    }

    // Save current room id
    fprintf(fp, "%d\n", current_room->id);

    fclose(fp);
    printf("Game saved to %s\n", filepath);
}

void load_game(Player *player, Room **current_room, const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        perror("Failed to open file for loading");
        return;
    }

    char line[256];  // Declare 'line' here

    // Read the number of rooms
    fscanf(fp, "%d\n", &NUM_ROOMS);

    // Free existing rooms if any
    free_memory();

    // Allocate rooms array
    rooms = (Room **)malloc(NUM_ROOMS * sizeof(Room *));
    if (!rooms) {
        perror("Failed to allocate memory for rooms array");
        exit(EXIT_FAILURE);
    }

    // Read rooms data
    for (int i = 0; i < NUM_ROOMS; ++i) {
        fgets(line, sizeof(line), fp); // Should be "ROOM\n"

        int id;
        char description[MAX_DESC_LENGTH];
        char item[50];
        char creature[50];
        int creature_health;
        char trap[50];  // Added trap
        int up_id, down_id, left_id, right_id;

        fscanf(fp, "%d\n", &id);

        fgets(description, sizeof(description), fp);
        description[strcspn(description, "\n")] = '\0';

        fgets(item, sizeof(item), fp);
        item[strcspn(item, "\n")] = '\0';

        fgets(creature, sizeof(creature), fp);
        creature[strcspn(creature, "\n")] = '\0';

        fscanf(fp, "%d\n", &creature_health);

        fgets(trap, sizeof(trap), fp);  // Read trap information
        trap[strcspn(trap, "\n")] = '\0';

        fscanf(fp, "%d\n", &up_id);
        fscanf(fp, "%d\n", &down_id);
        fscanf(fp, "%d\n", &left_id);
        fscanf(fp, "%d\n", &right_id);

        // Initialize room
        rooms[i] = (Room *)malloc(sizeof(Room));
        if (!rooms[i]) {
            perror("Failed to allocate memory for room");
            exit(EXIT_FAILURE);
        }

        rooms[i]->id = id;
        strcpy(rooms[i]->description, description);
        strcpy(rooms[i]->item, item);
        strcpy(rooms[i]->creature, creature);
        rooms[i]->creature_health = creature_health;
        strcpy(rooms[i]->trap, trap);  // Set trap
        rooms[i]->up = (Room *)(intptr_t)(long)up_id;
        rooms[i]->down = (Room *)(intptr_t)(long)down_id;
        rooms[i]->left = (Room *)(intptr_t)(long)left_id;
        rooms[i]->right = (Room *)(intptr_t)(long)right_id;
    }

    // Now resolve the room pointers
    for (int i = 0; i < NUM_ROOMS; ++i) {
        Room *room = rooms[i];

        int up_id = (int)(intptr_t)(long)room->up;
        int down_id = (int)(intptr_t)(long)room->down;
        int left_id = (int)(intptr_t)(long)room->left;
        int right_id = (int)(intptr_t)(long)room->right;

        room->up = get_room_by_id(up_id);
        room->down = get_room_by_id(down_id);
        room->left = get_room_by_id(left_id);
        room->right = get_room_by_id(right_id);
    }

    // Read player's data
    fgets(line, sizeof(line), fp); // Should be "PLAYER\n"

    fgets(player->name, sizeof(player->name), fp);
    player->name[strcspn(player->name, "\n")] = '\0';

    fscanf(fp, "%d\n", &player->health);
    fscanf(fp, "%d\n", &player->strength);
    fscanf(fp, "%d\n", &player->gold);  // Load player's gold
    fscanf(fp, "%d\n", &player->inventory_count);

    for (int i = 0; i < player->inventory_count; ++i) {
        fgets(player->inventory[i], sizeof(player->inventory[i]), fp);
        player->inventory[i][strcspn(player->inventory[i], "\n")] = '\0';
    }

    int current_room_id;
    fscanf(fp, "%d\n", &current_room_id);

    *current_room = get_room_by_id(current_room_id);

    fclose(fp);
    printf("Game loaded from %s\n", filepath);
}

void free_memory() {
    if (rooms) {
        for (int i = 0; i < NUM_ROOMS; ++i) {
            if (rooms[i]) {
                free(rooms[i]);
            }
        }
        free(rooms);
        rooms = NULL;
    }
}

