
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/*MACROS*/
//screen dimensions
#define SCREEN_WIDTH 70
#define SCREEN_HEIGHT 20
//initial number of lives
#define INITIAL_LIVES 5

/*STRUCTURES*/
//letter structure, tracks the letter's on-screen position and value
struct Letter {
    int xPositionDeviation; // the deviation from middle column
    int yPositionDeviation; // deviation from top of screen
    char character;         // value
};

/*FILE SCOPE VARIABLES*/
//number of lives player has left
int remainingLives = INITIAL_LIVES;
//player's total score
int totalScore = 0;
//the difficulty selected
int difficultyLevel = 1; // easy = 1, medium = 1.2, hard = 1.5, impossible = 2

/*FUNCTION PROTOTYPES*/
/*Screen Manipulation*/
//sets the cursor position
void setCursorPosition(int x, int y);

/*Level Generation*/
//takes input difficulty to generate, returns 1 if player won, 0 if player lost
int playLevel(int currentLevel);
//generates random letters, including the value and x position
void createRandomLetters(struct Letter *lettersArray, int count);
//checks if all letters in a sequence have crossed the lower border
int areAllLettersOutOfScreen(struct Letter *lettersArray, int count);

/*OUTPUT*/
//prints dashed lines one top and bottom of screen
void displayScreenBorders();
//prints the number of lives left, the score and lives
void displayDashboard(int currentLevel);
//prints a message marking the start of a new level
void displayLevelStartMessage(int currentLevel);
//prints the end message
void displayGameOverMessage();

/*MAIN*/
int main(void) {
    int currentLevel;
    system("cls");

    //loop to start each new level
    for (currentLevel = 1; currentLevel <= 5; currentLevel++) {
        displayLevelStartMessage(currentLevel);
        Sleep(500);
        system("cls");

        if (playLevel(currentLevel) == 0)
            break;
    }
    displayGameOverMessage();
    return 0;
}

/*FUNCTION DEFINITIONS*/
/*Screen Manipulation*/
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x + 1;
    coord.Y = y + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*Level Generation*/
int playLevel(int currentLevel) {
    displayScreenBorders();

    int sequenceLength = (currentLevel + 1) / 2;
    int maxSequences = currentLevel * 5; // a sequence being one letter array
    struct Letter lettersArray[sequenceLength];
    int sleepTime = (100 / currentLevel) * sequenceLength + 50;
    char userInput;

    //one complete level
    int sequenceIndex = 0, letterIndex;
    for (; sequenceIndex < maxSequences; sequenceIndex++) {
        displayDashboard(currentLevel);
        createRandomLetters(lettersArray, sequenceLength);

        while (!areAllLettersOutOfScreen(lettersArray, sequenceLength)) {
            for (letterIndex = 0; letterIndex < sequenceLength; letterIndex++) {
                if (lettersArray[letterIndex].yPositionDeviation >= 2 &&
                    lettersArray[letterIndex].yPositionDeviation <= SCREEN_HEIGHT) {
                    setCursorPosition(lettersArray[letterIndex].xPositionDeviation, lettersArray[letterIndex].yPositionDeviation);
                    putch(lettersArray[letterIndex].character);
                }
            }

            if (kbhit()) {
                userInput = getch();
                for (letterIndex = 0; letterIndex < sequenceLength; letterIndex++) {
                    if (userInput == lettersArray[letterIndex].character) {
                        setCursorPosition(lettersArray[letterIndex].xPositionDeviation, lettersArray[letterIndex].yPositionDeviation);
                        putch(' ');
                        lettersArray[letterIndex].yPositionDeviation = SCREEN_HEIGHT + 1;
                        totalScore++;
                    }
                }
            }

            Sleep(sleepTime);
            for (letterIndex = 0; letterIndex < sequenceLength; letterIndex++) {
                if (lettersArray[letterIndex].yPositionDeviation >= 2 &&
                    lettersArray[letterIndex].yPositionDeviation <= SCREEN_HEIGHT) {
                    setCursorPosition(lettersArray[letterIndex].xPositionDeviation, lettersArray[letterIndex].yPositionDeviation);
                    putch(' ');
                }
                lettersArray[letterIndex].yPositionDeviation++;
                if (lettersArray[letterIndex].yPositionDeviation == SCREEN_HEIGHT) {
                    remainingLives--;
                    if (remainingLives == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    system("cls");
    return 1;
}
void createRandomLetters(struct Letter *lettersArray, int count) {
    srand(time(NULL));
    int i;
    for (i = 0; i < count; i++) {
        lettersArray[i].character = 'a' + rand() % 26;
        lettersArray[i].yPositionDeviation = (rand() % 8) * -1;
    }

    int deviationMagnitude;
    int deviationDirection;
    for (i = 0; i < count; i++) {
        srand(time(NULL));
        deviationMagnitude = rand() % (SCREEN_WIDTH / 2 - 1);
        deviationDirection = rand() % 2;
        if (deviationDirection == 1) {
            lettersArray[i].xPositionDeviation = SCREEN_WIDTH / 2 + deviationMagnitude;
        } else {
            lettersArray[i].xPositionDeviation = SCREEN_WIDTH / 2 - deviationMagnitude;
        }
    }
}
int areAllLettersOutOfScreen(struct Letter *lettersArray, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (lettersArray[i].yPositionDeviation <= SCREEN_HEIGHT) {
            return 0;
        }
    }
    return 1;
}

/*Output*/
void displayScreenBorders() {
    int i = 0;
    setCursorPosition(0, 1);
    for (i = 0; i < SCREEN_WIDTH; i++) {
        putchar('-');
    }
    setCursorPosition(0, SCREEN_HEIGHT + 1);
    for (i = 0; i < SCREEN_WIDTH; i++) {
        putchar('-');
    }
}
void displayDashboard(int currentLevel) {
    setCursorPosition(0, 0);
    printf("Lives left: %d", remainingLives);
    setCursorPosition(SCREEN_WIDTH - 9, 0);
    printf("Score: %d", totalScore);
    setCursorPosition(0, SCREEN_HEIGHT + 2);
    printf("Level: %d", currentLevel);
}
void displayLevelStartMessage(int currentLevel) {
    setCursorPosition(SCREEN_WIDTH / 2 - 3, SCREEN_HEIGHT / 2);
    printf("Level %d", currentLevel);
}
void displayGameOverMessage() {
    setCursorPosition(SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2);
    if (remainingLives == 0) {
        printf("You lost!");
    } else {
        printf("You won!");
    }
    setCursorPosition(SCREEN_WIDTH / 2 - 7, SCREEN_HEIGHT / 2 + 1);
    printf("Your score: %d", totalScore);
    setCursorPosition(0, SCREEN_HEIGHT + 2);
}
