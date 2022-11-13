#include <iostream>
#include <vector>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include <map>

using namespace std;

map <string, int> Parameters{
    {"MinReprAge", 0},
    {"MaxReprAge", 0},
    {"BirthProbability", 0},
    {"MaxAge", 0},
    {"Day", 0},
    {"MinFoodPred", 0},
    {"MinFoodHerb", 0},
    {"CountHerb", 0},
    {"CountPred", 0},
    {"GrassRec", 0},
    {"Grass", 0},
    {"Xmax", 0},
    {"Ymax", 0}
};

vector <vector <int> > Population;

void ShowSettings() {
    system("cls");
    cout << "ЭМУЛЯЦИЯ РАЗВИТИЯ ПОПУЛЯЦИЙ ЖИВОТНЫХ\n\n"
        << "Чтобы продолжить с использованием значений по умолчанию - нажмите 1\n"
        << "Чтобы задать случайные параметры - нажмите 2\n"
        << "Чтобы задать параметны вручную - нажмите 3\n"
        << "Чтобы выйти из программы - нажмите esc\n\n";
}

void ShowParameters() {
    cout << "Размер поля: " << Parameters["Xmax"] << "x" << Parameters["Ymax"] << "\n"
        << "Минимальный репродуктивный возраст животных: " << Parameters["MinReprAge"] << "\n"
        << "Максимальный репродуктивный возраст животных: " << Parameters["MaxReprAge"] << "\n"
        << "Вероятность размножения: " << Parameters["BirthProbability"] << "%\n"
        << "Максимальный возраст животных: " << Parameters["MaxAge"] << "\n"
        << "Количество травоядных необходимое хищнику на месяц: " << Parameters["MinFoodPred"] << "\n"
        << "Количество травы необходимое травоядному на месяц: " << Parameters["MinFoodHerb"] << "\n"
        << "Количество травы на поле: " << Parameters["Grass"] << "\n"
        << "Восстановление травы за год: " << Parameters["GrassRec"] << "%\n"
        << "Количество травоядных: " << Parameters["CountHerb"] << "\n"
        << "Количество хищников: " << Parameters["CountPred"] << "\n";
}

void ShowWork(vector <vector <int> > Population, int mode) {
    system("cls");
    cout << "ЭМУЛЯЦИЯ РАЗВИТИЯ ПОПУЛЯЦИЙ ЖИВОТНЫХ\n\n"
        << "Для завершения эмуляции нажмите esc\n"
        << "Для смены вида отображения нажмите tab\n"
        << "Для смены частоты отображения нажмите space\n\n";
    ShowParameters();
    cout << "\nДень с начала эмуляции: " << Parameters["Day"] << "\n";

    if (Population.size()) {
        vector <vector <int>> mas(Parameters["Xmax"]);
        for (int i = 0; i < Parameters["Xmax"]; i++)
            mas[i].resize(Parameters["Ymax"]);

        if (mode) {
            for (int i = 0; i < Population.size(); i++) {
                for (int j = 0; j < Population[i].size(); j++)
                    cout << setw(4) << Population[i][j];
                cout << "\n";
                mas[Population[i][0]][Population[i][1]] = Population[i][3] + 1;
            }
            cout << "\n";
        }

        else {
            for (int i = 0; i < Population.size(); i++) {
                mas[Population[i][0]][Population[i][1]] = Population[i][3] + 1;
            }

            for (int i = 0; i < Parameters["Xmax"]; i++) {
                for (int j = 0; j < Parameters["Ymax"]; j++)
                    if (mas[i][j])
                        cout << mas[i][j] - 1 << " ";
                    else
                        cout << "  ";
                cout << "\n";
            }
        }
    }
}

