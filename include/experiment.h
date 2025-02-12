#pragma once

#include <string>
#include <vector>

/**
 *   A storage class, which stores name, id and temperatures for a particular experiment
 */
namespace data {
    class Experiment {
    public:
        Experiment(std::string name, std::string id,
                   const std::vector<double> &temperatures);

        const std::string &getName() const;

        const std::string &getID() const;

        const std::vector<double> &getTemperatures() const;

    private:
        std::string name_;
        std::string id_;
        std::vector<double> temperatures_;
    };
}