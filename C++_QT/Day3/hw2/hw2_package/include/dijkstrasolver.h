#ifndef DIJKSTRASOLVER_H
#define DIJKSTRASOLVER_H

#include <vector>
#include <utility>

class DijkstraSolver
{
public:
    DijkstraSolver(
        const std::vector<std::vector<int>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end
        );

    bool step();

    bool isPathFound() const
    {
        return pathFound;
    }

    const std::vector<std::vector<bool>>& getVisited() const
    {
        return visited;
    }

    std::vector<std::pair<int, int>> getPath() const;

    int getCost() const
    {
        if (!pathFound)
        {
            return -1;
        }

        return dist[end_.first][end_.second];
    }

    int getCurrentCost() const
    {
        return currentCost;
    }

    int getExploredCount() const
    {
        return exploredCount;
    }

    double getElapsedTimeMs() const
    {
        return elapsedNanoseconds / 1000000.0;
    }

private:
    std::vector<std::vector<int>> maze;

    int rows;
    int cols;

    std::pair<int, int> start;
    std::pair<int, int> end_;

    std::vector<std::vector<int>> dist;

    std::vector<std::vector<bool>> visited;

    std::vector<
        std::vector<std::pair<int, int>>
        > parent;

    bool finished = false;
    bool pathFound = false;

    int exploredCount = 0;

    int currentCost = 0;

    long long elapsedNanoseconds = 0;
};

#endif
