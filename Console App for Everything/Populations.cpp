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
    {"AverageAge", 0},
    {"MaxAge", 0},
    {"Day", 0},
    {"MinFoodPred", 0},
    {"MinFoodHerb", 0},
    {"CountHerb", 0},
    {"CountPred", 0},
    {"GrassRecPerc", 0},
    {"Grass", 0},
    {"CataclysmChance", 0},
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

void print(vector <vector <int> > vec) {
    if (vec.size()) {
        vector <vector <int>> mas(Parameters["Xmax"]);
        for (int i = 0; i < Parameters["Xmax"]; i++)
            mas[i].resize(Parameters["Ymax"]);

        for (int i = 0; i < vec.size(); i++) {
            for (int j = 0; j < vec[i].size(); j++)
                cout << setw(4) << vec[i][j];
            cout << "\n";
            mas[vec[i][0]][vec[i][1]] = vec[i][3] + 1;
        }
        cout << "\n";

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

void ShowParameters() {
    cout << "������ ����: " << Parameters["Xmax"] << "x" << Parameters["Ymax"] << "\n"
        << "����������� �������������� ������� ��������: " << Parameters["MinReprAge"] << "\n"
        << "������������ �������������� ������� ��������: " << Parameters["MaxReprAge"] << "\n"
        << "���������� ���������� ����������� ������� �� �����: " << Parameters["MinFoodPred"] << "\n"
        << "���������� ����� ����������� ����������� �� �����: " << Parameters["MinFoodHerb"] << "\n"
        << "�������������� ����� �� ���: " << Parameters["GrassRecPerc"] << "%\n"
        << "���� ����������: " << Parameters["CataclysmChance"] << "%\n"
        << "���������� ����� �� ����: " << Parameters["Grass"] << "\n"
        << "���������� ����������: " << Parameters["CountHerb"] << "\n"
        << "���������� ��������: " << Parameters["CountPred"] << "\n";
}

void ShowWork(vector <vector <int> > Population) {
    system("cls");
    cout << "�������� �������� ��������� ��������\n\n";
    ShowParameters();
    cout << "\n���� � ������ ��������: " << Parameters["Day"] << "\n";
    print(Population);
}

bool EnoughGrass = true;
bool EnoughHerbs = true;

void StartEmulation(vector <vector <int> > Population) {
    while (1) {

        if (_kbhit())
            if (_getch() == 27) {
                cout << "\n�������� ������������� ���������\n";
                return;
            }

        if (Population.size() == 0) {
            ShowWork(Population);
            cout << "\n�������� ���������\n��������� �������\n";
            return;
        }

        // ��������� � ��������� �������
        if (!(Parameters["Day"] % 30)) {
            Parameters["Grass"] -= (Parameters["MinFoodHerb"] * Parameters["CountHerb"]);
            Parameters["Grass"] += (Parameters["MinFoodHerb"] * Parameters["CountHerb"]) * Parameters["GrassRecPerc"] / 100;
            if (Parameters["CountHerb"])
                EnoughGrass = Parameters["Grass"] / (Parameters["MinFoodHerb"] * Parameters["CountHerb"]);
            ShowWork(Population);
            Sleep(500);
        }

        if (!(Parameters["Day"] % 360)) {
            if (Parameters["CountPred"])
                EnoughHerbs = Parameters["CountHerb"] / (Parameters["MinFoodPred"] * Parameters["CountPred"]);
            for (int i = 0; i < Population.size(); i++)
                Population[i][2]++;
        }

        int i = 0;
        while (i < Population.size()) {

            bool UnDead = true;

            if ((!EnoughHerbs) && (Population[i][3])) {
                float p = pow(99, pow(float(Population[i][2]) / float(Parameters["MaxAge"]), 3));
                if (p > rand() % 100) {
                    Parameters["CountPred"]--;
                    Population.erase(Population.begin() + i);
                    if (i >= Population.size())
                        UnDead = false();
                    break;
                }
            }

            if ((!EnoughGrass) && !(Population[i][3])) {
                float p = pow(99, pow(float(Population[i][2]) / float(Parameters["MaxAge"]), 3));
                if (p > rand() % 100) {
                    Parameters["CountHerb"]--;
                    Population.erase(Population.begin() + i);
                    if (i >= Population.size())
                        UnDead = false;
                    break;
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
        //ShowWork(Population);
        //Sleep(10);
        Parameters["Day"]++;
    }
}

// 0 - ����������� ������ ���� ����������
// 1 - ����������� �����������
// 2 - ���������� ������� ��������� �������� (�������� ������� �������)
// 3 - ����������� ����������

// ... - ����������� ��� �� QT


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    ShowSettings();

    while (1)
        switch (_getch())
        {
        case (27): {
            cout << "\n��������� ���������\n";
            return 0;
            break;
        }
        case (49): {
            Parameters["MinReprAge"] = 10;
            Parameters["MaxReprAge"] = 30;
            Parameters["AverageAge"] = 20;
            Parameters["MaxAge"] = 50;
            Parameters["Day"] = 0;
            Parameters["MinFoodPred"] = 2;
            Parameters["MinFoodHerb"] = 20;
            Parameters["CountHerb"] = 3;
            Parameters["CountPred"] = 1;
            Parameters["GrassRecPerc"] = 67;
            Parameters["Grass"] = 1000;
            Parameters["CataclysmChance"] = 1;
            Parameters["Xmax"] = 20;
            Parameters["Ymax"] = 20;

            Population = {
                    {Parameters["Xmax"] / 4, Parameters["Ymax"] / 4, 20, 0},
                    {Parameters["Xmax"] * 3 / 4, Parameters["Ymax"] / 4, 20, 0},
                    {Parameters["Xmax"] / 4, Parameters["Ymax"] * 3 / 4, 20, 0},
                    {Parameters["Xmax"] * 3 / 4, Parameters["Ymax"] * 3 / 4, 20, 1},
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
            Parameters["AverageAge"] = 20;
            Parameters["MaxAge"] = 50;
            Parameters["Day"] = 0;
            Parameters["MinFoodPred"] = 2;
            Parameters["MinFoodHerb"] = 20;
            Parameters["CountHerb"] = 1;
            Parameters["CountPred"] = 1;
            Parameters["GrassRecPerc"] = 67;
            Parameters["Grass"] = 1000;
            Parameters["CataclysmChance"] = 1;
            Parameters["Xmax"] = 20;
            Parameters["Ymax"] = 20;

            Population = { // ������� ��������
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % (2 * Parameters["AverageAge"]), 0 },
                    { rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % (2 * Parameters["AverageAge"]), 1 }
            };
            for (int i = 0; i < n - 2; i++) {
                int type = rand() % 4 / 3; // ����������� �������� � ���������� 1 � 3
                Population.push_back({ rand() % Parameters["Xmax"], rand() % Parameters["Ymax"], rand() % (2 * Parameters["AverageAge"]), type });
                if (type) Parameters["CountPred"]++;
                else Parameters["CountHerb"]++;
            }
            ShowSettings();
            ShowParameters();
            StartEmulation(Population);
            break;
        }
        case (51): {
            system("cls");
            ShowSettings();
            cout << "� ����������\n"; // ���� ��������!
            break;
        }
        }

    return 1;
}