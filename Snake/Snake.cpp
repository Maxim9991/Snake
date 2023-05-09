#include <iostream>
#include <ctime>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <string>
#include <fstream>

using namespace std;

//  Maxim Radchenko  //

enum objects { EMPTY, WALL, SNAKE_HEAD, SNAKE_TAIL, FRUIT };
objects** Map;
int main();

int speed = 200;
string name = "";
int score = 0;
int highscore;

string path = "Snake.txt";
string path2 = "Score.txt";
ofstream fout;
ofstream f;

const int m_x = 27;
const int m_y = 12;

class GameObject {
public:
    virtual void update() = 0;
};

class Snake : public GameObject {
public:
    void update();
private:
    int _x = 13;
    int _y = 4;
    int _x_old, _y_old;
    int _direction = 3;
    int _tsize = 0;

    struct Tail {
        int x;
        int y;
    };

    vector<Tail>tails;

    void _KeyBoard();
    void _MoveSnake();
    void _AddTail();
    bool _Ate();
    void _MoveTail();
    void _Collision();
};

void SetCol(int col) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
}

void SetPos(int x, int y) {
    COORD cPos; cPos.Y = y; cPos.X = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cPos);
}

void Snake::_KeyBoard()
{
    /*
        1                up
    2       3     left        right
        4               down
    */

    if (_kbhit())   // Напрямок
    {
        char input = _getch();

        if (input == 'w') {
            _direction = 1;
        }
        else if (input == 'a') {
            _direction = 2;
        }
        else if (input == 'd') {
            _direction = 3;
        }
        else if (input == 's') {
            _direction = 4;
        }
        else if (input == 'p') {
            system("pause");
            system("cls");
        }
        else if (input == (char)27) {
        }
        main();
    }
}

void GameOver() {
    system("cls");

    cout << "\a";

    SetPos(22, 3);
    cout << "Game Over :(" << endl;

    SetPos(20, 5);
    cout << "Your score: " << score << " !" << endl;
}

void Snake::_MoveSnake()
{
    // Напрямок
    _x_old = _x;
    _y_old = _y;

    switch (_direction) {
    case 1: {
        _y--;
        break;
    }
    case 2: {
        _x--;
        break;
    }
    case 3: {
        _x++;
        break;
    }
    case 4: {
        _y++;
        break;
    }
    }

    Map[_y_old][_x_old] = EMPTY;
    Map[_y][_x] = SNAKE_HEAD;
}

void Snake::_AddTail()  // Додавання хвостів
{
    // На початку гри два хвости
    if (_tsize == 0) {
        for (int i = 0; i < 2; i++) {
            tails.push_back(Tail());
            tails[i].x = 19 - i;
            tails[i].y = 4;

            Map[tails[i].y][tails[i].x] = SNAKE_TAIL;   // Розміщення хвостів на полі гри
            _tsize++;   // Збільшення кількості хвостів
        }
    }

    if (_Ate()) {
        tails.push_back(Tail());    // Додавання нових хвостів

        switch (_direction) {
        case 1: {
            tails[_tsize].x = tails[_tsize - 1].x;
            tails[_tsize].y = tails[_tsize - 1].y - 1;
            break;
        }
        case 2: {
            tails[_tsize].x = tails[_tsize - 1].x + 1;
            tails[_tsize].y = tails[_tsize - 1].y;
            break;
        }
        case 3: {
            tails[_tsize].x = tails[_tsize - 1].x - 1;
            tails[_tsize].y = tails[_tsize - 1].y;
            break;
        }
        case 4: {
            tails[_tsize].x = tails[_tsize - 1].x;
            tails[_tsize].y = tails[_tsize - 1].y + 1;
            break;
        }
        }

        score += 5;// Score

        if (speed > 51)
            speed -= 3; // Збільшення швидкості
        else if (speed == 51)
            speed = 51;

        Beep(1480, 200); // Звук
        _tsize++; // Збільшується кількість хвостів
    }
}

bool Snake::_Ate() { // Якщо змія фрукт

    if (_direction == 1 && Map[_y - 1][_x] == FRUIT) { return true; }
    else if (_direction == 2 && Map[_y][_x - 1] == FRUIT) { return true; }
    else if (_direction == 3 && Map[_y][_x + 1] == FRUIT) { return true; }
    else if (_direction == 4 && Map[_y + 1][_x] == FRUIT) { return true; }

    return false;
}

void Snake::_MoveTail() { // Переміщення хвоста
    Map[tails[_tsize - 1].y][tails[_tsize - 1].x] = EMPTY;

    for (int i = _tsize - 1; i != 0; i--) {
        tails[i].x = tails[i - 1].x;
        tails[i].y = tails[i - 1].y;
    }

    tails[0].x = _x_old;
    tails[0].y = _y_old;

    Map[_y_old][_x_old] = SNAKE_TAIL;
}

