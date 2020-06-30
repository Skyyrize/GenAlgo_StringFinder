#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

std::string finalResult = "SalutMecCaVa";
unsigned generation = 0;
unsigned startPopulation = 100;
unsigned mutationChance = 1;
bool finded = false;

char randomChar() {
    char c;
    if (rand() % 2 == 0) {
        c = 'a' + rand() % 26;
    } else {
        c = 'A' + rand() % 26;
    }
    return c;
}

class Entity {
private:
    std::string chromosome;
    unsigned fitness;
    void mutation() {
        if (rand() % 100 > mutationChance) return;
        chromosome[rand() % finalResult.size()] = randomChar();
    }
public:
    Entity(std::string c) : chromosome(c) {
        this->mutation();
        this->fitness = 0;
        for (unsigned i = 0; i < finalResult.size(); ++i) {
            if (finalResult[i] == this->chromosome[i]) {
                fitness++;
            }
        }
        if (this->chromosome == finalResult) finded = true;
    }
    ~Entity() {};
    std::string getChromosome() const {
        return this->chromosome;
    }
    unsigned getFitness() const {
        return this->fitness;
    }
};

std::vector<Entity*> population;


std::string crossover(Entity* one, Entity* two) {
    //https://en.wikipedia.org/wiki/Crossover_(genetic_algorithm)
    unsigned singleLine = rand() % finalResult.size();
    std::string newChromosome = one->getChromosome();
    for (unsigned i = singleLine; i < finalResult.size(); i=i+1) {
        newChromosome[i] = two->getChromosome()[i];
    }
    return newChromosome;
}

std::vector<Entity*> selection() {
    std::sort(population.begin(), population.end(), [](Entity* one, Entity* two) {return one->getFitness() > two->getFitness(); });
    std::cout << generation << " " << population[0]->getChromosome() << " " << population[0]->getFitness() << std::endl;

    //save Entity
    std::vector<Entity*> bestOldPopulation;
    for (unsigned i = 0; i < population.size() * 0.2; ++i) {
        bestOldPopulation.push_back(population[i]);
    }
    return bestOldPopulation;
}

void createFirstPopulation() {
    for (unsigned i = 0; i < startPopulation; ++i) {
        //Create population with very random chromosome
        std::string randomChromosome;
        for (unsigned y = 0; y < finalResult.size(); ++y) {
            randomChromosome += randomChar();
        }
        population.push_back(new Entity(randomChromosome));
    }
}

void createPopulation(std::vector<Entity*> bestEntities) {
    ++generation;
    population.clear();
    population = bestEntities;
    while (population.size() < startPopulation) {
        Entity* parent1 = population[rand() % startPopulation*0.2];
        Entity* parent2 = population[rand() % startPopulation*0.2];
        population.push_back(new Entity(crossover(parent1, parent2)));
    }
}

int main() {
    srand(time(NULL));

    createFirstPopulation();
    while (!finded) {
        createPopulation(selection());
    }

    std::cout << "Trouver" << std::endl;
    std::sort(population.begin(), population.end(), [](Entity* one, Entity* two) {return one->getFitness() > two->getFitness(); });
    std::cout << generation << " " << population[0]->getChromosome() << " " << population[0]->getFitness() << std::endl;
}