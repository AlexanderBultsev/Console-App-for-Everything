#include <iostream>
#include <vector>
#include <Windows.h>
#include <iomanip>
#include <conio.h>

using namespace std;

int MinReprAge = 10, // Не самые оптимальные (наверное)!
MaxReprAge = 30,
AverageAge = 20,
MaxAge = 50,
MinFoodPred = 2,
MinFoodHerb = 20,
GrassRecPerc = 67,
Grass = 3000,
CataclysmChance = 1,
Xmax = 20,
Ymax = 20,
CountHerb = 3,
CountPred = 1,
Day = 0;

vector <vector <int> > Population{
    //   X   Y   age type
        {Xmax / 4, Ymax / 4, 20, 0},
        {Xmax * 3 / 4, Ymax / 4, 20, 0},
        {Xmax / 4, Ymax * 3 / 4, 20, 0},
        {Xmax * 3 / 4, Ymax * 3 / 4, 20, 1},
};

void ShowSettings() {
    system("cls");
    cout << "ЭМУЛЯЦИЯ РАЗВИТИЯ ПОПУЛЯЦИЙ ЖИВОТНЫХ\n\n"
        << "Чтобы продолжить с использованием значений по умолчанию - нажмите 1\n"
        << "Чтобы задать случайные параметры - нажмите 2\n"
        << "Чтобы задать параметны вручную - нажмите 3\n"
        << "Чтобы выйти из программы - нажмите esc\n\n";
}

void print(vector <vector <int> > vec) {
    if (vec.size()) {
        vector <vector <int>> mas(Xmax);
        for (int i = 0; i < Xmax; i++)
            mas[i].resize(Ymax);

        for (int i = 0; i < vec.size(); i++) {
            for (int j = 0; j < vec[i].size(); j++)
                cout << setw(4) << vec[i][j];
            cout << "\n";
            mas[vec[i][0]][vec[i][1]] = vec[i][3] + 1;
        }
        cout << "\n";

        for (int i = 0; i < Xmax; i++) {
            for (int j = 0; j < Ymax; j++)
                if (mas[i][j])
                    cout << mas[i][j] - 1 << " ";
                else
                    cout << "  ";
            cout << "\n";
        }
    }
}

void ShowParameters() {
    cout << "Размер поля: " << Xmax << "x" << Ymax << "\n"
        << "Минимальный репродуктивный возраст животных: " << MinReprAge << "\n"
        << "Максимальный репродуктивный возраст животных: " << MaxReprAge << "\n"
        << "Количество травоядных необходимое хищнику на месяц: " << MinFoodPred << "\n"
        << "Количество травы необходимое травоядному на месяц: " << MinFoodHerb << "\n"
        << "Восстановление травы за год: " << GrassRecPerc << "%\n"
        << "Шанс катаклизма: " << CataclysmChance << "%\n"
        << "Количество травы на поле: " << Grass << "\n"
        << "Количество травоядных: " << CountHerb << "\n"
        << "Количество хищников: " << CountPred << "\n";
}

void ShowWork(vector <vector <int> > Population) {
    system("cls");
    cout << "ЭМУЛЯЦИЯ РАЗВИТИЯ ПОПУЛЯЦИЙ ЖИВОТНЫХ\n\n";
    ShowParameters();
    cout << "\nДень с начала эмуляции: " << Day << "\n";
    print(Population);
}

bool EnoughGrass = true;
bool EnoughHerbs = true;

void StartEmulation(vector <vector <int> > Population) {
    while (1) {

        if (_kbhit())
            if (_getch() == 27) {
                cout << "\nЭмуляция принудительно завершена\n";
                return;
            }

        if (Population.size() == 0) {
            ShowWork(Population);
            cout << "\nЭмуляция завершена\nПопуляции вымерли\n";
            return;
        }

        // завернуть в отдельную функцию
        if (!(Day % 30)) {
            Grass -= (MinFoodHerb * CountHerb);
            Grass += (MinFoodHerb * CountHerb) * GrassRecPerc / 100;
            if (CountHerb)
                EnoughGrass = Grass / (MinFoodHerb * CountHerb);
            ShowWork(Population);
            Sleep(500);
        }

        if (!(Day % 360)) {
            if (CountPred)
                EnoughHerbs = CountHerb / (MinFoodPred * CountPred);
            for (int i = 0; i < Population.size(); i++)
                Population[i][2]++;
        }

        int i = 0;
        while (i < Population.size()) {

            if ((!EnoughHerbs) && (Population[i][3]) && (pow(pow(100, -MaxAge), Population[i][2]) > rand() % 100)) { // доработать формулы, а то все умирают неправильно!
                CountPred--;
                Population.erase(Population.begin() + i);
                if (i >= Population.size())
                    break;
            }

            else if ((!EnoughGrass) && !(Population[i][3]) && (pow(pow(100, -MaxAge), Population[i][2]) > rand() % 100)) {
                CountHerb--;
                Population.erase(Population.begin() + i);
                if (i >= Population.size())
                    break;
            }

            else {
                Population[i][0] += rand() % 3 - 1;
                if (Population[i][0] < 0)
                    Population[i][0] = 0;
                if (Population[i][0] >= Xmax)
                    Population[i][0] = Xmax - 1;

                Population[i][1] += rand() % 3 - 1;
                if (Population[i][1] < 0)
                    Population[i][1] = 0;
                if (Population[i][1] >= Ymax)
                    Population[i][1] = Ymax - 1;



                if (Population[i][3]) {
                    int k = 0;
                    while (k < Population.size()) {
                        if ((abs(Population[i][0] - Population[k][0]) < 2) && (abs(Population[i][1] - Population[k][1]) < 2) && !(Population[k][3])) {
                            CountHerb--;
                            Population.erase(Population.begin() + k);
                            if (i >= Population.size())
                                break;
                        }
                        else
                            k++;
                    }
                }

                i++;
                if (i >= Population.size())
                    break;
            }
        }
        ShowWork(Population);
        Sleep(20);
        Day++;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    ShowSettings();

    while (1)
        switch (_getch())
        {
        case (27): {
            cout << "\nПРОГРАММА ЗАВЕРШЕНА\n";
            return 0;
            break;
        }
        case (49): {
            ShowSettings();
            ShowParameters();
            StartEmulation(Population); // нужно чтобы новый создавался!
            break;
        }
        case (50): {
            ShowSettings();
            Population.clear();
            int n = rand() % 16 + 4;
            CountHerb = 0;
            CountPred = 0;
            for (int i = 0; i < n; i++) {
                int type = rand() % 3 / 2; // соотношение хищников и травоядных
                Population.push_back({ rand() % Xmax, rand() % Ymax, rand() % (2 * AverageAge), type });
                if (type) CountPred++;
                else CountHerb++;
            }
            ShowParameters();
            StartEmulation(Population);
            break;
        }
        case (51): {
            system("cls");
            ShowSettings();
            cout << "В разработке\n"; // надо доделать!
            break;
        }
        }

    return 1;
}