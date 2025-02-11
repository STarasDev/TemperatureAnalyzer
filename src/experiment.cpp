#include "../include/Experiment.h"

Experiment::Experiment(const std::string &name, const std::string &id,
                       const std::vector<double> &temperatures)
        : name(name), id(id), temperatures(temperatures) {
}

const std::string &Experiment::getName() const {
    return name;
}

const std::string &Experiment::getID() const {
    return id;
}

const std::vector<double> &Experiment::getTemperatures() const {
    return temperatures;
}
