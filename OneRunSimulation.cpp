#include <bits/stdc++.h>

using namespace std;

vector<char> generateChild(vector<char>& par) {
    vector<char> child(par.size());
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(1.0/(int)par.size());

    for (int64_t i = 0; i < par.size(); i++) {
        if (dist(gen)) {
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
            exponential_distribution<double> dist2(0.4);
           // cauchy_distribution<double> dist2(0, 0.3);
            //uniform_real_distribution<> dist2(0, 5.0);
            double distortion = abs(dist2(gen2));
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
                cout << genCounter << " " << count(currPoint.begin(), currPoint.end(), 1) << " " << (getFitness(currPoint, fitness, p)-count(currPoint.begin(), currPoint.end(), 1)) << "\n";
            }
        } else {
            currPoint = fittestChild;
            cout << genCounter << " " << count(currPoint.begin(), currPoint.end(), 1) << " " << (getFitness(currPoint, fitness, p)-count(currPoint.begin(), currPoint.end(), 1)) << "\n";

        }
    }
    //cout << genCounter << " " << getFitness(currPoint, fitness, p) << " " << count(currPoint.begin(), currPoint.end(), 1) << endl;
    return genCounter;

}

int main()
{

    /*string filename = "simulate_uniformTo5_iter=" + to_string(numIter) + "_from=" + to_string(from) + "_to=" + to_string(to) + "_steps=" +to_string(stepSize)+ "_cutoff=" +to_string(cutoff) + ".txt";
    ofstream outFile(filename);
    streambuf* coutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());*/

    int64_t n= 100;
    long double p = 0.3/(sqrt(n)); // distortion probability
    long double k = pow(n, 0.15);
    int64_t lambda = round( (exp(1)/(exp(1)-1)) * log(n));
    int64_t cutoff = 1'000'000;
    cout << n-k << endl;
    map<vector<char>,long double> fitness;
    vector<char> initSearchPoint(n, 0);
    simulate(initSearchPoint, fitness, p, cutoff, lambda, k, true);

    return 0;
}



