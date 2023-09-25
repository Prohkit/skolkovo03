#include <string>
#include <vector>
#include "calculation_path.h"


calculation_path::calculation_path(const std::vector<std::string> &path, const std::map<std::string, bool> &visited)
        : path(path), isCycle(false), visited(visited) {}

const std::vector<std::string> &calculation_path::getPath() const {
    return path;
}

void calculation_path::setPath(const std::vector<std::string> &vector) {
    calculation_path::path = vector;
}

bool calculation_path::isCyclicDependency() const {
    return isCycle;
}

void calculation_path::setCyclicDependencyFlag(bool b) {
    calculation_path::isCycle = b;
}

const std::map<std::string, bool> &calculation_path::getVisited() const {
    return visited;
}

void calculation_path::setVisited(const std::map<std::string, bool> &visited) {
    calculation_path::visited = visited;
}
