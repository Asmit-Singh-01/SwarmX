#include <algorithm>
#include <iostream>
#include <vector>

namespace SwarmEngine {

struct Coord3D {
    int x;
    int y;
    int time_step;

    bool operator==(const Coord3D& other) const {
        return x == other.x && y == other.y && time_step == other.time_step;
    }
};

class SpaceTimePathfinder {
public:
    SpaceTimePathfinder(int grid_width, int grid_height, int max_steps = 200)
        : width(std::max(1, grid_width)),
          height(std::max(1, grid_height)),
          max_steps(std::max(1, max_steps)) {}

    Coord3D clamp_to_grid(Coord3D pt) const {
        pt.x = std::clamp(pt.x, 0, width - 1);
        pt.y = std::clamp(pt.y, 0, height - 1);
        return pt;
    }

    std::vector<Coord3D> compute_trajectory(Coord3D start, Coord3D goal) const {
        std::vector<Coord3D> trajectory;
        Coord3D current = clamp_to_grid(start);
        Coord3D target = clamp_to_grid(goal);

        trajectory.push_back(current);

        int steps_taken = 0;
        while (!(current.x == target.x && current.y == target.y) && steps_taken < max_steps) {
            if (current.x < target.x) {
                ++current.x;
            } else if (current.x > target.x) {
                --current.x;
            }

            if (current.y < target.y) {
                ++current.y;
            } else if (current.y > target.y) {
                --current.y;
            }

            ++current.time_step;
            ++steps_taken;
            trajectory.push_back(current);
        }

        return trajectory;
    }

private:
    int width;
    int height;
    int max_steps;
};

}  // namespace SwarmEngine

int main() {
    SwarmEngine::SpaceTimePathfinder pathfinder(50, 50);
    SwarmEngine::Coord3D start{0, 0, 0};
    SwarmEngine::Coord3D goal{10, 10, 0};

    auto path = pathfinder.compute_trajectory(start, goal);
    std::cout << "[C++ Engine] Waypoints generated: " << path.size() << std::endl;
    return 0;
}
