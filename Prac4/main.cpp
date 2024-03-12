#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>


class Student {
public:
    std::string name;
    int score;


    Student(const std::string &_name, int _score) : name(_name), score(_score) {}
};

class Month {
public:
    std::string name;
    double average_temperature;

    Month(const std::string &_name, double _average_temperature)
            : name(_name), average_temperature(_average_temperature) {}

    bool operator<(const Month &other) const {
        return average_temperature < other.average_temperature;
    }
};

void mySort(std::vector<Month> &months) {
    std::sort(months.begin(), months.end(),
              [](Month a, Month b) -> bool { return a.average_temperature < b.average_temperature; });
}

void maxMark(std::vector<Student> students, int &mark) {
    mark = std::max_element(students.begin(), students.end(),
                            [](const Student &a, const Student &b) -> bool {
                                return a.score < b.score;
                            })->score;
}


int main() {
    // Створюємо масив студентів
    std::vector<Student> students = {
            {"Ivan",   85},
            {"Mariia", 92},
            {"Petro",  78},
            {"Olena",  95},
            {"Andriy", 88}
    };

    // Знаходимо студента з найвищими балами
    auto highest_score_student = std::max_element(students.begin(), students.end(),
                                                  [](const Student &a, const Student &b) -> bool {
                                                      return a.score < b.score;
                                                  });
    std::cout << "highest_score_student: " << highest_score_student->name << std::endl;
    int size_of_chunk = 2;
    int number_of_chunks = students.size() / size_of_chunk + bool(students.size() % size_of_chunk);
//    std::cout << number_of_chunks << "\n";
    std::vector<std::vector<Student>> vectors1;
    for (int i = 0; i < students.size(); i += size_of_chunk) {
        std::vector<Student> sub_vector1;
        int k = 0;
        for (int j = i; j < students.size() && k < size_of_chunk; ++k, ++j) {
            sub_vector1.push_back(students[j]);
        }
        vectors1.push_back(sub_vector1);
    }
    std::vector<std::thread> threads(number_of_chunks);
    std::vector<int> maxMarks(number_of_chunks, 0);
    for (int i = 0; i < number_of_chunks; ++i) {
        threads[i] = std::thread(maxMark, vectors1[i], std::ref(maxMarks[i]));
    }
    for (int i = 0; i < number_of_chunks; ++i) {
        threads[i].join();
    }
    std::sort(maxMarks.begin(), maxMarks.end(), [](int a, int b) -> bool { return a > b; });
    std::cout << "Max mark: " << maxMarks[0] << "\nTask2:\n";
    std::vector<Month> months1 = {
            {"January",  0.5},
            {"February", 1.2},
            {"March",    3.8},
    };
    std::sort(months1.begin(), months1.end(),
              [](Month a, Month b) -> bool { return a.average_temperature < b.average_temperature; });
    for (const auto &month: months1) {
        std::cout << month.name << " " << month.average_temperature << "\n";
    }
    std::cout << "\n";
    std::vector<Month> months2 = {
            {"January",  0.5},
            {"February", 1.2},
            {"March",    3.8},
    };
    size_of_chunk = 6;
    number_of_chunks = months2.size() / size_of_chunk + bool(months2.size() % size_of_chunk);

    std::vector<int> indexs(number_of_chunks, 0);
    std::vector<std::thread> threads2(number_of_chunks);
    std::vector<std::vector<Month>> vectors2;
    for (int i = 0; i < months2.size(); i += size_of_chunk) {
        std::vector<Month> sub_vector1;
        int k = 0;
        for (int j = i; j < months2.size() && k < size_of_chunk; ++k, ++j) {
            sub_vector1.push_back(months2[j]);
        }
        vectors2.push_back(sub_vector1);
    }
    for (int i = 0; i < number_of_chunks; ++i) {
        threads2[i] = std::thread(mySort, std::ref(vectors2[i]));
    }
    for (int i = 0; i < number_of_chunks; ++i) {
        threads2[i].join();
    }
    int months2Size = months2.size();
    std::vector<Month> result;

    while (months2Size > 0) {
        std::vector<Month> *minVec = nullptr;
        int minIndex = -1;
        for (int i = 0; i < number_of_chunks; ++i) {
            if (indexs[i] < vectors2[i].size() &&
                (minVec == nullptr || vectors2[i][indexs[i]] < (*minVec)[indexs[i]])) {
                minVec = &vectors2[i];
                minIndex = i;
            }
        }
        if (minVec != nullptr) {
            result.push_back((*minVec)[indexs[minIndex]]);
            indexs[minIndex]++;
            months2Size--;
        }
    }

    for (const auto &month: result) {
        std::cout << month.name << " " << month.average_temperature << std::endl;
    }

    return 0;
}
