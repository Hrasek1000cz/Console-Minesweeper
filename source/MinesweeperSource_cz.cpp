#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>


using namespace std;

// Velikost desky
//Není mi známo proč se mohou zadat jen hodnoty, které jsou menší a ty stejné nebo větší nejdou zadat
int Board_SIZE = 1000;
// Celkový počet min
//Není mi známo proč se mohou zadat jen hodnoty, které jsou menší a ty stejné nebo větší nejdou zadat
int Total_MINES = 100000;
// Počet nalezených min
int score = 0;
// Počet odhalených polí
int scorepole = 0;



vector<vector<char>> mainBoard(Board_SIZE, vector<char>(Board_SIZE, '#'));
vector<vector<bool>> mineBoard(Board_SIZE, vector<bool>(Board_SIZE, false));
// Showed je používáno k zjišťování, jestli už bylo dříve pole odhaleno
vector<vector<bool>> showed(Board_SIZE, vector<bool>(Board_SIZE, false));

// Úvodní text
void StartingText() {
    cout << endl << "  Vitej v me hre Hledani min";
    cout << "       Verze 1.4.2      Autor: Hrasek1000cz" << endl;
    cout << endl << "   Cil hry:" << endl;
    cout << "     Musis oznacit vsechna zaminovana policka, tudiz hra konci az vsechny miny budou oznacene." << endl;
    cout << endl << "   Ovladani:" << endl;
    cout << "     Na zacatku hry (ted), si muzete zadanim cisel vybrat na jak velkem poli s kolika minama budete hrat." << endl;
    cout << "     Pak uvidite vzdy sve pole a budete psat souradnice, ktera chcete odhalit. napr: 0 0.. 5 1...atd" << endl;
    cout << "     Cisla v zavorce udavaji hodnoty na souradnice. napr 0-8, znamena ze muzete pouzit jen cisla od 0 do 8." << endl;
    cout << "     Jestlize chcete oznacit minu, napiste souradnice a * a napiste souradnice kde mina je. Napr..0 5 *..1 7*... Ale pozor, jestli se netrefite prohrajete :)" << endl;
    cout << "     Varovani, pokud si zvolite pole vetsi nez 10x10, tak pomocna souradnicova cisla nahore nad polem nebudou zarovnana." << endl;
    cout << "     Muzete si vybrat maximalne hraci desku o velikosti "<< Board_SIZE <<"x" << Board_SIZE << " a maximalne " << Total_MINES << " min." << endl;
    cout << endl << "   Zname chyby:" << endl;
    cout << "     Pocet odhalenych poli ve vyhodnoceni bude vadny, jestlize jste behem hry odkryli pole s cislem 0." << endl;
    cout << "     ..." << endl;
    cout << endl << endl << "  Udej velikost desky(0-"<<Board_SIZE<<"):";
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
        cout << "  Zadej radek a sloupec pro odhaleni pole nebo * navic pro oznaceni miny(0-" << Board_SIZE - 1 << "), zbyva " << Total_MINES - score << " min: ";
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
                                    cout << "  Takto vypadala hra:" << endl;
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
                                    cout << "  Gratuluji! Vyhral jsi na na hraci plose " << Board_SIZE << "x" << Board_SIZE << " s " << Total_MINES << " minami a stacilo ti odhalit pouze " << scorepole << " poli z " << Board_SIZE * Board_SIZE << " poli :)" << endl;
                                    cout << endl;
                                    int f;
                                    cout << endl << " K ukonceni zmackni jakekoliv cislo a enter:";
                                    cin >> f;
                                    break;
                                }
                            }
                        }
                        else {
                            FinalSpace();

                            cout << "  Odkryti hry:" << endl;
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
                            cout << "  Hra skoncila! Netrefils! Nasel jsi " << score << " min z " << Total_MINES << ", odhalil jsi " << scorepole << " policek z " << (Board_SIZE * Board_SIZE) << ", na hraci plose " << Board_SIZE << "x" << Board_SIZE << "  :)" << endl;
                            for (int w = 0; w < 20; w++) {
                                cout << "*";
                            }
                            int g;
                            cout << endl << " K ukonceni zmackni jakekoliv cislo a enter:";
                            cin >> g;
                            break;
                        }

                        continue;
                    }
                    else {
                        cout << endl;
                        cout << "  Neplatny vystup. Zkus to znovu" << endl;
                        cout << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                }
                else { // Pokud se nám nepodaří načíst třetí číslo, zůstane proměnná z nezměněná
                    if (cin.fail() || x < 0 || x >= Board_SIZE || y < 0 || y >= Board_SIZE) {
                        cout << endl;
                        cout << "  Neplatny vystup. Zkus to znovu" << endl;
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
                            cout << "  Odkryti hry:" << endl;
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
                        cout << "  Hra skoncila! Slapl jsi na minu! Nasel jsi " << score << " min z "<< Total_MINES <<", odhalil jsi " << scorepole << " policek z " << (Board_SIZE * Board_SIZE) << ", na hraci plose "<< Board_SIZE <<"x"<< Board_SIZE <<"  :)" << endl;
                        for (int w = 0; w < 20; w++) {
                            cout << "*";
                        }
                        int g;
                        cout << endl << " K ukonceni zmackni jakekoliv cislo a enter:";
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
    cout << endl << endl << "  Udej pocet min. Pohybujte se mezi hodnotami -> (1-"<<Board_SIZE*Board_SIZE<<"):";
    cin >> Total_MINES;
    srand(time(0));
    Game();
    return 0;
}
