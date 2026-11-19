#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

struct Point3D {
    double x;
    double y;
    double z;
};

double to_radians(double degrees) {
    const double pi = 3.141592653589793;
    return degrees * pi / 180.0;
}

Point3D translate3D(Point3D point, double dx, double dy, double dz) {
    return {
        point.x + dx,
        point.y + dy,
        point.z + dz
    };
}

Point3D rotateX(Point3D point, double degrees) {
    double radians = to_radians(degrees);
    double c = std::cos(radians);
    double s = std::sin(radians);

    return {
        point.x,
        point.y * c - point.z * s,
        point.y * s + point.z * c
    };
}

Point3D rotateY(Point3D point, double degrees) {
    double radians = to_radians(degrees);
    double c = std::cos(radians);
    double s = std::sin(radians);

    return {
        point.x * c + point.z * s,
        point.y,
        -point.x * s + point.z * c
    };
}

Point3D rotateZ(Point3D point, double degrees) {
    double radians = to_radians(degrees);
    double c = std::cos(radians);
    double s = std::sin(radians);

    return {
        point.x * c - point.y * s,
        point.x * s + point.y * c,
        point.z
    };
}

void print_point(Point3D point) {
    std::cout << "("
              << point.x << ", "
              << point.y << ", "
              << point.z << ")\n";
}

Point3D rotate3D(Point3D point, double rx, double ry, double rz) {
    point = rotateX(point, rx);
    point = rotateY(point, ry);
    point = rotateZ(point, rz);

    return point;
}

int main() {
    int count = 0;

    std::cout << "How many points? ";
    std::cin >> count;

    if (!std::cin || count < 1 || count > 100) {
        std::cerr << "Enter a whole number from 1 to 100.\n";
        return 1;
    }

    std::vector<Point3D> points(count);

    for (int p = 0; p < count; p++) {
        std::cout << "Enter x, y, z for point " << p + 1 << ": ";
        std::cin >> points[p].x >> points[p].y >> points[p].z;

        if (!std::cin) {
            std::cerr << "Invalid coordinates.\n";
            return 1;
        }
    }

    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;

    double rx = 0.0;
    double ry = 0.0;
    double rz = 0.0;

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

    // I FOUND A WAY TO WRITE THE INPUT SECTION DIFFERENTLY.
    // This is the earlier input approach, adapted here for 3D.

    // if (choice == 2 || choice == 3 || choice == 4) {
    //     std::cout << "Enter rotation angles rx, ry, rz in degrees: ";
    //     std::cin >> rx >> ry >> rz;
    // }

    // if (choice == 1 || choice == 3 || choice == 4) {
    //     std::cout << "Enter movements dx, dy, dz: ";
    //     std::cin >> dx >> dy >> dz;
    // }

    // THIS USES THE ALTERNATIVE INPUT-BRANCH STRUCTURE I WROTE.
    // The extra coordinates and angles have been added for 3D.

    if (choice == 1) {
        std::cout << "Enter movements dx, dy, dz: ";
        std::cin >> dx >> dy >> dz;
    } else if (choice == 2) {
        std::cout << "Enter rotation angles rx, ry, rz in degrees: ";
        std::cin >> rx >> ry >> rz;
    } else if (choice == 3) {
        std::cout << "Enter rotation angles rx, ry, rz in degrees: ";
        std::cin >> rx >> ry >> rz;

        std::cout << "Enter movements dx, dy, dz: ";
        std::cin >> dx >> dy >> dz;
    } else if (choice == 4) {
        std::cout << "Enter movements dx, dy, dz: ";
        std::cin >> dx >> dy >> dz;

        std::cout << "Enter rotation angles rx, ry, rz in degrees: ";
        std::cin >> rx >> ry >> rz;
    } else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    // End of the adapted input-branch structure.

    if (!std::cin) {
        std::cerr << "Invalid transformation values.\n";
        return 1;
    }

    std::ofstream points_file("points3d.csv");
    std::ofstream path_file("path3d.csv");

    if (!points_file || !path_file) {
        std::cerr << "Could not open the output files.\n";
        return 1;
    }

    points_file << "name,point,x,y,z\n";
    path_file << "frame,point,x,y,z\n";

    const int steps = 100;

    for (int frame = 0; frame <= steps; frame++) {
        double progress = static_cast<double>(frame) / steps;

        double first = std::min(2.0 * progress, 1.0);
        double second = std::max(2.0 * progress - 1.0, 0.0);

        for (int p = 0; p < count; p++) {
            Point3D original = points[p];
            Point3D current = original;

            if (choice == 1) {
                current = translate3D(
                    original,
                    dx * progress, dy * progress, dz * progress
                );
            } else if (choice == 2) {
                current = rotate3D(
                    original,
                    rx * progress, ry * progress, rz * progress
                );
            } else if (choice == 3) {
                current = rotate3D(
                    original,
                    rx * first, ry * first, rz * first
                );

                current = translate3D(
                    current,
                    dx * second, dy * second, dz * second
                );
            } else if (choice == 4) {
                current = translate3D(
                    original,
                    dx * first, dy * first, dz * first
                );

                current = rotate3D(
                    current,
                    rx * second, ry * second, rz * second
                );
            }

            path_file << frame << "," << p << ","
                      << current.x << ","
                      << current.y << ","
                      << current.z << "\n";

            if (frame == 0) {
                points_file << "Original," << p << ","
                            << current.x << ","
                            << current.y << ","
                            << current.z << "\n";
            }

            if (frame == steps) {
                points_file << "Transformed," << p << ","
                            << current.x << ","
                            << current.y << ","
                            << current.z << "\n";

                std::cout << "Point " << p + 1 << "\n";

                std::cout << "Original: ";
                print_point(original);

                std::cout << "Result: ";
                print_point(current);
            }
        }
    }

    points_file.close();
    path_file.close();

    std::cout << "Results saved to points3d.csv\n";
    std::cout << "Movement positions saved to path3d.csv\n";

    return 0;
}