void file() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    fout.open(path, ofstream::app);
    if (score > highscore)
        fout << "Name: " << name << " - Score: " << score << " (New Record)" << " [ Time " << t.wHour << ":" << t.wMinute << ":" << t.wSecond << " ] [ Date " << t.wDay << "." << t.wMonth << "." << t.wYear << " ]" << endl;
    else
        fout << "Name: " << name << " - Score: " << score << " [ Time " << t.wHour << ":" << t.wMinute << ":" << t.wSecond << " ] [ Date " << t.wDay << "." << t.wMonth << "." << t.wYear << " ]" << endl;
    fout.close();
}

void over(int a, int b) {
    if (a > b) {
        int col;
        for (int i = 0; i < 50; i++) {
            col = rand() % 12 + 1;
            Sleep(35);
            SetPos(7, 10);
            SetCol(col);
            cout << "\t\t     New Highscore!!!";
            Sleep(35);
        }
        f.open(path2, ios_base::out);
        f << score;
        f.close();
    }
}

void outputfile() {
    ifstream fout;
    fout.open(path);
    char ch;
    while (fout.get(ch))
        cout << ch;
    fout.close();
}


void Snake::_Collision() {  // Зіткнення
    if (_x == 0 || _x == 26) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }

    if (_direction == 1 && Map[_y - 1][_x] == WALL || _direction == 4 && Map[_y + 1][_x] == WALL) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }

    // Зіткнення з хвостом
    if (_direction == 1 && Map[_y - 1][_x] == SNAKE_TAIL) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }
    else if (_direction == 2 && Map[_y][_x - 1] == SNAKE_TAIL) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }
    else if (_direction == 3 && Map[_y][_x + 1] == SNAKE_TAIL) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }
    else if (_direction == 4 && Map[_y + 1][_x] == SNAKE_TAIL) {
        GameOver();
        file();
        over(score, highscore);
        _getch();
        main();
    }
}

void Snake::update() { // Обновлення
    _KeyBoard();
    _AddTail();
    _Collision();
    _MoveSnake();
    _MoveTail();
}

class Fruit : public GameObject {
public:
    void update();
private:
    int _x;
    int _y;

    void _spawnFruit();
    bool _check_for_fruit();
};


void Fruit::_spawnFruit() {     // Появлення нових фруктів
    int x, y;
    x = 1 + rand() % 25;
    y = 1 + rand() % 10;

    Map[y][x] = FRUIT;
}

bool Fruit::_check_for_fruit() {    // провірка на фрукти
    for (int i = 1; i < m_y - 1; i++) {
        for (int j = 1; j < m_x - 1; j++) {
            if (Map[i][j] == FRUIT) {
                return true;
            }
        }
    }
    return false;
}

void Fruit::update() {  // якщо на полі є фрукт, новий не появиться
    if (!_check_for_fruit()) {
        _spawnFruit();
    }
}

void Scene(vector<GameObject*>& updates) {
    //Збереження підкласу в вектор, щоб зробити новий вказівник цього підкласу
    if (updates.size() == 0) {
        updates.push_back(new Snake());
        updates.push_back(new Fruit());
    }
    // Оновлення кожного підкласу
    for (vector<GameObject*>::iterator itr = updates.begin(), end = updates.end(); itr != end; itr++) {
        (*itr)->update();
    }
}

void draw() {
    COORD cor = { 0,0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cor);

    for (int i = 0; i < m_y; i++) {
        for (int j = 0; j < m_x; j++) {
            if (Map[i][j] == WALL)
            {
                SetCol(1); cout << (char)176;  SetCol(7);
            }
            else if (Map[i][j] == EMPTY)
            {
                cout << " ";
            }
            else if (Map[i][j] == SNAKE_HEAD)
            {
                SetCol(10); cout << "O"; SetCol(7);
            }
            else if (Map[i][j] == SNAKE_TAIL)
            {
                SetCol(8); cout << "o"; SetCol(7);
            }
            else if (Map[i][j] == FRUIT)
            {
                SetCol(14); cout << "+"; SetCol(7);
            }
        }
        cout << endl;
    }
}

void Score() {
    ifstream f;
    f.open(path2);
    f >> highscore;
    f.close();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, (WORD)((4 << 4) | 0));
    SetPos(28, 0);
    cout << "Name: " << name;

    SetConsoleTextAttribute(hConsole, (WORD)((12 << 4) | 0));
    SetPos(28, 1);
    cout << "Score: " << score;

    SetConsoleTextAttribute(hConsole, (WORD)((8 << 4) | 0));
    SetPos(28, 2);
    if (score > highscore)
        cout << "Highscore: " << highscore << "  New Highscore!!!";
    else
        cout << "Highscore: " << highscore;

    SetConsoleTextAttribute(hConsole, (WORD)((9 << 4) | 0));
    SetPos(28, 3);
    if (speed < 201 && speed > 176)
        cout << "Level: 1";
    else if (speed < 175 && speed > 151)
        cout << "Level: 2";
    else if (speed < 150 && speed > 126)
        cout << "Level: 3";
    else if (speed < 125 && speed > 101)
        cout << "Level: 4";
    else if (speed < 100 && speed > 76)
        cout << "Level: 5";
    else if (speed < 75 && speed == 51) {
        cout << "Level: Max";
        speed = 51;
    }

    SetConsoleTextAttribute(hConsole, (WORD)((9 << 4) | 0));
    SetPos(28, 6);
    cout << "Pause 'P'";

    SetConsoleTextAttribute(hConsole, (WORD)((9 << 4) | 0));
    SetPos(28, 7);
    cout << "Exit 'Esc'";

    SYSTEMTIME st;
    GetLocalTime(&st);
    SetConsoleTextAttribute(hConsole, (WORD)((8 << 4) | 0));
    SetPos(28, 10);
    cout << "Time " << st.wHour << ":" << st.wMinute << ":" << st.wSecond;
    SetPos(28, 11);
    cout << "Date " << st.wDay << "." << st.wMonth << "." << st.wYear;
}

