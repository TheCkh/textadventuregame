#include <stdio.h>
#include <string.h>

// You can run the game with ./gamefile.exe <filehere> | <filehere> is only csv formatted file for the game. Try savegame.text.
void flush_input() // this code just 'flushes' the buffer to prevent overflow. Maybe unnecessary?
{
    int ch;
    while ((ch = getchar() != '\n' && ch != EOF)) // use ' ' for \n
        ;
}

typedef struct
{
    char class[50];
    int hp;
    int maxhp;
    int level;
    int gold;
    int moxie;
    int magic;
    int str;
    int mana;
    int maxmana;
    struct
    {
        char itemName[50];
        int quantity;
        char location[50];
        char spells[50];
        int check;
        int wins;
        int loss;
    } items[10]; // Struct inside another struct. As of now, we only have 10 items. Player.items[0] = first item!
} Player;

void save_game(Player *player, const char *filename, const char *csvfilename)
{
    FILE *file = fopen(filename, "w");
    FILE *csvfile = fopen(csvfilename, "w");

    if (file == NULL || csvfile == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Writing header to textfile
    fprintf(file, "Class,Item,Howmany,HP,MaxHP,Gold,Level,Moxie,Magic,Str,Mana,MaxMana,Spells,Check,Location,Wins,Loss\n");

    // Writing header to csvfile
    fprintf(csvfile, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            "Class", "Item", "Howmany", "HP", "MaxHP", "Level", "Moxie", "Magic", "Str", "Mana", "MaxMana", "Spells", "Check", "Location", "Wins", "Loss");

    // Writing player data to file
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            // // Write class and HP only once, in the first line
            // fprintf(file, "%s,%s,%d,%d\n", player->class, player->items[i].itemName, player->items[i].quantity, player->hp);

            // Writing player data to csvfile, this code below will only work if there is a game logic to update location, spells, check, wins, loss, otherwise it will just output the address of the struct
            // fprintf(file, "%s,%s,%d,%d,%s,%s,%s,%d,%d,%d\n",
            //         player->class, player->items[i].itemName, player->items[i].quantity, player->hp,
            //         player->location, player->spells, player->check, player->level, player->wins, player->loss);

            // Write class and HP only once, in the first line
            fprintf(file, "%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%s,%d,%d\n",
                    player->class,
                    player->items[i].itemName,
                    player->items[i].quantity,
                    player->hp,
                    player->maxhp,
                    player->gold,
                    player->level,
                    player->moxie,
                    player->magic,
                    player->str,
                    player->mana,
                    player->maxmana,
                    player->items[i].spells,
                    player->items[i].check,
                    player->items[i].location,
                    player->items[i].wins,
                    player->items[i].loss);

            // Writing player data to csvfile, this code below will only work if there is a game logic to update location, spells, check, wins, loss, otherwise it will just output the address of the struct
            // fprintf(csvfile, "%s,%s,%d,%d,%s,%s,%s,%d,%d,%d\n",
            //         player->class, player->items[i].itemName, player->items[i].quantity, player->hp,
            //         player->location, player->spells, player->check, player->level, player->wins, player->loss);

            fprintf(csvfile, "%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%s,%d,%d\n",
                    player->class,
                    player->items[i].itemName,
                    player->items[i].quantity,
                    player->hp,
                    player->maxhp,
                    player->gold,
                    player->level,
                    player->moxie,
                    player->magic,
                    player->str,
                    player->mana,
                    player->maxmana,
                    player->items[i].spells,
                    player->items[i].check,
                    player->items[i].location,
                    player->items[i].wins,
                    player->items[i].loss);
        }
        else
        {
            // Write only item details in subsequent lines
            fprintf(file, "-,%s,%d,-,-,-,-,-,-,-,-,-,%s,%d,%s,%d,%d\n", player->items[i].itemName, player->items[i].quantity, player->items[i].spells, player->items[i].check, player->items[i].location, player->items[i].wins, player->items[i].loss);

            fprintf(csvfile, "-,%s,%d,-,-,-,-,-,-,-,-,-,%s,%d,%s,%d,%d\n", player->items[i].itemName, player->items[i].quantity, player->items[i].spells, player->items[i].check, player->items[i].location, player->items[i].wins, player->items[i].loss);
        }
    }

    // // Writing item details to file
    // for (int i = 1; i < 10; i++)
    // {
    //     fprintf(file, "-,%s,%d,-\n", player->items[i].itemName, player->items[i].quantity);
    //     fprintf(csvfile, "-,%s,%d,-\n", player->items[i].itemName, player->items[i].quantity);
    // }

    fclose(file);
    fclose(csvfile);
}

