#include <iostream>
#include <vector>
#include <random>
#include <fstream>

class CreatePoint {
private:
  std::pair<double, double> currentPoint;
  std::vector<std::pair<double, double>> pointsOfPlane;
  std::mt19937 rng;
  std::uniform_int_distribution<> randomIndex;

public:
  CreatePoint(std::pair<double, double> start, std::vector<std::pair<double, double>> points)
      : currentPoint(start), pointsOfPlane(points),
        rng(std::random_device{}()), randomIndex(0, pointsOfPlane.empty() ? 0 : pointsOfPlane.size() - 1) {
  }

  bool hasBasePoints() const {
    return !pointsOfPlane.empty();
  }

  std::pair<double, double> getCurrent() const {
    return currentPoint;
  }

  const std::vector<std::pair<double, double>> getBasePoints() const {
    return pointsOfPlane;
  }
};

CreatePoint readPointsFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "The opening file error: " << filename << std::endl;
  }

  int n;
  if (!(file >> n)) {
    std::cerr << "The input error" << std::endl;
  }

  double x0, y0;
  if (!(file >> x0 >> y0)) {
    std::cerr << "The input error" << std::endl;
  }

  std::vector<std::pair<double, double>> bases;
  double x_b, y_b;
  while (file >> x_b >> y_b) {
    bases.push_back({x_b, y_b});
  }

  if (bases.empty()) {
    std::cerr << "Error: base points are not found." << std::endl;
  }

  file.close();
  return CreatePoint({x0, y0}, bases);
}

int main() {
  return 0;
}
