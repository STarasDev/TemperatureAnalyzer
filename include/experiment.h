#pragma once

#include <string>
#include <vector>

/**
 *   A storage class, which stores name, id and temperatures for a particular experiment
 */
class Experiment {
public:
    Experiment(const std::string &name, const std::string &id,
               const std::vector<double> &temperatures);

    const std::string &getName() const;

    const std::string &getID() const;

    const std::vector<double> &getTemperatures() const;

private:
    std::string name;
    std::string id;
    std::vector<double> temperatures;
};
