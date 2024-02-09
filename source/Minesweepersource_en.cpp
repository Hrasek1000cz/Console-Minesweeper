#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>


using namespace std;

// Board size
// It is unknown to me why only values that are smaller can be entered and those that are the same or larger cannot be entered
int Board_SIZE = 1000;
// Total mines count
// It is unknown to me why only values that are smaller can be entered and those that are the same or larger cannot be entered
int Total_MINES = 100000;
// Number of mines found
int score = 0;
// Numberof showed fields
int scorepole = 0;



vector<vector<char>> mainBoard(Board_SIZE, vector<char>(Board_SIZE, '#'));
vector<vector<bool>> mineBoard(Board_SIZE, vector<bool>(Board_SIZE, false));
// Showed is used to determine if the field has been revealed before
vector<vector<bool>> showed(Board_SIZE, vector<bool>(Board_SIZE, false));

// First text
void StartingText() {
    cout << endl << "  Welcome to my game Minesweeper";
    cout << "       Version 1.4.2      Author: Hrasek1000cz" << endl;
    cout << endl << "   Goal of the game:" << endl;
    cout << "     You must mark all mined shelves, so the game ends when all mines are marked." << endl;
    cout << endl << "   Gameplay:" << endl;
    cout << "     At the beginning of the game (now), you can choose how big a board you want to play with how many mines." << endl;
    cout << "     Then you will always see your field and write the coordinates you want to reveal. For example: 0 0... 5 1...etc." << endl;
    cout << "     The numbers in the brackets indicate the coordinate values. e.g. 0-8, means that you can only use numbers from 0 to 8." << endl;
    cout << "     If you want to mark a mine, write the coordinates and * and write the coordinates where the mine is. For example..0 5 *..1 7*..But be careful, if you miss you lose :)" << endl;
    cout << "     Warning, if you choose a field larger than 10x10, the auxiliary coordinate numbers above the field will not be aligned." << endl;
    cout << "     You can choose the maximum board size " << Board_SIZE << "x" << Board_SIZE << " and the maximum " << Total_MINES << " min." << endl;
    cout << endl << "   Known bugs:" << endl;
    cout << "     The number of revealed fields in the evaluation will be incorrect if you have revealed a field with the number 0 during the game." << endl;
    cout << "     ..." << endl;
    cout << endl << endl << "  Specify the board size(1-" << Board_SIZE << "):";
}

//Funkce co rozmístí náhodně miny
int PlaceMines() {
    int count = 0;
    while (count < Total_MINES) {
        int x = rand() % Board_SIZE;
        int y = rand() % Board_SIZE;
        if (!mineBoard[x][y]) {
            mineBoard[x][y] = true;
            count++;
        }
    }
    return count;
}

// Funkce co umí spočítat miny okolo daného pole 
int countMinesAround(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < Board_SIZE && newY >= 0 && newY < Board_SIZE && mineBoard[newX][newY]) {
                count++;
            }
        }
    }
    return count;
}

// Funkce zobrazující herní desku v konzoli
void showGameBoard() {
    int NumberRight = 0;
    int NumberTop = 0;
    cout << endl;
    cout << "  ";
    for (int w = 0; w < Board_SIZE; w++) {
        cout << NumberTop;
        cout << " ";
        NumberTop = NumberTop + 1;
    }
    cout << endl;
    cout << endl;
    cout << "  ";
    for (int i = 0; i < Board_SIZE; i++) {
        for (int j = 0; j < Board_SIZE; j++) {
            if (showed[i][j]) {
                cout << mainBoard[i][j] << " ";
            }
            else {
                cout << "# ";
            }
        }
        cout << "  ";
        cout << NumberRight << endl;
        cout << "  ";
        NumberRight = NumberRight + 1;
    }
    cout << endl;
}

//Funkce odhalující pole 
void revealBoard(int x, int y) {
    if (x < 0 || x >= Board_SIZE || y < 0 || y >= Board_SIZE || showed[x][y]) {
        return;
    }

    showed[x][y] = true;
    if (mainBoard[x][y] == ' ') {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                revealBoard(x + i, y + j);
            }
        }
    }
}

// Velká mezera
void FinalSpace() {
    for (int w = 0; w < 20; w++) {
        cout << endl;
    }
}

