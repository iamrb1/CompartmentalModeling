#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>

namespace cse {

  class Distribution {
  private:
    std::vector<double> probs;
    std::vector<double> cumulative_probs;
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


void Distribution::BuildBinomial(int trials, double prob){
    if(trials < 1){
        throw std::runtime_error("Must have at least 1 datapoint");
    }
    if(prob > 1.0 || prob < 0.0){
        throw std::out_of_range("Probability not expessed as a decimal.");
    }
    cumulative_probs.resize(trials);
    probs.resize(trials);
    for(int i = 0; i < trials; i++){
        probs[i] = std::tgamma(trials+1.0)/(std::tgamma(i+1.0)*std::tgamma(trials-i+1.0))*std::pow(prob,i)*std::pow((1.0-prob),trials-i);
        
        if (i>0){
            cumulative_probs[i] = cumulative_probs[i-1] + probs[i];
        }
        else if (i==0){
            cumulative_probs[i] = probs[i];
        }
    }
}

void Distribution::BuildUniform(int trials){
    if(trials < 1){
        throw std::runtime_error("Must have at least 1 datapoint");
    }
    double odds = 1.0/trials;
    probs.assign(trials, odds);
    cumulative_probs.resize(trials);
    cumulative_probs[0] = noChance;
    for(int i = 1; i < trials; i++){
        cumulative_probs[i] = cumulative_probs[i-1] + odds;
    }
}

void Distribution::BuildNormal([[maybe_unused]] int trials){
    
}

void Distribution::BuildPoisson([[maybe_unused]] int trials){
    
}

void Distribution::BuildPowerLaw([[maybe_unused]] int trials){
    
}

double Distribution::getProb(int trials){
    if(trials < 0){
        throw std::out_of_range("Must have at least 1 trial");
    }
    if(probs.empty()){
        throw std::runtime_error("You have not built a distrubtion");
    }
    if(trials >= static_cast<int>(probs.size())){
        return noChance;
    }
    return probs[trials];
}

double Distribution::getCumulativeProb(int trials){
    if(trials < 0){
        std::out_of_range("Must have at least 1 trial");
    }
    if(cumulative_probs.empty()){
        throw std::runtime_error("You have not built a distrubtion");
    }
    if(trials >= static_cast<int>(cumulative_probs.size())){
        return noChance;
    }
    return cumulative_probs[trials];
}

} // namespace cse