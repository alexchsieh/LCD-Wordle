#include <LiquidCrystal.h>

// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int attemptsLeft = 6;
const String letters[2][13] = {{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"}, {"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}};
String itr[11] = {"_", "_", "_", "_", "_", "*", " ", " ", " ", " ", " "};
String select = "";
int count = 0;
String guess = "";
String correct = "ANGIE";
int occur[5] = {0};

const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0;
const int yAxis = A1;
const int SW_pin = 2;
const int xAxis_left = 100;

// Adjust this if needed
const int xAxis_right = 850;

// Adjust this if needed
const int yAxis_down = 100;

// Adjust this if needed
const int yAxis_up = 850;

const double win_sound[4] = {329.63, 349.23, 440.00, 493.88};
const double lose_sound[4] = {369.99, 349.23, 311.13, 261.63};

const int buzzer = 3;
volatile int counter = 0;
// position variable
int column = 0; // lcd column
int row = 0;    // lcd row
int X;
int Y;

const unsigned long period = 500;
char userInput[5];
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--)
    {
        if ((b_temp >> i) & 0x01)
        {
            digitalWrite(b[i], HIGH);
        }
    }
}

void resetBuffer()
{
    for (int i = 0; i < b_size; i++)
    {
        digitalWrite(b[i], LOW);
    }
}

typedef struct task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct)(int);

} task;

int delay_gcd;

const unsigned short tasksNum = 1;
task tasks[tasksNum];