void SnakeGame() {
    SetCol(2);
    cout << "                  __    __    __    __                            " << endl;
    cout << "                 /  \\  /  \\  /  \\  /  \\                       " << endl;
    SetCol(8);
    cout << "________________"; SetCol(2); cout << "/  "; SetCol(8); cout << "__"; SetCol(2); cout << "\\/ "; SetCol(8); cout << " __"; SetCol(2); cout << "\\/ "; SetCol(8); cout << " __"; SetCol(2); cout << "\\/ "; SetCol(8); cout << " __"; SetCol(2); cout << "\\"; SetCol(8); cout << "_______________________" << endl;
    cout << "_______________"; SetCol(2); cout << "/  /"; SetCol(8); cout << "__"; SetCol(2); cout << "/  /"; SetCol(8); cout << "__"; SetCol(2); cout << "/  /"; SetCol(8); cout << "__"; SetCol(2); cout << "/  /"; SetCol(8); cout << "__________________________   " << endl;
    SetCol(2);
    cout << "               | / \\   / \\   / \\   / \\  \\___                 " << endl;
    cout << "               |/   \\_/   \\_/   \\_/   \\"; SetCol(14); cout << "   o "; SetCol(2); cout << "\\                " << endl;
    cout << "                                       \\____/"; SetCol(4);  cout << "--<             " << endl;
    SetCol(7);


}

int main() {
    srand(time(0));

    //system("mode con cols=60 lines=26");    // Розмір консольного вікна

    /* FILE
        -- Name \ Score --
    */

    // Поле
    Map = new objects * [m_y];

    for (int i = 0; i < m_y; i++) {
        Map[i] = new objects[m_x];
    }

    // Заповнненя поля
    for (int i = 0; i < m_y; i++) {   // N
        for (int j = 0; j < m_x; j++) {  // M
            if (i == 0 || i == m_y - 1 || j == 0 || j == m_x - 1) {
                Map[i][j] = WALL;
            }
            else {
                Map[i][j] = EMPTY;
            }
        }
    }

    // Обновлення класів 
    vector<GameObject*>updates;

    ifstream f;
    f.open(path2);
    f >> highscore;
    f.close();

    int c = 1;
    int ch = 1;
    while (true) {
        system("cls");

        SnakeGame();

        if (c == 1)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        else
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "\t\t\tStart Game\n";

        if (c == 2)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        else
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "\t\t\tPlayer List\n";

        if (c == 3)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        else
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "\t\t\tControl\n\n";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "\t\t\tHighscore: " << highscore << endl;

        ch = _getch();
        switch (ch) {
        case 224: {
            switch (_getch()) {
            case 72: c > 1 ? c-- : c = 3; Beep(1480, 200); break;
            case 80: c < 3 ? c++ : c = 1; Beep(1480, 200); break;
            }
        }
        }
        if (ch == 13) {
            if (c == 1) {
                Beep(1480, 200);
                cout << "\t\t\tName: ";
                cin >> name;
                system("cls");
                score = 0;
                speed = 200;
                while (1) { // 1 - true
                    draw();
                    Scene(updates);
                    Sleep(speed);
                    Score();
                }
            }
            else if (c == 2) {
                Beep(1480, 200);
                system("cls");
                outputfile();
                cout << "\n-- Press Any Button --";
                _getch();
            }
            else if (c == 3) {
                system("cls");
                SetCol(7);
                cout << "-----------------------------------\n";
                SetCol(14);
                cout << "   Management\n";
                SetCol(7);
                cout << "-----------------------------------\n";
                SetCol(14);
                cout << " -> Press 'A' to the left\n";
                cout << " -> Press 'D' to the right\n";
                cout << " -> Press 'W' to the up\n";
                cout << " -> Press 'S' to the down\n";
                SetCol(7);
                cout << "-----------------------------------\n";
                SetCol(14);
                cout << " -> Press 'P' pause\n";
                cout << " -> Press 'Esc' main menu";
                SetCol(7);
                _getch();
            }
        }
    }
    return 0;
}