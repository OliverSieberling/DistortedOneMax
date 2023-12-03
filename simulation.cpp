#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par) {
    vector<char> child(par.size());
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(1.0/(int)par.size());

    for (int64_t i = 0; i < par.size(); i++) {
        if (dist(gen)) { // flip with probability p
            child[i] = 1-par[i];
        } else {
            child[i] = par[i];
        }
    }
    return child;
}


long double getFitness(vector<char> &point, map<vector<char>, long double> &fitness, long double p) {
    if (fitness.count(point)==0) {
        random_device rd;
        mt19937 gen(rd());
        bernoulli_distribution dis(p); // distort with probability
        if (dis(gen)) { // distorted point
            random_device rd2;
            mt19937 gen2(rd2());
            exponential_distribution<double> dist2(0.7);
            double distortion = dist2(gen2);
            //cout << distortion << endl;
            fitness[point] = count(point.begin(), point.end(), 1) + distortion;
        } else { // not distorted point
            fitness[point] = count(point.begin(), point.end(), 1);
        }
    }
    return fitness[point];
}

int64_t simulate(vector<char> currPoint, map<vector<char>,long double> &fitness, long double p, int64_t cutoff, int64_t lambda, long double k, bool isElitary) {
    int64_t genCounter = 1;
    while (getFitness(currPoint, fitness, p) < (int64_t)currPoint.size()-k && genCounter < cutoff) {

        genCounter++;
        vector<char> fittestChild = generateChild(currPoint);

        for (int64_t i = 0; i < lambda-1; i++) {
            vector<char> child = generateChild(currPoint);

            if (getFitness(child, fitness, p) > getFitness(fittestChild, fitness, p)) {
                fittestChild = child;
            }
        }
        if (isElitary) {
            if (getFitness(fittestChild, fitness, p) > getFitness(currPoint, fitness, p)) {
                currPoint = fittestChild;
            } else if (getFitness(fittestChild, fitness, p) == getFitness(currPoint, fitness, p)) { // break ties with coinflip
                if (rand()%2) {
                    currPoint = fittestChild;
                }
            }
        } else {
            currPoint = fittestChild;
        }
    }
    return genCounter;

}

int main()
{
    // Compare 1+lambda and 1,lambda for
    int64_t numIter = 20;
    int64_t from =  50; // smallest n to benchmark
    int64_t to = 250; // biggest n to benchmark
    int64_t stepSize = 25; // step size of n
    int64_t cutoff = 20'000;

    vector<vector<int64_t>> plusGen, commaGen;
    vector<int64_t> nValues;

    string filename = "simulate_iter=" + to_string(numIter) + "_from=" + to_string(from) + "_to=" + to_string(to) + "_steps=" +to_string(stepSize)+ "_cutoff=" +to_string(cutoff) + ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());


    for (int64_t n = from; n <= to; n += stepSize) {
        cout << n << ": " << endl;
        nValues.push_back(n);
        vector<int64_t> nPlusGen, nCommaGen;
        for (int64_t iter = 0; iter < numIter; iter++) {
            long double p = 1.0/sqrt(n); // distortion probability
            long double k = pow(n,1/3.0);
            int64_t lambda = round( (exp(1)/(exp(1)-1)) * log(n));
            map<vector<char>,long double> fitness;
            vector<char> initSearchPoint(n, 0);
            nPlusGen.push_back(simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true));
            nCommaGen.push_back(simulate(initSearchPoint, fitness, p, cutoff, lambda, k, false));
           // cout << nPlusGen.back() << " " << nCommaGen.back() << endl;
        }
        for (int j = 0; j < nPlusGen.size(); j++) {
            cout << nPlusGen[j] << " ";
        }
        cout << endl;
        for (int j = 0; j < nCommaGen.size(); j++) {
            cout << nCommaGen[j] << " ";
        }
        cout << endl;
        plusGen.push_back(nPlusGen);
        commaGen.push_back(nCommaGen);
    }
    for (int i = 0; i < plusGen.size(); i++) {
        for (int j = 0; j < plusGen[i].size(); j++) {
            cout << plusGen[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < commaGen.size(); i++) {
        for (int j = 0; j < commaGen[i].size(); j++) {
            cout << commaGen[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}



