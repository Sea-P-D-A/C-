#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

using namespace std;

class strana {
public:
    string name, continent, stolic, gos_stroy;
    int S, people_size;

    strana(const std::string& name = "", const std::string& continent = "", const std::string& stolic = "",
        const std::string& gos_stroy = "", int S = 0, int people_size = 0)
        : name(name), continent(continent), stolic(stolic), gos_stroy(gos_stroy),
        S(S), people_size(people_size) {}

    std::string serialize() const {
        std::ostringstream oss;
        oss << "Название: " << name << ",континент: " << continent << ",столица: " << stolic
            << ",площадь: " << S << ",население: " << people_size << ",гос.строй: " << gos_stroy << "\n";

        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& out, const strana& country) {
        out << country.serialize();
        return out;
    }

    friend std::istream& operator>>(std::istream& in, strana& country) {
        string temp;
        in >> temp >> country.name >> temp >> country.continent >> temp >> country.stolic >> temp >> country.S >> temp >> country.people_size >> temp >> country.gos_stroy;
        //
        //getline(in, country.name, ',');
        //getline(in, country.continent, ',');
        //getline(in, country.stolic, ',');
        //in >> country.S;
        //in.ignore(); // Игнорируем запятую
        //in >> country.people_size;
        //in.ignore(); // Игнорируем запятую
        //getline(in, country.gos_stroy);

        

        return in;
    }

    int get_people() { return people_size; }
    string get_continent() { return continent; }
    void set_people(int a) { people_size = a; }
};

void print_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    std::cout << "Содержимое " << filename << ":\n";
    strana temp;
    while (file >> temp) {
        std::cout << temp.name << " " << temp.people_size << "\n";
    }
    file.close();
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::ifstream osn("osn.txt");
    if (!osn.is_open()) {
        std::cerr << "Не удалось открыть osn.txt" << std::endl;
        return 1;
    }

    std::ofstream file1("file1.txt");
    std::ofstream file2("file2.txt");

    strana temp, temp2;

    // Чтение из основного файла и распределение по двум файлам
    while (osn >> temp) {
        file1 << temp;
        if (osn >> temp2) file2 << temp2;
    }
    file1.close();
    file2.close();
    osn.close();

    print_file_contents("file1.txt");
    print_file_contents("file2.txt");

    bool inputSwitch = false;
    int iteration = 0;

    while (true) {
        std::ifstream input1(inputSwitch ? "result.txt" : "file1.txt");
        std::ifstream input2(inputSwitch ? "result2.txt" : "file2.txt");
        std::ofstream output1(inputSwitch ? "file1.txt" : "result.txt");
        std::ofstream output2(inputSwitch ? "file2.txt" : "result2.txt");

        std::cout << "Iteration " << iteration++ << ": "
            << (inputSwitch ? "result.txt" : "file1.txt") << " and "
            << (inputSwitch ? "result2.txt" : "file2.txt") << " -> "
            << (inputSwitch ? "file1.txt" : "result.txt") << " and "
            << (inputSwitch ? "file2.txt" : "result2.txt") << std::endl;

        bool hasData1 = static_cast<bool>(input1 >> temp);
        bool hasData2 = static_cast<bool>(input2 >> temp2);

        if (!hasData1 && !hasData2) {
            input1.close();
            input2.close();
            break;
        }

        input1.clear();
        input1.seekg(0, std::ios::beg);
        input2.clear();
        input2.seekg(0, std::ios::beg);

        bool eof1 = !hasData1;
        bool eof2 = !hasData2;

        if (!eof1 && !(input1 >> temp)) eof1 = true;
        if (!eof2 && !(input2 >> temp2)) eof2 = true;

        while (!eof1 && !eof2) {
            if (temp.get_people() <= temp2.get_people()) {
                output1 << temp;
                if (!(input1 >> temp)) eof1 = true;
            }
            else {
                output1 << temp2;
                if (!(input2 >> temp2)) eof2 = true;
            }
        }

        while (!eof1) {
            output1 << temp;
            if (!(input1 >> temp)) eof1 = true;
        }

        while (!eof2) {
            output1 << temp2;
            if (!(input2 >> temp2)) eof2 = true;
        }

        input1.close();
        input2.close();
        output1.close();
        output2.close();

        print_file_contents(inputSwitch ? "file1.txt" : "result.txt");
        print_file_contents(inputSwitch ? "file2.txt" : "result2.txt");

        inputSwitch = !inputSwitch;
    }

    std::string finalFile = inputSwitch ? "file1.txt" : "result.txt";
    std::cout << "Финальный файл: " << finalFile << std::endl;
    print_file_contents(finalFile);

    return 0;
}

