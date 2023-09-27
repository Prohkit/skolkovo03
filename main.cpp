#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "cyclic_dependencies_API.h"
#include "calculation_path.h"

int main(int argc, char *argv[])
{
    csv::table_t table = get_table(argv[1]);

    std::vector<calculation_path> pathsLoopedVector;
    auto tableIterator = table.begin();
    for (int i = 0; tableIterator != table.end(); tableIterator++, i++) {

        // tableIterator->first - адрес текущей ячейки, tableIterator->second - адрес следующей ячейки
        std::string currentCell(tableIterator->first);
        std::string nextCell(tableIterator->second);
        nextCell = remove_equal_char(nextCell);

        // пропускаю ячейки с числами
        if (!((nextCell[0] != '=') & !isdigit(nextCell[0])) || isdigit(nextCell[0]) || currentCell.empty()
        || nextCell.empty()) {
            continue;
        }

        std::vector<std::string> path{currentCell};
        std::map<std::string, bool> visited;
        calculation_path calculationPath{path, visited};
        std::vector<calculation_path> paths;

        // поиск циклов, запись путей с циклической зависимостью в итоговый вектор
        paths = csv::find_loops(calculationPath, table);
        for (const auto &item: paths) {
            if (item.isCyclicDependency()) {
                pathsLoopedVector.emplace_back(item);
            }
        }
    }

    // вывести на экран все возможные пути с циклами
    int count = 0;
    for (int i = 0; i < pathsLoopedVector.size(); ++i) {
        for (int j = 0; j < pathsLoopedVector[i].getPath().size(); ++j) {
            std::cout << pathsLoopedVector[i].getPath()[j] << " ";
        }
        count++;
        std::cout << std::endl;
    }
    std::cout << count;

    // убрать повторяющиеся циклы, обрезать пути до мест зацикливания, вывести на экран
//    std::vector<std::vector<std::string>> toDisplay = remove_duplicates(pathsLoopedVector);
//
//    for (const auto &item: toDisplay) {
//        for (const auto &vector: item) {
//            std::cout << vector << " ";
//        }
//        std::cout << std::endl;
//    }
//    return 0;
}
