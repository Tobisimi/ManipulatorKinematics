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

int main() {
    int count = 0;

    std::cout << "How many points? ";
    std::cin >> count;

    if (!std::cin || count < 1 || count > 100) {
        std::cerr << "Enter a whole number from 1 to 100.\n";
        return 1;
    }

    std::vector<Point2D> points(count);

    for (int p = 0; p < count; p++) {
        std::cout << "Enter x and y for point " << p + 1 << ": ";
        std::cin >> points[p].x >> points[p].y;

        if (!std::cin) {
            std::cerr << "Invalid coordinates. Enter numbers only.\n";
            return 1;
        }
    }

    double dx = 0.0;
    double dy = 0.0;
    double angle = 0.0;
    int choice = 0;

    std::cout << "1. Translate\n";
    std::cout << "2. Rotate\n";
    std::cout << "3. Rotate, then translate\n";
    std::cout << "4. Translate, then rotate\n";
    std::cout << "Choose 1, 2, 3, or 4: ";
    std::cin >> choice;

    if (!std::cin || choice < 1 || choice > 4) {
        std::cerr << "Enter a numeric choice from 1 to 4.\n";
        return 1;
    }

    // I FOUND A WAY TO WRITE THIS PART OF THE CODE DIFFERENTLY,
    // SO I DID THAT.
    // The previous single-point version is kept below for reference.

    // if (!std::cin || choice < 1 || choice > 4) {
    //     std::cerr << "Enter numeric coordinates and a choice from 1 to 4.\n";
    //     return 1;
    // }

    // if (choice == 2 || choice == 3 || choice == 4) {
    //     std::cout << "Enter the rotation angle in degrees: ";
    //     std::cin >> angle;
    // }

    // if (choice == 1 || choice == 3 || choice == 4) {
    //     std::cout << "Enter the movement dx and dy: ";
    //     std::cin >> dx >> dy;
    // }

    // if (!std::cin) {
    //     std::cerr << "Invalid input. Enter numbers only.\n";
    //     return 1;
    // }

    // if (choice == 1) {
    //     result = translate(original, dx, dy);
    // } else if (choice == 2) {
    //     result = rotate(original, angle);
    // } else if (choice == 3) {
    //     result = rotate(original, angle);
    //     result = translate(result, dx, dy);
    // } else {
    //     result = translate(original, dx, dy);
    //     result = rotate(result, angle);
    // }

    
    // THIS INPUT STRUCTURE BELOW IS THE ONE I WROTE TO REPLACE IT.
    // In this multiple-point version, the calculations have been
    // moved into the loops below. The input branches remain here.

    if (choice == 1) {
        std::cout << "Enter the movement dx and dy: ";
        std::cin >> dx >> dy;
    } else if (choice == 2) {
        std::cout << "Enter the rotation angle in degrees: ";
        std::cin >> angle;
    } else if (choice == 3) {
        std::cout << "Enter the rotation angle in degrees: ";
        std::cin >> angle;

        std::cout << "Enter the movement dx and dy: ";
        std::cin >> dx >> dy;
    } else if (choice == 4) {
        std::cout << "Enter the movement dx and dy: ";
        std::cin >> dx >> dy;

        std::cout << "Enter the rotation angle in degrees: ";
        std::cin >> angle;
    } else {
        std::cerr << "Enter numeric coordinates and a choice from 1 to 4.\n";
        return 1;
    }
    // End of the alternative input structure.

    if (!std::cin) {
        std::cerr << "Invalid transformation values. Enter numbers only.\n";
        return 1;
    }

    // This part asks the user to enter the pivot. It only asks if the choice is 2, 3, or 4. Choice 1 is just translation which does not require a pivot.
    Point2D pivot{0.0, 0.0};

    if (choice == 2 || choice == 3 || choice == 4) {
        std::cout << "Enter the rotation pivot x and y: ";
        std::cin >> pivot.x >> pivot.y;

        if (!std::cin) {
            std::cerr << "Could not read the pivot coordinates.\n";
            return 1;
        }
    }

    // Save the starting positions, final positions, and animation path.
    std::ofstream points_file("points.csv");
    std::ofstream path_file("path.csv");

    if (!points_file || !path_file) {
        std::cerr << "Could not open the output files.\n";
        return 1;
    }

    points_file << "name,point,x,y\n";
    path_file << "frame,point,x,y\n";

    const int steps = 100;

    for (int frame = 0; frame <= steps; frame++) {
        double progress = static_cast<double>(frame) / steps;

        double first = std::min(2.0 * progress, 1.0);
        double second = std::max(2.0 * progress - 1.0, 0.0);

        for (int p = 0; p < count; p++) {
            Point2D original = points[p];
            Point2D current = original;

            if (choice == 1) {
                current = translate(original, dx * progress, dy * progress);
            } else if (choice == 2) {
                current = rotate_about(original, pivot, angle * progress);            
            } else if (choice == 3) {
                current = rotate_about(original, pivot, angle * first);
                current = translate(current, dx * second, dy * second);
            } else if (choice == 4) {
                current = translate(original, dx * first, dy * first);
                current = rotate_about(current, pivot, angle * second);
            }

            path_file << frame << "," << p << ","
                      << current.x << "," << current.y << "\n";

            if (frame == 0) {
                points_file << "Original," << p << ","
                            << current.x << "," << current.y << "\n";
            }

            if (frame == steps) {
                points_file << "Transformed," << p << ","
                            << current.x << "," << current.y << "\n";

                std::cout << "Point " << p + 1 << "\n";
                std::cout << "Original: ("
                          << original.x << ", " << original.y << ")\n";
                std::cout << "Result: ("
                          << current.x << ", " << current.y << ")\n";
            }
        }
    }

    points_file.close();
    path_file.close();

    std::cout << "Results saved to points.csv\n";
    std::cout << "Movement positions saved to path.csv\n";

    return 0;
}