void StartEmulation(vector <vector <int> > Population) {

    bool EnoughGrass = true;
    bool EnoughHerbs = true;
    int DeadHerbsCount = 0;
    int OutputMode = 0;
    int OutputSpeed = 1;

    while (1) {

        if (_kbhit()) {
            switch (_getch())
            {
            case 27: {
                system("cls");
                ShowWork(Population, OutputMode);
                cout << "\nЭмуляция принудительно завершена\n";
                cout << "Нажмите любую кнопку для перехода к настройкам\n";
                int _ = _getch();
                ShowSettings();
                return;
                break;
            }
            case 9: {
                OutputMode = (OutputMode + 1) % 2;
                break;
            }
            case 32: {
                OutputSpeed = (OutputSpeed + 1) % 3;
            }
            }
        }

        if (Population.size() == 0) {
            ShowWork(Population, OutputMode);
            cout << "\nЭмуляция завершена\nПопуляции вымерли\n";
            cout << "Нажмите любую кнопку для перехода к настройкам\n";
            int _ = _getch();
            ShowSettings();
            return;
        }

        // Определение условия голодания животных
        if (!(Parameters["Day"] % 30)) {
            Parameters["Grass"] -= (Parameters["MinFoodHerb"] * Parameters["CountHerb"]);
            Parameters["Grass"] += (Parameters["MinFoodHerb"] * Parameters["CountHerb"]) * Parameters["GrassRec"] / 100;
            if (Parameters["Grass"] < 0)
                Parameters["Grass"] = 0;
            if (Parameters["CountHerb"])
                EnoughGrass = Parameters["Grass"] / (Parameters["MinFoodHerb"] * Parameters["CountHerb"]);

            if (Parameters["CountPred"])
                EnoughHerbs = DeadHerbsCount / (Parameters["MinFoodPred"] * Parameters["CountPred"]);
            DeadHerbsCount = 0;
            ShowWork(Population, OutputMode);
            Sleep(500);
        }

        if (!(Parameters["Day"] % 360)) {
            for (int i = 0; i < Population.size(); i++)
                Population[i][2]++;
        }

        int i = 0;
        while (i < Population.size()) {

            bool UnDead = true;

            // Обработка голодания и размножения хищников
            if (Population[i][3]) {
                if (!EnoughHerbs) {
                    float p = pow(99, pow(float(Population[i][2]) / float(Parameters["MaxAge"]), 3));
                    if (p > rand() % 100) {
                        Parameters["CountPred"]--;
                        Population.erase(Population.begin() + i);
                        if (i >= Population.size())
                            UnDead = false;
                        break;
                    }
                }
                else 
                    if ((Population[i][2] - Parameters["MaxReprAge"] - Parameters["MinReprAge"]) <= Parameters["MaxReprAge"] - Parameters["MinReprAge"])
                        if (Parameters["BirthProbability"] > rand() % 100) {
                            Parameters["CountPred"]++;
                            Population.push_back({ Population[i][0], Population[i][1], 0, 1 });
                        }
            }

            // Обработка голодания и размножения травоядных
            if (!Population[i][3]) {
                if (!EnoughGrass) {
                    float p = pow(99, pow(float(Population[i][2]) / float(Parameters["MaxAge"]), 3));
                    if (p > rand() % 100) {
                        Parameters["CountHerb"]--;
                        DeadHerbsCount++;
                        Population.erase(Population.begin() + i);
                        if (i >= Population.size())
                            UnDead = false;
                        break;
                    }
                }
                else
                    if ((Population[i][2] - Parameters["MaxReprAge"] - Parameters["MinReprAge"]) <= Parameters["MaxReprAge"] - Parameters["MinReprAge"])
                        if (Parameters["BirthProbability"] > rand() % 100) {
                            Parameters["CountHerb"]++;
                            Population.push_back({ Population[i][0], Population[i][1], 0, 0 });
                        }
            }

            if (UnDead) {
                Population[i][0] += rand() % 3 - 1;
                if (Population[i][0] < 0)
                    Population[i][0] = 0;
                if (Population[i][0] >= Parameters["Xmax"])
                    Population[i][0] = Parameters["Xmax"] - 1;

                Population[i][1] += rand() % 3 - 1;
                if (Population[i][1] < 0)
                    Population[i][1] = 0;
                if (Population[i][1] >= Parameters["Ymax"])
                    Population[i][1] = Parameters["Ymax"] - 1;

                if (Population[i][3]) {
                    int k = 0;
                    while (k < Population.size()) {
                        if ((abs(Population[i][0] - Population[k][0]) < 2) && (abs(Population[i][1] - Population[k][1]) < 2) && !(Population[k][3])) {
                            Parameters["CountHerb"]--;
                            DeadHerbsCount++;
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
        
        if (OutputSpeed == 1) {
            ShowWork(Population, OutputMode);
            Sleep(100);
        }
        if (OutputSpeed == 2) {
            ShowWork(Population, OutputMode);
            Sleep(5);
        }

        Parameters["Day"]++;
    }
}


int main() {
    HWND hWnd = GetForegroundWindow();
    ShowWindow(hWnd, SW_MAXIMIZE);
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    ShowSettings();

    while (1)
        switch (_getch())
        {
        case (27): {
            system("cls");
            cout << "ЭМУЛЯЦИЯ РАЗВИТИЯ ПОПУЛЯЦИЙ ЖИВОТНЫХ\n\n"
                << "ПРОГРАММА ЗАВЕРШЕНА\n";
            return 0;
            break;
        }
        case (49): {
            Parameters["MinReprAge"] = 10;
            Parameters["MaxReprAge"] = 30;
            Parameters["BirthProbability"] = 1;
            Parameters["MaxAge"] = 50;
            Parameters["Day"] = 1;
            Parameters["MinFoodPred"] = 1;
            Parameters["MinFoodHerb"] = 20;
            Parameters["CountHerb"] = 4;
            Parameters["CountPred"] = 1;
            Parameters["GrassRec"] = 33;
            Parameters["Grass"] = 1000;
            Parameters["Xmax"] = 20;
            Parameters["Ymax"] = 20;

            Population = {
                    {Parameters["Xmax"] / 4, Parameters["Ymax"] / 4, 10, 0},
                    {Parameters["Xmax"] * 3 / 4, Parameters["Ymax"] / 4, 10, 0},
                    {Parameters["Xmax"] / 4, Parameters["Ymax"] * 3 / 4, 10, 0},
                    {Parameters["Xmax"] * 3 / 4, Parameters["Ymax"] * 3 / 4, 10, 0},
                    {Parameters["Xmax"] / 2, Parameters["Ymax"] / 2, 10, 1}
            };
            ShowSettings();
            ShowParameters();
            StartEmulation(Population);
            break;
        }
        case (50): {
            int n = rand() % 17 + 4; // от 4 до 20

            Parameters["MinReprAge"] = 10;
            Parameters["MaxReprAge"] = 30;
            Parameters["BirthProbability"] = 1;
            Parameters["MaxAge"] = 50;
            Parameters["Day"] = 1;
            Parameters["MinFoodPred"] = 2;
            Parameters["MinFoodHerb"] = 20;
            Parameters["CountHerb"] = 1;
            Parameters["CountPred"] = 1;
            Parameters["GrassRec"] = 33;
            Parameters["Grass"] = 1000;
            Parameters["Xmax"] = 20;
            Parameters["Ymax"] = 20;

            Population = { // минимум эмуляции
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % Parameters["MaxAge"], 0 },
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % Parameters["MaxAge"], 1 }
            };
            for (int i = 0; i < n - 2; i++) {
                int type = rand() % 4 / 3; // соотношение хищников и травоядных 1 к 3
                Population.push_back({ rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % Parameters["MaxAge"], type });
                if (type) Parameters["CountPred"]++;
                else Parameters["CountHerb"]++;
            }
            ShowSettings();
            ShowParameters();
            StartEmulation(Population);
            break;
        }
        case (51): {
            ShowSettings();
            cout << "Задайте параметры\n\n";
            cout << "Минимальный репродуктивный возраст животных: ";
            cin >> Parameters["MinReprAge"];
            cout << "Максимальный репродуктивный возраст животных: ";
            cin >> Parameters["MaxReprAge"];
            cout << "Вероятность размножения: ";
            cin >> Parameters["BirthProbability"];
            cout << "Максимальный возраст животных: ";
            cin >> Parameters["MaxAge"];
            cout << "Количество травоядных необходимое хищнику на месяц: ";
            cin >> Parameters["MinFoodPred"];
            cout << "Количество травы необходимое травоядному на месяц: ";
            cin >> Parameters["MinFoodHerb"];
            cout << "Количество травы на поле: ";
            cin >> Parameters["Grass"];
            cout << "Процент восстановления травы за год: ";
            cin >> Parameters["GrassRec"];
            cout << "Ширина поля: ";
            cin >> Parameters["Xmax"];
            cout << "Высота поля: ";
            cin >> Parameters["Ymax"];

            cout << "Задайте животных в виде 'Xкоордината  Yкоордината  Возраст  Тип(0 травоядное; 1 хищник)'\n"
                << "(Чтобы завершить ввод введите '-1')\n";
            int X, Y, A, T;
            while (1) {

                cin >> X;
                if (X == -1)
                    break;
                cin >> Y >> A >> T;

                if ((abs(2 * X - Parameters["Xmax"]) - Parameters["Xmax"] <= 0)
                    and (abs(2 * Y - Parameters["Ymax"]) - Parameters["Ymax"] <= 0)
                    and (A >= 0)
                    and (abs(2 * T - 1) - 1 <= 0)){
                    Population.push_back({ X, Y, A, T });
                    if (T)
                        Parameters["CountPred"]++;
                    else
                        Parameters["CountHerb"]++;

                }
                else
                    cout << "Введены неправильные параметры! Животное не добавлено на поле!\n";

            }
            
            cout << "Все параметры эмуляции заданы\n"
                << "Нажмите enter, чтобы начать эмуляцию";
            if (_getch() == 13)
                StartEmulation(Population);
            break;
        }
        }

    return 1;
}