int load_game(Player *player, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    char buffer[128];
    // Skipping the header
    fgets(buffer, 128, file);

    // Reading first line with class, HP, MaxHP...MaxMana
    fscanf(file, "%49[^,],%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%49[^,],%d,%49[^,],%d,%d\n",
           player->class,              // %s - string
           player->items[0].itemName,  // %s - string
           &player->items[0].quantity, // %d - integer
           &player->hp,                // %d - integer
           &player->maxhp,             // %d - integer
           &player->level,             // %d - integer
           &player->gold,              // %d - integer
           &player->moxie,             // %d - integer
           &player->magic,             // %d - integer
           &player->str,               // %d - integer
           &player->mana,              // %d - integer
           &player->maxmana,           // %d - integer
           player->items[0].spells,    // %s - string
           &player->items[0].check,    // %d - integer
           player->items[0].location,  // %s - string
           &player->items[0].wins,     // %d - integer
           &player->items[0].loss);    // %d - integer

    // Reading remaining item details
    for (int i = 1; i < 10; i++)
    {
        fscanf(file, "%*[^,],%49[^,],%d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%49[^,],%d,%49[^,],%d,%d\n",
               player->items[i].itemName,  // %s - string
               &player->items[i].quantity, // %d - integer
               // Skipping several integer fields
               player->items[i].spells,   // %s - string
               &player->items[i].check,   // %d - integer
               player->items[i].location, // %s - string
               &player->items[i].wins,    // %d - integer
               &player->items[i].loss);   // %d - integer
    }

    fclose(file);
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("You did not enter the right number of parameters. Ending program.");
        return 1;
    }

    const char *filename = argv[1];
    const char *csvfile = argv[2];
    char playerchoice[50]; // Increased buffer size to accommodate longer item names
    int keeplaying = 1;
    Player player;
    load_game(&player, filename);
    printf("Welcome!\n");
    printf("Your current HP is: %d\n", player.hp);

    while (keeplaying)
    {
        printf("What will you do now?\n");
        fgets(playerchoice, sizeof(playerchoice), stdin);
        playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(playerchoice, "Quit") == 0 || strcmp(playerchoice, "quit") == 0)
        {
            keeplaying = 0;
            printf("You quit the game!\n");
        }
        else if (strcmp(playerchoice, "Save") == 0 || strcmp(playerchoice, "save") == 0)
        {
            save_game(&player, filename, csvfile);
        }
        else if (strcmp(playerchoice, "Battle") == 0 || strcmp(playerchoice, "battle") == 0)
        {
            player.hp += 10;
            printf("You battled some monsters and gained 10 more health!\n");
        }
        // else // Assume it's an item name
        // {
        //     int found = 0;
        //     for (int i = 0; i < 10; i++)
        //     {
        //         if (strcmp(playerchoice, player.items[i].itemName) == 0)
        //         {
        //             player.items[i].quantity++;
        //             printf("You obtained %s!\n", playerchoice);
        //             found = 1;
        //             break;
        //         }
        //     }
        //     if (!found)
        //     {
        //         printf("Item not found!\n");
        //     }
        // }
    }
    return 0;
}