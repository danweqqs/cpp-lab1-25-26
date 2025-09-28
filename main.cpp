#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

class CreatePoint {
private:
    pair<double, double> currentPoint;
    vector<pair<double, double>> pointsOfPlane;
public:
    CreatePoint(pair<double, double> start, vector<pair<double, double>> points)
        : currentPoint(start), pointsOfPlane(points) {
        try {
            if (pointsOfPlane.empty()) {
                throw invalid_argument("the list is empty");
            }
        } catch (const invalid_argument &e) {
            cerr << "error in CreatePoint constructor:  " << e.what() << endl;
        }
    }
    pair<double, double> getCurrent() const {
        return currentPoint;
    }
    const vector<pair<double, double>> getBasePoints() const {
        return pointsOfPlane;
    }
};
CreatePoint readPointsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "The opening file error: " << filename << endl;
    }

    int n;
    file >> n;
    if(!(file >> n)){
        cerr << "The input error" << endl;
    }

    double x0, y0;
    file >> x0 >> y0;
    if(!(file >> x0 >> y0)){
        cerr << "The input error" << endl;
    }

    vector<pair<double, double>> bases;
    double x_b, y_b;
    while (file >> x_b >> y_b) {
        bases.push_back({x_b, y_b});
    }

    if (bases.empty()) {
        cerr << "Error: base points are not found." << endl;
    }

    file.close();
    return CreatePoint({x0, y0}, bases);
}
int main() {
    return 0;
}