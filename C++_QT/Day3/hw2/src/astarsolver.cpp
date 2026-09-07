#include "astarsolver.h"

#include <climits>
#include <algorithm>
#include <cmath>
#include <chrono>

static int astarDx[4] =
    {
        -1, 1, 0, 0
};

static int astarDy[4] =
    {
        0, 0, -1, 1
};

AStarSolver::AStarSolver(
    const std::vector<std::vector<int>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end
    )
    : maze(maze),
    start(start),
    end_(end)
{
    rows = maze.size();

    cols = maze[0].size();

    gCost.assign(
        rows,
        std::vector<int>(cols, INT_MAX)
        );

    visited.assign(
        rows,
        std::vector<bool>(cols, false)
        );

    parent.assign(
        rows,
        std::vector<std::pair<int, int>>(
            cols,
            {-1, -1}
            )
        );

    gCost[start.first][start.second] = 0;
}

int AStarSolver::heuristic(
    int x,
    int y
    ) const
{
    return
        std::abs(x - end_.first)
        +
        std::abs(y - end_.second);
}

bool AStarSolver::step()
{
    if (finished)
    {
        return false;
    }

    auto begin =
        std::chrono::high_resolution_clock::now();

    int ux = -1;
    int uy = -1;

    int bestF = INT_MAX;

    // 방문하지 않은 노드 중
    // f = g + h가 가장 작은 노드 선택
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            if (visited[x][y])
            {
                continue;
            }

            if (gCost[x][y] == INT_MAX)
            {
                continue;
            }

            int fCost =
                gCost[x][y]
                +
                heuristic(x, y);

            if (
                ux == -1 ||
                fCost < bestF
                )
            {
                ux = x;
                uy = y;
                bestF = fCost;
            }
        }
    }

    // 갈 수 있는 노드가 없음
    if (ux == -1)
    {
        finished = true;

        auto end =
            std::chrono::high_resolution_clock::now();

        elapsedNanoseconds +=
            std::chrono::duration_cast<
                std::chrono::nanoseconds
                >(end - begin).count();

        return false;
    }

    visited[ux][uy] = true;

    exploredCount++;

    // 현재 노드까지 실제 이동 비용
    currentCost =
        gCost[ux][uy];

    // 도착점
    if (
        ux == end_.first &&
        uy == end_.second
        )
    {
        finished = true;
        pathFound = true;

        auto end =
            std::chrono::high_resolution_clock::now();

        elapsedNanoseconds +=
            std::chrono::duration_cast<
                std::chrono::nanoseconds
                >(end - begin).count();

        return false;
    }

    // 상하좌우 확인
    for (int d = 0; d < 4; d++)
    {
        int nx =
            ux + astarDx[d];

        int ny =
            uy + astarDy[d];

        if (
            nx < 0 ||
            nx >= rows ||
            ny < 0 ||
            ny >= cols
            )
        {
            continue;
        }

        if (maze[nx][ny] == 1)
        {
            continue;
        }

        if (visited[nx][ny])
        {
            continue;
        }

        int newG =
            gCost[ux][uy] + 1;

        if (
            newG <
            gCost[nx][ny]
            )
        {
            gCost[nx][ny] =
                newG;

            parent[nx][ny] =
                {ux, uy};
        }
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    elapsedNanoseconds +=
        std::chrono::duration_cast<
            std::chrono::nanoseconds
            >(end - begin).count();

    return true;
}

std::vector<std::pair<int, int>>
AStarSolver::getPath() const
{
    std::vector<std::pair<int, int>> path;

    if (!pathFound)
    {
        return path;
    }

    int cx =
        end_.first;

    int cy =
        end_.second;

    while (!(cx == -1 && cy == -1))
    {
        path.push_back(
            {cx, cy}
            );

        auto p =
            parent[cx][cy];

        cx = p.first;
        cy = p.second;
    }

    std::reverse(
        path.begin(),
        path.end()
        );

    return path;
}