enum States
{
    INIT,
    MAIN_SCREEN,
    LETTERS_SCREEN,
    RESULT,
    WIN,
    LOSE
} SM1_state;
int SM1_Tick(int state)
{ // State transitions
    switch (state)
    {
    case INIT:
        state = MAIN_SCREEN;
        break;

    case MAIN_SCREEN:
        if (digitalRead(SW_pin) == 0 && !(column == 5 && row == 0))
        {
            count = column;
            column = 0;
            row = 0;
            state = LETTERS_SCREEN;
        }
        else if (column == 5 && row == 0 && digitalRead(SW_pin) == 0 && attemptsLeft > 0 && correct != guess)
        {
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (guess[i] == correct[j])
                    {
                        occur[i]++;
                    }
                }
            }
            // for nested loop through your word, your result divide by number of dups in ans, this solves other edge case so ignore it
            // then subtract after ^, have to check duplicates in both first
            for (int i = 0; i < 5; i++)
            {
                if (guess[i] == correct[i])
                {
                    itr[i + 6] = guess[i];
                }
                else if (guess[i] != correct[i] && occur[i] != 0)
                {
                    itr[i + 6] = "?";
                }
                else
                {
                    itr[i + 6] = "_";
                }
            }
            attemptsLeft--;
            state = MAIN_SCREEN;
        }
        else if (column == 5 && row == 0 && digitalRead(SW_pin) == 0 && attemptsLeft == 0)
        {
            lcd.clear();
            lcd.print("YOU LOSE!");
            lcd.setCursor(0, 1);
            lcd.print("WORD: " + correct);
            state = LOSE;
            for (int i = 0; i < 4; i++)
            {
                tone(buzzer, lose_sound[i]);
                delay(1000);
            }
        }
        else if (column == 5 && row == 0 && digitalRead(SW_pin) == 0 && correct == guess)
        {
            lcd.clear();
            lcd.print("YOU WON!");
            lcd.setCursor(0, 1);
            lcd.print("WORD: " + correct);
            state = WIN;
            for (int i = 0; i < 4; i++)
            {
                tone(buzzer, win_sound[i]);
                delay(1000);
            }
        }
        else
        {
            state = MAIN_SCREEN;
        }
        break;
    case LETTERS_SCREEN:
        if (digitalRead(SW_pin) == 0)
        {
            column = count + 1;
            row = 0;
            state = MAIN_SCREEN;
        }
        else
        {
            state = LETTERS_SCREEN;
            itr[count] = select;
            guess = itr[0] + itr[1] + itr[2] + itr[3] + itr[4];
        }
        break;
    case WIN:
        if (digitalRead(SW_pin) == 0)
        {
            lcd.clear();
            attemptsLeft = 6;
            column = 0;
            row = 0;
            guess = "";
            for (int i = 0; i < 5; i++)
            {
                itr[i] = "_";
                itr[i + 6] = " ";
                occur[i] = 0;
            }
            state = MAIN_SCREEN;
        }
        else
        {
            noTone(buzzer);
            state = WIN;
        }
        break;
    case LOSE:
        if (digitalRead(SW_pin) == 0)
        {
            lcd.clear();
            attemptsLeft = 6;
            column = 0;
            row = 0;
            guess = "";
            for (int i = 0; i < 5; i++)
            {
                itr[i] = "_";
                itr[i + 6] = " ";
                occur[i] = 0;
            }
            state = MAIN_SCREEN;
        }
        else
        {
            noTone(buzzer);
            state = LOSE;
        }
        break;
    }

    switch (state)
    { // state actions
    case INIT:
        break;

    case MAIN_SCREEN:
        // prints your current choices
        lcd.clear();
        lcd.print(itr[0]);
        lcd.print(itr[1]);
        lcd.print(itr[2]);
        lcd.print(itr[3]);
        lcd.print(itr[4]);
        lcd.print(itr[5]);
        lcd.setCursor(15, 0);
        lcd.print(attemptsLeft);

        lcd.setCursor(0, 1);
        lcd.print(itr[6]);
        lcd.print(itr[7]);
        lcd.print(itr[8]);
        lcd.print(itr[9]);
        lcd.print(itr[10]);

        // everything below just adjusts the column and row using joystick
        X = map(analogRead(xAxis), 0, 1000, 0, 100);
        Y = map(analogRead(yAxis), 0, 1000, 0, 100);
        lcd.setCursor(column, row);
        if (X < 30)
        {
            column--;
        }
        else if (X > 70)
        {
            column++;
        }
        if (column < 0)
        {
            column = 5;
        }
        else if (column > 5)
        {
            column = 0;
        }
        if (row < 0)
        {
            row = 1;
        }
        else if (row > 1)
        {
            row = 0;
        }
        break;

    case LETTERS_SCREEN:
        select = letters[row][column];
        // moves the current blinker to the first
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("ABCDEFGHIJKLM");
        lcd.setCursor(0, 1);
        lcd.print("NOPQRSTUVWXYZ");
        lcd.setCursor(0, 0);
        // prints the options and moves cursor to the first

        // everything below just adjusts the column and row using joystick
        X = map(analogRead(xAxis), 0, 1000, 0, 100);
        Y = map(analogRead(yAxis), 0, 1000, 0, 100);
        lcd.setCursor(column, row);
        if (X < 30)
        {
            column--;
        }
        else if (X > 70)
        {
            column++;
        }
        else if (Y < 30)
        {
            row++;
        }
        else if (Y > 70)
        {
            row--;
        }
        if (column < 0)
        {
            column = 12;
        }
        else if (column > 12)
        {
            column = 0;
        }
        if (row < 0)
        {
            row = 1;
        }
        else if (row > 1)
        {
            row = 0;
        }
        break;
    }
    return state;
}

void setup()
{
    for (int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    pinMode(SW_pin, INPUT);
    digitalWrite(SW_pin, HIGH);

    unsigned char i = 0;
    tasks[i].state = SM1_state;
    tasks[i].period = 500;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &SM1_Tick;

    delay_gcd = 500; // GCD
    lcd.begin(16, 2);
    lcd.blink();
}

void loop()
{
    unsigned char i;
    for (i = 0; i < tasksNum; ++i)
    {
        if ((millis() - tasks[i].elapsedTime) >= tasks[i].period)
        {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = millis();
        }
    }
}