#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "cyclic_dependencies_API.h"


struct csv::ComplexCell {
    std::string firstArg;
    std::string secondArg;
};

//проверка на количество аргументов в ячейке, возвращает аргументы
csv::ComplexCell check_ops(const csv::table_t &csv, const std::string &nextCell) {
    std::string firstArg;
    std::string secondArg;

    for (int j = 0; j < nextCell.length(); j++) {
        if (nextCell[j] == '+' || nextCell[j] == '-' || nextCell[j] == '*' || nextCell[j] == '/') {
            firstArg = nextCell.substr(0, j);
            secondArg = nextCell.substr(j + 1, nextCell.length());
            break;
        }
    }
        if (secondArg.empty()) {
            firstArg = nextCell;
        }
    csv::ComplexCell complexCell{firstArg, secondArg};
    return complexCell;
}

//удаляет знак равенства в начале строки
std::string remove_equal_char(const std::string &token) {
    std::string result = token;
    if (token.compare(0, 1, "=") == 0) {
        result = token.substr(1, token.size() - 1);
    }
    return result;
}

csv::table_t get_table(const std::string &path) {
    std::ifstream file(path, std::ios::in);
    std::string input;
    std::getline(file, input);

    size_t pos = input.find(',');

    csv::table_t table;
    csv::cell_data_t cellData = "";
    csv::cell_address_t cellAddress = "";
    input.erase(0, pos + 1);

    //заполняю вектор названиями столбцов
    std::vector<std::string> vec;
    while ((pos = input.find(',')) != std::string::npos) {
        cellData = input.substr(0, pos);
        vec.push_back(cellData);
        input.erase(0, pos + 1);
    }
    vec.push_back(input);

    const int tableLineSize = vec.size();
    std::string currentLineValue;
    while (file >> input) {
        std::stringstream ss(input);
        for (int i = 0; getline(ss, cellData, ','); i++) {

            //исключаю названия строк из таблицы
            int remainder = i % tableLineSize;
            if (remainder == 0 && i != tableLineSize) {
                currentLineValue = cellData;
                continue;
            }

            cellAddress = vec.at(i - 1) + currentLineValue;
            table[cellAddress] = cellData;
        }
    }
    return table;
}

std::vector<calculation_path>
csv::find_loops(const calculation_path &calculationPath, const csv::table_t &table) {
    std::vector<std::string> path = calculationPath.getPath();
    std::string currentCell = path.back();
    std::string nextCell{table.at(currentCell)};
    //карта, в которой отмечено какие ячейки уже были пройдены
    std::map<std::string, bool> visited = calculationPath.getVisited();
    visited[currentCell] = true;
    nextCell = remove_equal_char(nextCell);
    std::vector<calculation_path> paths;
    calculation_path result{path, visited};

    //проходим путь, пока не дойдем до цифры либо пока не найдем зависимость (строка 140)
    while (!std::isdigit(nextCell[0])) {

        //проверка на разветвление, если в ячейке два аргумента, создаются два отдельных пути и для каждого рекурсивно
        //вызывается find_loops()
        csv::ComplexCell complexCell = check_ops(table, nextCell);
        if (!complexCell.secondArg.empty()) {

            //если эта ячейка уже была пройдена.
            bool before = false;
            if (visited.count(complexCell.firstArg) != 0) {
                before = true;
            }
            //готовим переменные для первого аргумента,
            visited[complexCell.firstArg] = true;
            path.push_back(complexCell.firstArg);
            calculation_path temp{path, visited};
            std::vector<calculation_path> tempPaths;
            //если выявлены зависимости, то они записываются в итоговый вектор paths
            tempPaths = csv::find_loops(temp, table);
            paths.insert(paths.end(), tempPaths.begin(), tempPaths.end());

            //убираем из переменных первый аргумент, добавляем второй аргумент
            path.pop_back();
            path.push_back(complexCell.secondArg);
            if (!before) {
                visited[complexCell.firstArg] = false;
            }
            visited[complexCell.secondArg] = true;
            calculation_path temp2{path, visited};
            //если выявлены зависимости, то они записываются в итоговый вектор paths
            tempPaths = csv::find_loops(temp2, table);
            paths.insert(paths.end(), tempPaths.begin(), tempPaths.end());
            break;
        }

        if (!visited[nextCell]) {
            visited[nextCell] = true;
            path.push_back(nextCell);
        }

        currentCell = table.at(nextCell);
        currentCell = remove_equal_char(currentCell);

        nextCell = currentCell;

        //если ячейка уже была пройдена, значит мы нашли зависимость
            if (visited[currentCell]) {
                path.push_back(nextCell);
                result.setPath(path);
                result.setVisited(visited);
                result.setCyclicDependencyFlag(true);
            }

        if (result.isCyclicDependency()) {
            paths.push_back(result);
            break;
        }
    }
    return paths;
};

//убрать повторяющиеся циклы, обрезать пути до мест зацикливания
std::vector<std::vector<std::string>> remove_duplicates(const std::vector<calculation_path> &pathsLoopedVector) {
    std::vector<std::vector<std::string>> toDisplay;
    for (const auto &item: pathsLoopedVector) {
        std::vector<std::string> path = item.getPath();
        std::string cycleStart = path.back();
        std::vector<std::string> newPath;
        for (unsigned i = path.size(); i > 0; i--) {
            newPath.push_back(path[i - 1]);
            if (path[i - 1] == cycleStart && i != path.size()) {
                break;
            }
        }
        bool flag = true;
        for (const auto &display: toDisplay) {
            if (display == newPath) {
                flag = false;
                break;
            }
        }
        if (flag) {
            toDisplay.push_back(newPath);
        }
    }
    return toDisplay;
};