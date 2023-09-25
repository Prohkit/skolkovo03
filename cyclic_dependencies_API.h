#ifndef UNTITLED_CYCLIC_DEPENDENCIES_API_H
#define UNTITLED_CYCLIC_DEPENDENCIES_API_H

#include <string>
#include <map>
#include "calculation_path.h"

namespace csv {

    using cell_address_t = std::string;
    using cell_data_t = std::string;
    using table_t = std::map<cell_address_t, cell_data_t>;

    struct ComplexCell;

    std::vector<calculation_path>
    find_loops(const calculation_path &calculationPath, const table_t &csv);
}

csv::ComplexCell check_ops(const csv::table_t &csv, const std::string &nextCell);
std::string remove_equal_char(const std::string &token);
csv::table_t get_table(const std::string &path);
std::vector<std::vector<std::string>> remove_duplicates(const std::vector<calculation_path> &pathsLoopedVector);


#endif //UNTITLED_CYCLIC_DEPENDENCIES_API_H
