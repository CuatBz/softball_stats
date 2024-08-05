#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 20
#define PLYR_AMNT 18            //amount of players

struct Player_Info {
    int player_num;
    char fname[NAME_LEN];
    char lname[NAME_LEN];
    int bats;
    int hits;
    int walks;
    int rbi;
    float bat_avg;          //batting average
};

int text_to_num (char * text_num);      //converts a string to an integer

//A program that reads stats about softball players and processes the info
int main (void)
{
    FILE * file;
    struct Player_Info rec [PLYR_AMNT];     //holds info for amount of players
    _Bool is_space, in_word;                //checks if c is space | checks if currently in a word
    int c, col_count, temp_num;             //will read file | count columns | stores converted ints
    char temp[NAME_LEN];                    //holds temp strings

    //Open file where stats are saved
    if ((file = fopen("stats.txt", "r")) == NULL)
    {
        printf("Unable to read file. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    //Set values to zero to avoid errors
    for (int k = 0; k < PLYR_AMNT; k++)
    {
        rec[k].bats = 0;
        rec[k].hits = 0;
        rec[k].walks = 0;
        rec[k].rbi = 0;
    }
    
    int index;          //stores player number
    c = ' ';            //give c a safe value

    while (c != EOF)
    {
        //reset values everytime loop starts (new line in the file)
        in_word = is_space = 0;
        col_count = 0;
        int i = 0;          //this is to keep track of char position in temp array

        //Will keep reading until newline
        while ((c = fgetc(file)) != EOF && c != '\n')
        {
            if (isspace(c))
            {
                //If c was in a word and now there's whitespace then process the data
                if (in_word == 1)
                {
                    temp[i] = '\0';         //add null terminator to make it string

                    //the first col is player number which doubles as index
                    if (col_count == 0)
                    {
                        temp_num = text_to_num(temp);
                        index = temp_num - 1;
                        rec[index].player_num = temp_num;
                    }

                    else if (col_count == 1)
                    {
                        strncpy(rec[index].fname, temp, NAME_LEN);
                    }

                    else if (col_count == 2)
                    {
                        strncpy(rec[index].lname, temp, NAME_LEN);
                    }

                    else if (col_count == 3)
                    {
                        temp_num = text_to_num(temp);
                        rec[index].bats += temp_num;
                    }

                    else if (col_count == 4)
                    {
                        temp_num = text_to_num(temp);
                        rec[index].hits += temp_num;
                    }
                    
                    else if (col_count == 5)
                    {
                        temp_num = text_to_num(temp);
                        rec[index].walks += temp_num;
                    }
                    
                    col_count++;            //increas col num after reaching whitespace and processing input
                    in_word = 0;            //no longer in a word
                }

                is_space = 1;           //let program know whitespace was read
            }

            else
            {
                //if data is found and we were in space we're reading a new part of the data
                if (is_space == 1)
                {
                    i = 0;              //resets char position for temp array
                    is_space = 0;       //not space
                    in_word = 1;        //inside a word
                    temp[i++] = c;      //store c to temp array
                }

                else
                {
                    in_word = 1;        //keep letting the program know we're still in a word
                    temp[i++] = c;      //store c to temp
                }
                
            }
        }

        //special case for last char as the white is exited when newline is found
        if (c == '\n')
        {
            temp[i] = '\0';
            temp_num = text_to_num(temp);
            rec[index].rbi += temp_num;         //save rbi
        }
    }
    
    fclose(file);

    // calculate average for the players and print players info
    float average;
    printf("\n");
    for (int k = 0; k < PLYR_AMNT; k++)
    {
        average = (float) rec[k].hits / rec[k].bats;
        rec[k].bat_avg = average;
        printf("Player Number #%d | %s %s\n", rec[k].player_num, rec[k].fname, rec[k].lname);
        printf("Statistics:\n");
        printf("Bats: %d | Hits: %d | Walks: %d | RBIs: %d\n", rec[k].bats, rec[k].hits, rec[k].walks, rec[k].rbi);
        printf("Batting Average: %.2f\n\n", rec[k].bat_avg);
    }

    //array for team stats
    float team_stats[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

    //add all player info to team stats
    for (int i = 0; i < PLYR_AMNT; i++)
    {
        team_stats[0] += rec[i].bats;
        team_stats[1] += rec[i].hits;
        team_stats[2] += rec[i].walks;
        team_stats[3] += rec[i].rbi;
        team_stats[4] += rec[i].bat_avg;
    }
    
    //divide by player amount to get averages
    for (int i = 0; i < 5; i++)
        team_stats[i] /= PLYR_AMNT;

    //Print Team Stats
    printf("\nTeam's Average Statistics:\n");
    printf("Bats: %.2f | Hits: %.2f | Walks: %.2f | RBIs: %.2f\n", team_stats[0], team_stats[1], team_stats[2], team_stats[3]);
    printf("Batting Average: %.2f\n", team_stats[4]);

    return 0;
}

//Function to convert a string to integers
int text_to_num (char * text_num)
{
    int length = strlen(text_num);
    int result, temp, index;

    result = index = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        temp = text_num[index] - '0';
        if (i > 0)
            temp = (i * 10) * temp;

        result += temp;
        index++;
    }

    return result;
}