#include "dijkstrasolver.h"

#include <climits>
#include <algorithm>
#include <chrono>

static int dx[4] =
    {
        -1, 1, 0, 0
};

static int dy[4] =
    {
        0, 0, -1, 1
};

DijkstraSolver::DijkstraSolver(
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

    dist.assign(
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

    dist[start.first][start.second] = 0;
}

bool DijkstraSolver::step()
{
    if (finished)
    {
        return false;
    }

    auto begin =
        std::chrono::high_resolution_clock::now();

    int ux = -1;
    int uy = -1;

    // 방문하지 않은 노드 중
    // dist가 가장 작은 노드 선택
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            if (
                !visited[x][y] &&
                (
                    ux == -1 ||
                    dist[x][y] < dist[ux][uy]
                    )
                )
            {
                ux = x;
                uy = y;
            }
        }
    }

    // 더 이상 갈 수 있는 노드가 없음
    if (
        ux == -1 ||
        dist[ux][uy] == INT_MAX
        )
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

    // 현재 노드 방문 확정
    visited[ux][uy] = true;

    exploredCount++;

    currentCost =
        dist[ux][uy];

    // 도착점에 도착
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
            ux + dx[d];

        int ny =
            uy + dy[d];

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

        if (
            dist[ux][uy] + 1
            <
            dist[nx][ny]
            )
        {
            dist[nx][ny] =
                dist[ux][uy] + 1;

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
DijkstraSolver::getPath() const
{
    std::vector<std::pair<int, int>> path;

    if (!pathFound)
    {
        return path;
    }

    int cx = end_.first;
    int cy = end_.second;

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
