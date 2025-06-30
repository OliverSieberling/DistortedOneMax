#include <bits/stdc++.h>
using namespace std;

// returns a copy of the parent, where each bit is flipped with probability 1/n
vector<char> generateChild(vector<char>& par, mt19937& gen) {
    vector<char> child(par);
    uniform_int_distribution<> dis(0, par.size());
    for (int64_t i = 0; i < child.size(); i++) {
        if (dis(gen) == 0) {
            child[i] = 1-child[i];
        }
    }
    return child;
}

// returns if v1 and v2 are equal
bool isEqual(vector<char> &v1, vector<char> &v2) {
    for (int i = 0; i < v1.size(); i++) {
        if(v1[i] != v2[i]) return false;
    }
    return true;
}

// returns fitness of a point; when called for the first time on this point samples distortion and computes fitness
long double getFitness(vector<char> &point, map<vector<char>, long double> &fitness, long double p, mt19937& gen) {
    if (fitness.count(point)==0) {
        bernoulli_distribution dis(p); // distort with probability p
        if (dis(gen)) {
            //exponential_distribution<double> dis2(0.4);
            uniform_real_distribution<> dis2(0, 4.0);
            double distortion = dis2(gen);
            fitness[point] = count(point.begin(), point.end(), 1) + distortion;
        } else {
            fitness[point] = count(point.begin(), point.end(), 1);
        }
    }
    return fitness[point];
}

// simulates optimizing distorted OneMax on (1+lambda)-EA (isElitary = true) or (1-lambda)-EA (isElitary = false).
int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t cutoff, int64_t lambda, long double k, bool isElitary, mt19937& gen) {
    int64_t genCounter = 1;
    long double currentPointFitness = getFitness(currPoint, fitness, p, gen);
    while (currentPointFitness < (int64_t)currPoint.size()-k && genCounter < cutoff) {
        genCounter++;
        long double fittestChildFitness = -1;
        vector<char> fittestChild;

        for (int64_t i = 0; i < lambda; i++) {
            vector<char> child = generateChild(currPoint, gen);
            long double childFitness = getFitness(child, fitness, p, gen);

            if (childFitness > fittestChildFitness) { // uniform tie break done by order
                fittestChildFitness = childFitness;
                fittestChild = child;
            }
        }

        long double fittestChildDistortion = fittestChildFitness - count(fittestChild.begin(), fittestChild.end(), 1);

        if (isElitary) {
            if (fittestChildFitness >= currentPointFitness) {
                currPoint = fittestChild;
                currentPointFitness = fittestChildFitness;
            }
        } else {
            currPoint = fittestChild;
            currentPointFitness = fittestChildFitness;
        }
    }
    return genCounter;
}

// g++ -O3 -march=native -mtune=native -funroll-loops -flto -ffast-math -DNDEBUG -o prog0 MedianSimulations_expUnifDistortions.cpp

int main()
{
    // Compare (1+lambda)-EA and (1,lambda)-EA on DistortedOneMax for
    int64_t numIter = 49;
    int64_t from = 60; // smallest n to benchmark
    int64_t to = 200; // biggest n to benchmark
    int64_t stepSize = 10; // step size of n
    int64_t cutoff = 1'000'000;

    random_device rd;
    mt19937 gen(rd());

    string filename = "lnn15fixed_simulate_unif4_iter=" + to_string(numIter) + "_from=" + to_string(from) + "_to=" + to_string(to) + "_steps=" +to_string(stepSize)+ "_cutoff=" +to_string(cutoff) + ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    for (int64_t n = from; n <= to; n += stepSize) {
        long double p = 0.3/(sqrt(n));
        long double k = pow(n, 0.15);
        int64_t lambda = round(1.5 * log(n));

        vector<int64_t> plusGenerations, commaGenerations;

        for (int64_t iter = 0; iter < numIter; iter++) {
            // choose initial search point uniformly at random
            uniform_int_distribution<> dis(0, 1);
            vector<char> initSearchPoint(n);
            for (int i = 0; i < n; i++) {
                initSearchPoint[i] = dis(gen);
            }

            map<vector<char>,long double> fitness;
            plusGenerations.push_back(simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true, gen));
            commaGenerations.push_back(simulate(initSearchPoint, fitness, p, cutoff, lambda, k, false, gen));
        }
        cout << "n = " << n << endl;
        cout << setw(18) << left << "(1+lambda)-EA:";
        for (int j = 0; j < numIter; j++) {
            cout << setw(10) << left << plusGenerations[j] << " ";
        }
        cout << endl;
        cout << setw(18) << left << "(1,lambda)-EA:";
        for (int j = 0; j < numIter; j++) {
            cout << setw(10) << left << commaGenerations[j] << " ";
        }
        cout << endl << endl;
    }
    return 0;
}


