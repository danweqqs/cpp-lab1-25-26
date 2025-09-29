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

    std::pair<double, double> operator()() {
        int i = randomIndex(rng);
        std::pair<double, double> bk = pointsOfPlane[i];
        currentPoint.first = (bk.first + currentPoint.first) / 2.0;
        currentPoint.second = (bk.second + currentPoint.second) / 2.0;
        return currentPoint;
    }
};

CreatePoint readPointsFromFile(const std::string& filename, int& n) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "The opening file error: " << filename << std::endl;
    }

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

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Wrong number of arguments!\n";
        return 0;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int n;
    CreatePoint create_point = readPointsFromFile(inputFile, n);

    if (n <= 0 || !create_point.hasBasePoints()) {
        return 1;
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: cannot open output file\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        auto pt = create_point();
        outFile << pt.first << " " << pt.second << "\n";
    }

    return 0;
}