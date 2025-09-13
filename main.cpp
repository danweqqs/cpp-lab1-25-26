#include "main.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>
#include <fstream>

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

  bool empty() const {
    return pointsOfPlane.empty();
  }
};

int main() {
  return 0;
}