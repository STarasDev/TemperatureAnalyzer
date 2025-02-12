#include <utility>
#include "../include/experiment.h"

namespace data {
    Experiment::Experiment(std::string name, std::string id,
                           const std::vector<double> &temperatures)
        : name_(std::move(name)), id_(std::move(id)), temperatures_(temperatures) {
    }

    const std::string &Experiment::getName() const {
        return name_;
    }

    const std::string &Experiment::getID() const {
        return id_;
    }

    const std::vector<double> &Experiment::getTemperatures() const {
        return temperatures_;
    }
}
