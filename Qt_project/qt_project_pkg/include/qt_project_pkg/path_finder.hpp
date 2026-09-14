#ifndef PATH_FINDER_HPP_
#define PATH_FINDER_HPP_

#include <vector>
#include <utility>

#include "map_manager.hpp"
#include "cat.hpp"


struct PathNode
{
    int row;
    int col;

    int g;
    int h;
    int f;
};


struct ComparePathNode
{
    bool operator()(
        PathNode a,
        PathNode b)
    {
        // f 값이 작은 노드 우선
        if (a.f == b.f)
        {
            return a.h > b.h;
        }

        return a.f > b.f;
    }
};


class PathFinder
{
public:
    PathFinder();

    std::vector<std::pair<int, int>> findPath(
        int startRow,
        int startCol,
        int goalRow,
        int goalCol,
        MapManager* mapManager,
        std::vector<Cat>& cats,
        int catFearRadius
    );

private:
    int calculateHeuristic(
        int row,
        int col,
        int goalRow,
        int goalCol
    );

    bool isCatDangerPosition(
        int row,
        int col,
        std::vector<Cat>& cats,
        int catFearRadius
    );
};

#endif