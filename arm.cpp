#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>

struct Point2D {
    double x;
    double y;
};

Point2D translate(Point2D point, double dx, double dy) {
    return {point.x + dx, point.y + dy};
}

Point2D rotate(Point2D point, double degrees) {
    const double pi = 3.141592653589793;
    double radians = degrees * pi / 180.0;
    double c = std::cos(radians);
    double s = std::sin(radians);

    return {
        point.x * c - point.y * s,
        point.x * s + point.y * c
    };
}

Point2D rotate_about(Point2D point, Point2D pivot, double degrees) {
    Point2D shifted = translate(point, -pivot.x, -pivot.y);

    Point2D rotated = rotate(shifted, degrees);

    return translate(rotated, pivot.x, pivot.y);
}

std::vector<Point2D> arm_positions(
    Point2D base,
    double length1,
    double length2,
    double shoulder_angle,
    double elbow_angle
) {
    Point2D initial_elbow{base.x + length1, base.y};
    Point2D initial_tip{base.x + length1 + length2, base.y};

    Point2D elbow = rotate_about(initial_elbow, base, shoulder_angle);
    Point2D tip = rotate_about(initial_tip, base, shoulder_angle);

    tip = rotate_about(tip, elbow, elbow_angle);

    return {base, elbow, tip};
}

int main() {
    Point2D base{0.0, 0.0};
    double length1 = 0.0;
    double length2 = 0.0;
    double shoulder_angle = 0.0;
    double elbow_angle = 0.0;

    std::cout << "Enter the base coordinates x and y: ";
    std::cin >> base.x >> base.y;

    if (!std::cin) {
        std::cerr << "Could not read the base coordinates.\n";
        return 1;
    }

    std::cout << "Enter the lengths of link 1 and link 2: ";
    std::cin >> length1 >> length2;

    if (!std::cin) {
        std::cerr << "Could not read the link lengths.\n";
        return 1;
    }

    if (!std::isfinite(length1) || !std::isfinite(length2)
        || length1 <= 0.0 || length2 <= 0.0) {
        std::cerr << "Both link lengths must be finite numbers greater than zero.\n";
        return 1;
    }

    std::cout << "Enter shoulder and elbow angles in degrees: ";
    std::cin >> shoulder_angle >> elbow_angle;

    if (!std::cin) {
        std::cerr << "Could not read the joint angles.\n";
        return 1;
    }

    std::ofstream path_file("arm_path.csv");

    if (!path_file) {
        std::cerr << "Could not open arm_path.csv.\n";
        return 1;
    }

    path_file << "frame,point,x,y\n";

    const int steps = 100;

    for (int frame = 0; frame <= steps; frame++) {
        double progress = static_cast<double>(frame) / steps;

        std::vector<Point2D> points = arm_positions(
            base,
            length1,
            length2,
            shoulder_angle * progress,
            elbow_angle * progress
        );

        for (int p = 0; p < 3; p++) {
            path_file << frame << "," << p << ","
                      << points[p].x << "," << points[p].y << "\n";
        }

        if (frame == steps) {
            std::cout << "Final elbow: ("
                      << points[1].x << ", " << points[1].y << ")\n";

            std::cout << "Final tip: ("
                      << points[2].x << ", " << points[2].y << ")\n";
        }
    }

    path_file.close();
    return 0;
}