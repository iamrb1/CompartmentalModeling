#pragma once

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace cse {

  class Distribution {
  private:
    std::vector<double> probs;
    std::vector<double> cumulativeProbs;
    const double noChance = 0.0;

  public:
    double getProb(int);
    double getCumulativeProb(int);
    void BuildBinomial(int, double);
    void BuildUniform(int);
    void BuildNormal(int);
    void BuildPoisson(int);
    void BuildPowerLaw(int);
  };

  // Fills the probs and cumulativeProbs vectors with binomial probabilities
  void Distribution::BuildBinomial(int trials, double prob) {
    if (trials < 1) {
      throw std::runtime_error("Must have at least 1 datapoint");
    }
    if (prob > 1.0 || prob < 0.0) {
      throw std::out_of_range("Probability not expessed as a decimal.");
    }
    cumulativeProbs.resize(trials);
    probs.resize(trials);
    for (int i = 0; i < trials; i++) {
      // The formula for binomial probability
      probs[i] = std::tgamma(trials + 1.0) / (std::tgamma(i + 1.0) * std::tgamma(trials - i + 1.0)) *
                 std::pow(prob, i) * std::pow((1.0 - prob), trials - i);
      if (i > 0) {
        cumulativeProbs[i] = cumulativeProbs[i - 1] + probs[i];
      } else if (i == 0) {
        cumulativeProbs[i] = probs[i];
      }
    }
  }
  // Fills the probs and cumulativeProbs vectors with uniform probabilities
  void Distribution::BuildUniform(int trials) {
    if (trials < 1) {
      throw std::runtime_error("Must have at least 1 datapoint");
    }
    double odds = 1.0 / trials;
    probs.assign(trials, odds);
    cumulativeProbs.resize(trials);
    cumulativeProbs[0] = noChance;
    for (int i = 1; i < trials; i++) {
      cumulativeProbs[i] = cumulativeProbs[i - 1] + odds;
    }
  }

  // Future dsitribution likely to be added
  void Distribution::BuildNormal([[maybe_unused]] int trials) {}

  void Distribution::BuildPoisson([[maybe_unused]] int trials) {}

  void Distribution::BuildPowerLaw([[maybe_unused]] int trials) {}

  // Getters
  double Distribution::getProb(int trials) {
    if (trials < 0) {
      throw std::out_of_range("Must have at least 1 trial");
    }
    if (probs.empty()) {
      throw std::runtime_error("You have not built a distrubtion");
    }
    if (trials >= static_cast<int>(probs.size())) {
      return noChance;
    }
    return probs[trials];
  }

  double Distribution::getCumulativeProb(int trials) {
    if (trials < 0) {
      std::out_of_range("Must have at least 1 trial");
    }
    if (cumulativeProbs.empty()) {
      throw std::runtime_error("You have not built a distrubtion");
    }
    if (trials >= static_cast<int>(cumulativeProbs.size())) {
      return noChance;
    }
    return cumulativeProbs[trials];
  }

} // namespace cse