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

        [[nodiscard]] const std::string &getName() const;

        [[nodiscard]] const std::string &getID() const;

        [[nodiscard]] const std::vector<double> &getTemperatures() const;

    private:
        std::string name_;
        std::string id_;
        std::vector<double> temperatures_;
    };
}