//int main() {
//    setlocale(LC_ALL, "Russian");
//    ifstream osn("osn.txt");
//    ofstream file1("file1.txt");
//    ofstream file2("file2.txt");
//
//    strana temp, temp2;
//
//    // Чтение из основного файла и распределение по двум файлам
//    while (osn >> temp) {
//        file1 << temp;
//        if (osn >> temp2) file2 << temp2;
//    }
//    file1.close();
//    file2.close();
//    osn.close();
//
//    bool inputSwitch = false;
//
//    while (true) {
//        ifstream input1(inputSwitch ? "result.txt" : "file1.txt");
//        ifstream input2(inputSwitch ? "result2.txt" : "file2.txt");
//        ofstream output1(inputSwitch ? "file1.txt" : "result.txt");
//        ofstream output2(inputSwitch ? "file2.txt" : "result2.txt");
//
//        if (!(input1 >> temp) && !(input2 >> temp2)) {
//            input1.close();
//            input2.close();
//            break; // Прерываем цикл, если оба файла пусты
//        }
//        input1.close();
//        input2.close();
//        input1.open(inputSwitch ? "result.txt" : "file1.txt");
//        input2.open(inputSwitch ? "result2.txt" : "file2.txt");
//
//        bool eof1 = !(input1 >> temp);
//        bool eof2 = !(input2 >> temp2);
//
//        while (!eof1 && !eof2) {
//            if (temp.get_people() <= temp2.get_people()) {
//                output1 << temp;
//                eof1 = !(input1 >> temp);
//            }
//            else {
//                output1 << temp2;
//                eof2 = !(input2 >> temp2);
//            }
//        }
//
//        while (!eof1) {
//            output1 << temp;
//            eof1 = !(input1 >> temp);
//        }
//
//        while (!eof2) {
//            output1 << temp2;
//            eof2 = !(input2 >> temp2);
//        }
//
//        input1.close();
//        input2.close();
//        output1.close();
//        output2.close();
//
//        inputSwitch = !inputSwitch;
//    }
//
//    // Переименовываем последний активный файл вывода в конечный результат
//    string finalFile = inputSwitch ? "file1.txt" : "result.txt";
//    //rename(finalFile.c_str(), "final_result.txt");
//    cout << finalFile.c_str();
//    return 0;
//}

//int main() {
//    setlocale(LC_ALL, "Russian");
//    list<strana> countries;
//
//    ifstream osn("osn.txt");
//    std::fstream file1("file1.txt", ios::out);
//    std::fstream file2("file2.txt", ios::out);
//    std::fstream file3("file1.txt");
//    std::fstream file4("file2.txt");
//
//
//    strana temp;
//    strana temp2;
//
//    while (!osn.eof()) {
//        osn >> temp;
//        file1 << temp;
//        if (!osn.eof()) {
//            osn >> temp;
//            file2 << temp;
//        }
//    }
//    int size_s = 1;
//    file1.close();
//    file2.close();
//    file3.close();
//    file4.close();
//    bool flag = true;
//    
//    if (flag) {
//        file1.open("file1.txt", ios::in);
//        file2.open("file2.txt", ios::in);
//        file3.open("resule.txt", ios::out);
//        file4.open("resule2.txt", ios::out); 
//        while (!file1.eof() && !file2.eof()) {
//
//        }
//        flag = false;
//    }
//    else {
//        file1.open("file1.txt", ios::out);
//        file2.open("file2.txt", ios::out);
//        file3.open("resule.txt", ios::in);
//        file4.open("resule2.txt", ios::in);
//        while (!file3.eof() && !file4.eof()) {
//
//        }
//        flag = true;
//    }
//
//    return 0;
//}
