#ifndef UNTITLED_CALCULATION_PATH_H
#define UNTITLED_CALCULATION_PATH_H

#include <map>

class calculation_path {
private:
    std::vector<std::string> path;
    bool isCycle;
    std::map<std::string, bool> visited;

public:
    calculation_path(const std::vector<std::string> &path, const std::map<std::string, bool> &visited);

    const std::vector<std::string> &getPath() const;

    void setPath(const std::vector<std::string> &vector);

    const std::map<std::string, bool> &getVisited() const;

    void setVisited(const std::map<std::string, bool> &visited);

    bool isCyclicDependency() const;

    void setCyclicDependencyFlag(bool b);
};

#endif
