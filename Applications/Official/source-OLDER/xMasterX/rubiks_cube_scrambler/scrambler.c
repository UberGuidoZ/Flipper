/*
Authors: Tanish Bhongade and RaZe
*/

#include <stdio.h>
#include <furi.h>
#include <gui/gui.h>
#include "furi_hal_random.h"
#include <input/input.h>
#include <gui/elements.h>
#include "scrambler.h"

// 6 moves along with direction
char moves[6] = {'R', 'U', 'F', 'B', 'L', 'D'};
char dir[4] = {' ', '\'', '2'};
const int SLEN = 20;
#define RESULT_SIZE 100
// Structure which holds main scramble
struct GetScramble
{
	char mainScramble[25][3];
};
struct GetScramble a; // Its object

// Function prototypes to avoid bugs
void scrambleReplace();
void genScramble();
void valid();
int getRand(int upr, int lwr);
char *printData();
void writeToFile();

// Main function
/* int main(){
	genScramble ();//Calling genScramble
	scrambleReplace();//Calling scrambleReplace
	valid();//Calling valid to validate the scramble
	printData ();//Printing the final scramble
	//writeToFile();//If you want to write to a file, please uncomment this

	return 0;
} */

void genScramble()
{
	// Stage 1
	for (int i = 0; i < SLEN; i++)
	{
		strcpy(a.mainScramble[i], "00");
	}
	// This makes array like this 00 00 00.......
}

void scrambleReplace()
{
    // Stage 2
    // Actual process begins here

    // Initialize the mainScramble array with all the possible moves
    for (int i = 0; i < SLEN; i++)
    {
        a.mainScramble[i][0] = moves[furi_hal_random_get() % 6];
        a.mainScramble[i][1] = dir[furi_hal_random_get() % 3];
    }

    // Perform the Fisher-Yates shuffle
    for (int i = 6 - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char temp[3];
        strcpy(temp, a.mainScramble[i]);
        strcpy(a.mainScramble[i], a.mainScramble[j]);
        strcpy(a.mainScramble[j], temp);
    }

    // Select the first 10 elements as the scramble, using only the first three elements of the dir array
    for (int i = 0; i < SLEN; i++)
    {
        a.mainScramble[i][1] = dir[furi_hal_random_get() % 3];
    }
     for (int i = 1; i < SLEN; i++) {
        while ( a.mainScramble[i][0] == a.mainScramble[i - 2][0] || a.mainScramble[i][0] == a.mainScramble[i - 1][0]) {
            a.mainScramble[i][0] = moves[furi_hal_random_get()%5];
        }
    }
}







// Let this function be here for now till I find out what is causing the extra space bug in the scrambles
void remove_double_spaces(char *str) {
  int i, j;
  int len = strlen(str);
  for (i = 0, j = 0; i < len; i++, j++) {
    if (str[i] == ' ' && str[i + 1] == ' ') {
      i++;
    }
    str[j] = str[i];
  }
  str[j] = '\0';
}
char *printData()
{
    static char result[RESULT_SIZE];
    int offset = 0;
    for (int loop = 0; loop < SLEN; loop++)
    {
        offset += snprintf(result + offset, RESULT_SIZE - offset, "%s ", a.mainScramble[loop]);
    }
    remove_double_spaces(result);
    return result;
}


