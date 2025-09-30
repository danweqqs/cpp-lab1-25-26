// Compiler: Apple Clang 16.0.0 (clang-1600.0.26.3)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

class CreatePoint {
    std::pair<double, double> currentPoint;
    std::vector<std::pair<double, double>> pointsOfPlane;
    std::mt19937 rng;
    std::uniform_int_distribution<> randomIndex;
public:
    CreatePoint() : rng(std::random_device{}()), randomIndex(0, 0) {}

    CreatePoint(std::pair<double, double> start, std::vector<std::pair<double, double>> points)
        : currentPoint(start), pointsOfPlane(points),
          rng(std::random_device{}()), randomIndex(0, pointsOfPlane.empty() ? 0 : pointsOfPlane.size() - 1) {
    }

    bool hasBasePoints() const {
        return !pointsOfPlane.empty();
    }

    std::pair<double,double> operator()() {
        auto cur = currentPoint;
        if (!pointsOfPlane.empty()) {
            int i = randomIndex(rng);
            auto bk = pointsOfPlane[i];
            currentPoint.first  = (bk.first  + currentPoint.first)  / 2.0;
            currentPoint.second = (bk.second + currentPoint.second) / 2.0;
        }
        return cur;
    }
};

bool readPointsFromFile(const std::string& filename, int& n, CreatePoint& cp) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return false;
    }

    if (!(file >> n) || n <= 0) {
        std::cerr << "input error of n" << std::endl;
        return false;
    }

    double x0, y0;
    if (!(file >> x0 >> y0)) {
        std::cerr << "input error of (x0, y0)"<< std::endl;
        return false;
    }

    std::vector<std::pair<double, double>> bases;
    double x_b, y_b;
    while (file >> x_b >> y_b) {
        bases.push_back({x_b, y_b});
    }

    if (bases.empty()) {
        std::cerr << "no base points found" << std::endl;
        return false;
    }

    cp = CreatePoint({x0, y0}, bases);
    return true;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int n;
    CreatePoint create_point;

    if (!readPointsFromFile(inputFile, n, create_point)) {
        return 1;
    }

    if (!create_point.hasBasePoints()) {
        std::cerr << "no base points" << std::endl;
        return 1;
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "cannot open output file " << outputFile << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        auto pt = create_point();
        outFile << pt.first << " " << pt.second << "\n";
    }

    return 0;
}
