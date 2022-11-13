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
    cout << "�������� �������� ��������� ��������\n\n"
        << "����� ���������� � �������������� �������� �� ��������� - ������� 1\n"
        << "����� ������ ��������� ��������� - ������� 2\n"
        << "����� ������ ��������� ������� - ������� 3\n"
        << "����� ����� �� ��������� - ������� esc\n\n";
}

void ShowParameters() {
    cout << "������ ����: " << Parameters["Xmax"] << "x" << Parameters["Ymax"] << "\n"
        << "����������� �������������� ������� ��������: " << Parameters["MinReprAge"] << "\n"
        << "������������ �������������� ������� ��������: " << Parameters["MaxReprAge"] << "\n"
        << "����������� �����������: " << Parameters["BirthProbability"] << "%\n"
        << "������������ ������� ��������: " << Parameters["MaxAge"] << "\n"
        << "���������� ���������� ����������� ������� �� �����: " << Parameters["MinFoodPred"] << "\n"
        << "���������� ����� ����������� ����������� �� �����: " << Parameters["MinFoodHerb"] << "\n"
        << "���������� ����� �� ����: " << Parameters["Grass"] << "\n"
        << "�������������� ����� �� ���: " << Parameters["GrassRec"] << "%\n"
        << "���������� ����������: " << Parameters["CountHerb"] << "\n"
        << "���������� ��������: " << Parameters["CountPred"] << "\n";
}

void ShowWork(vector <vector <int> > Population, int mode) {
    system("cls");
    cout << "�������� �������� ��������� ��������\n\n"
        << "��� ���������� �������� ������� esc\n"
        << "��� ����� ���� ����������� ������� tab\n"
        << "��� ����� ������� ����������� ������� space\n\n";
    ShowParameters();
    cout << "\n���� � ������ ��������: " << Parameters["Day"] << "\n";

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
                cout << "\n�������� ������������� ���������\n";
                cout << "������� ����� ������ ��� �������� � ����������\n";
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
            cout << "\n�������� ���������\n��������� �������\n";
            cout << "������� ����� ������ ��� �������� � ����������\n";
            int _ = _getch();
            ShowSettings();
            return;
        }

        // ����������� ������� ��������� ��������
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

            // ��������� ��������� � ����������� ��������
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

            // ��������� ��������� � ����������� ����������
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
            cout << "�������� �������� ��������� ��������\n\n"
                << "��������� ���������\n";
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
            int n = rand() % 17 + 4; // �� 4 �� 20

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

            Population = { // ������� ��������
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % Parameters["MaxAge"], 0 },
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % Parameters["MaxAge"], 1 }
            };
            for (int i = 0; i < n - 2; i++) {
                int type = rand() % 4 / 3; // ����������� �������� � ���������� 1 � 3
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
            cout << "������� ���������\n\n";
            cout << "����������� �������������� ������� ��������: ";
            cin >> Parameters["MinReprAge"];
            cout << "������������ �������������� ������� ��������: ";
            cin >> Parameters["MaxReprAge"];
            cout << "����������� �����������: ";
            cin >> Parameters["BirthProbability"];
            cout << "������������ ������� ��������: ";
            cin >> Parameters["MaxAge"];
            cout << "���������� ���������� ����������� ������� �� �����: ";
            cin >> Parameters["MinFoodPred"];
            cout << "���������� ����� ����������� ����������� �� �����: ";
            cin >> Parameters["MinFoodHerb"];
            cout << "���������� ����� �� ����: ";
            cin >> Parameters["Grass"];
            cout << "������� �������������� ����� �� ���: ";
            cin >> Parameters["GrassRec"];
            cout << "������ ����: ";
            cin >> Parameters["Xmax"];
            cout << "������ ����: ";
            cin >> Parameters["Ymax"];

            cout << "������� �������� � ���� 'X����������  Y����������  �������  ���(0 ����������; 1 ������)'\n"
                << "(����� ��������� ���� ������� '-1')\n";
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
                    cout << "������� ������������ ���������! �������� �� ��������� �� ����!\n";

            }
            
            cout << "��� ��������� �������� ������\n"
                << "������� enter, ����� ������ ��������";
            if (_getch() == 13)
                StartEmulation(Population);
            break;
        }
        }

    return 1;
}