//Průběh hry
void Game() {
    PlaceMines();
    for (int i = 0; i < Board_SIZE; i++) {
        for (int j = 0; j < Board_SIZE; j++) {
            mainBoard[i][j] = countMinesAround(i, j) + '0';
            if (mineBoard[i][j]) {
                mainBoard[i][j] = '*';
            }
        }
    }

    while (true) {
        showGameBoard();
        string line;
        int x, y;
        char z;
        cout << endl;
        cout << "  Enter a row and column to reveal the field or * in addition to mark the mine (0-" << Board_SIZE - 1 << "), left " << Total_MINES - score << " mines:";
        getline(cin, line);
        while (line.empty()) {
            getline(cin, line);
        }
        istringstream iss(line);
            if (iss >> x >> y) {
                if (iss >> z) {
                    if (z == '*') {
                        if (mineBoard[x][y]) {
                            if (showed[x][y]) {
                                revealBoard(x, y);
                            }
                            else {
                                revealBoard(x, y);
                                score = score + 1;
                                scorepole = scorepole + 1;
                                if (score == Total_MINES) {
                                    FinalSpace();
                                    cout << "  This is what the game looked like:" << endl;
                                    x = 0;
                                    y = 0;
                                    for (int w = 0; w < ((Board_SIZE * Board_SIZE) + Board_SIZE - 1); w++) {
                                        revealBoard(x, y);
                                        if (y < Board_SIZE) {
                                            y = y + 1;
                                        }
                                        else {
                                            y = 0;
                                            x = x + 1;
                                        }
                                    }
                                    showGameBoard();
                                    cout << endl;
                                    cout << "  Congratulations! You won on the board " << Board_SIZE << "x" << Board_SIZE << " with " << Total_MINES << " mines and you only needed to reveal " << scorepole << " field of " << Board_SIZE * Board_SIZE << " field :)" << endl;
                                    cout << endl;
                                    int f;
                                    cout << endl << " To exit, press any number and enter enter:";
                                    cin >> f;
                                    break;
                                }
                            }
                        }
                        else {
                            FinalSpace();

                            cout << "  Uncovered game:" << endl;
                            x = 0;
                            y = 0;
                            for (int w = 0; w < ((Board_SIZE * Board_SIZE) + Board_SIZE - 1); w++) {
                                revealBoard(x, y);
                                if (y < Board_SIZE) {
                                    y = y + 1;
                                }
                                else {
                                    y = 0;
                                    x = x + 1;
                                }
                            }

                            showGameBoard();
                            cout << endl;
                            for (int w = 0; w < 20; w++) {
                                cout << "*";
                            }
                            cout << endl;
                            cout << "  The game is over! You missed! You found " << score << " min of " << Total_MINES << ", you revealed " << scorepole << " shelf of " << (Board_SIZE * Board_SIZE) << ", on the board " << Board_SIZE << "x" << Board_SIZE << " :)" << endl;
                            for (int w = 0; w < 20; w++) {
                                cout << "*";
                            }
                            int g;
                            cout << endl << " To exit, press any number and enter enter:";
                            cin >> g;
                            break;
                        }

                        continue;
                    }
                    else {
                        cout << endl;
                        cout << "  Invalid input. Try again" << endl;
                        cout << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                }
                else { // Pokud se nám nepodaří načíst třetí číslo, zůstane proměnná z nezměněná
                    if (cin.fail() || x < 0 || x >= Board_SIZE || y < 0 || y >= Board_SIZE) {
                        cout << endl;
                        cout << "  Invalid input. Try again" << endl;
                        cout << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;

                    }
                    if (mineBoard[x][y]) {
                        if (showed[x][y]) {
                            continue;
                        }
                        else {
                            FinalSpace();
                            cout << "  Uncovered game:" << endl;
                            x = 0;
                            y = 0;
                            for (int w = 0; w < ((Board_SIZE * Board_SIZE) + Board_SIZE - 1); w++) {
                                revealBoard(x, y);
                                if (y < Board_SIZE) {
                                    y = y + 1;
                                }
                                else {
                                    y = 0;
                                    x = x + 1;
                                }

                            }
                        }

                        showGameBoard();
                        cout << endl;
                        for (int w = 0; w < 20; w++) {
                            cout << "*";
                        }
                        cout << endl;
                        cout << "  The game is over! You stepped on a mine! You found " << score << " mines from " << Total_MINES << ", you found " << scorepole << " shelf from " << (Board_SIZE * Board_SIZE) << ", on the board " << Board_SIZE << "x" << Board_SIZE << " :)v" << endl;
                        for (int w = 0; w < 20; w++) {
                            cout << "*";
                        }
                        int g;
                        cout << endl << " To exit, press any number and enter enter:";
                        cin >> g;
                        break;
                    }
                    if (showed[x][y]) {
                        revealBoard(x, y);
                        if (mainBoard[x][y] == '0') {
                            for (int a = -1; a <= 1; a++) {
                                for (int b = -1; b <= 1; b++) {
                                    int c = x + a;
                                    int d = y + b;
                                    revealBoard(c, d);
                                    scorepole = scorepole + 1;
                                }
                            }
                        }
                    }
                    else {
                        revealBoard(x, y);
                        if (mainBoard[x][y] == '0') {
                            for (int a = -1; a <= 1; a++) {
                                for (int b = -1; b <= 1; b++) {
                                    int c = x + a;
                                    int d = y + b;
                                    revealBoard(c, d);
                                    scorepole = scorepole + 1;
                                }
                            }

                        }
                        scorepole = scorepole + 1;
                    }


                }
                continue;

            }

    }
}

//"Spouštěč"
int main() {
    int r;
    StartingText();
    cin >> Board_SIZE;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl << endl << "  Give the number of min. Move between values -> (1-"<<Board_SIZE*Board_SIZE<<"):";
    cin >> Total_MINES;
    srand(time(0));
    Game();
    return 0;
}
