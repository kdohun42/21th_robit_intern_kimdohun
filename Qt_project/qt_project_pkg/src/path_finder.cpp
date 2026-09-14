#include "../include/qt_project_pkg/path_finder.hpp"

#include <queue>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>


PathFinder::PathFinder()
{
}


int PathFinder::calculateHeuristic(
    int row,
    int col,
    int goalRow,
    int goalCol)
{
    // 현재 위치에서 플레이어까지 맨해튼 거리
    int rowDistance =
        std::abs(
            goalRow - row
        );

    int colDistance =
        std::abs(
            goalCol - col
        );

    return
        rowDistance +
        colDistance;
}


bool PathFinder::isCatDangerPosition(
    int row,
    int col,
    std::vector<Cat>& cats,
    int catFearRadius)
{
    // 모든 고양이 검사
    for (
        int i = 0;
        i < static_cast<int>(
            cats.size()
        );
        i++)
    {
        // 배치되지 않은 고양이는 무시
        if (!cats[i].isPlaced())
        {
            continue;
        }


        // 현재 위치와 고양이 사이 거리
        int rowDistance =
            std::abs(
                cats[i].getRow() -
                row
            );

        int colDistance =
            std::abs(
                cats[i].getCol() -
                col
            );

        int distance =
            rowDistance +
            colDistance;


        // 고양이 공포 범위 안
        if (
            distance <=
            catFearRadius)
        {
            return true;
        }
    }


    return false;
}


std::vector<std::pair<int, int>>
PathFinder::findPath(
    int startRow,
    int startCol,
    int goalRow,
    int goalCol,
    MapManager* mapManager,
    std::vector<Cat>& cats,
    int catFearRadius)
{
    // 경로 실패 시 반환
    std::vector<std::pair<int, int>> emptyPath;


    // 맵 확인
    if (mapManager == nullptr)
    {
        return emptyPath;
    }


    // 맵 크기
    int mapRows =
        mapManager->
        getMapRows();

    int mapCols =
        mapManager->
        getMapCols();


    // 시작 위치 범위 확인
    if (
        startRow < 0 ||
        startRow >= mapRows ||
        startCol < 0 ||
        startCol >= mapCols)
    {
        return emptyPath;
    }


    // 목표 위치 범위 확인
    if (
        goalRow < 0 ||
        goalRow >= mapRows ||
        goalCol < 0 ||
        goalCol >= mapCols)
    {
        return emptyPath;
    }


    // 시작 위치 장애물 확인
    if (
        mapManager->
        getCollisionValue(
            startRow,
            startCol
        ) == 1)
    {
        return emptyPath;
    }


    // 플레이어 위치가 고양이 공포 범위인지 확인
    bool goalInsideCatArea =
        isCatDangerPosition(
            goalRow,
            goalCol,
            cats,
            catFearRadius
        );


    // 플레이어가 고양이 범위 밖인데
    // 플레이어 위치가 장애물이라면 경로 없음
    if (
        !goalInsideCatArea &&
        mapManager->
        getCollisionValue(
            goalRow,
            goalCol
        ) == 1)
    {
        return emptyPath;
    }


    // 실제 이동 비용
    std::vector<std::vector<int>> gScore(
        mapRows,
        std::vector<int>(
            mapCols,
            INT_MAX
        )
    );


    // 방문 여부
    std::vector<std::vector<int>> closed(
        mapRows,
        std::vector<int>(
            mapCols,
            0
        )
    );


    // 부모 행
    std::vector<std::vector<int>> parentRow(
        mapRows,
        std::vector<int>(
            mapCols,
            -1
        )
    );


    // 부모 열
    std::vector<std::vector<int>> parentCol(
        mapRows,
        std::vector<int>(
            mapCols,
            -1
        )
    );


    // A* Open List
    std::priority_queue<
        PathNode,
        std::vector<PathNode>,
        ComparePathNode
    > openList;


    // 시작 노드
    PathNode startNode;

    startNode.row =
        startRow;

    startNode.col =
        startCol;

    startNode.g =
        0;

    startNode.h =
        calculateHeuristic(
            startRow,
            startCol,
            goalRow,
            goalCol
        );

    startNode.f =
        startNode.g +
        startNode.h;


    gScore[
        startRow
    ][
        startCol
    ] = 0;


    openList.push(
        startNode
    );


    // 플레이어와 가장 가까운 안전한 위치 저장
    int bestRow =
        startRow;

    int bestCol =
        startCol;

    int bestHeuristic =
        calculateHeuristic(
            startRow,
            startCol,
            goalRow,
            goalCol
        );


    // 상하좌우
    int rowChange[4] =
    {
        -1,
        1,
        0,
        0
    };


    int colChange[4] =
    {
        0,
        0,
        -1,
        1
    };


    // A* 탐색
    while (!openList.empty())
    {
        PathNode current =
            openList.top();

        openList.pop();


        // 이미 방문한 위치
        if (
            closed[
                current.row
            ][
                current.col
            ] == 1)
        {
            continue;
        }


        // 방문 처리
        closed[
            current.row
        ][
            current.col
        ] = 1;


        // 현재 칸이 고양이 안전 구역인지 확인
        bool currentDanger =
            isCatDangerPosition(
                current.row,
                current.col,
                cats,
                catFearRadius
            );


        // 안전한 칸 중 플레이어와 가장 가까운 칸 저장
        if (!currentDanger)
        {
            int currentHeuristic =
                calculateHeuristic(
                    current.row,
                    current.col,
                    goalRow,
                    goalCol
                );


            if (
                currentHeuristic <
                bestHeuristic)
            {
                bestHeuristic =
                    currentHeuristic;

                bestRow =
                    current.row;

                bestCol =
                    current.col;
            }
        }


        // 플레이어가 고양이 범위 밖이면
        // 기존처럼 정확한 플레이어 위치까지 이동
        if (
            !goalInsideCatArea &&
            current.row == goalRow &&
            current.col == goalCol)
        {
            bestRow =
                goalRow;

            bestCol =
                goalCol;

            break;
        }


        // 상하좌우 탐색
        for (
            int i = 0;
            i < 4;
            i++)
        {
            int nextRow =
                current.row +
                rowChange[i];


            int nextCol =
                current.col +
                colChange[i];


            // 맵 범위
            if (
                nextRow < 0 ||
                nextRow >= mapRows ||
                nextCol < 0 ||
                nextCol >= mapCols)
            {
                continue;
            }


            // 건물 충돌
            if (
                mapManager->
                getCollisionValue(
                    nextRow,
                    nextCol
                ) == 1)
            {
                continue;
            }


            // 고양이 공포 범위는 절대 진입하지 않음
            if (
                isCatDangerPosition(
                    nextRow,
                    nextCol,
                    cats,
                    catFearRadius
                ))
            {
                continue;
            }


            // 이미 방문
            if (
                closed[
                    nextRow
                ][
                    nextCol
                ] == 1)
            {
                continue;
            }


            // 실제 이동 비용
            int newG =
                current.g + 1;


            if (
                newG <
                gScore[
                    nextRow
                ][
                    nextCol
                ])
            {
                gScore[
                    nextRow
                ][
                    nextCol
                ] =
                    newG;


                // 부모 위치 저장
                parentRow[
                    nextRow
                ][
                    nextCol
                ] =
                    current.row;


                parentCol[
                    nextRow
                ][
                    nextCol
                ] =
                    current.col;


                // 다음 노드
                PathNode nextNode;

                nextNode.row =
                    nextRow;

                nextNode.col =
                    nextCol;

                nextNode.g =
                    newG;

                nextNode.h =
                    calculateHeuristic(
                        nextRow,
                        nextCol,
                        goalRow,
                        goalCol
                    );

                nextNode.f =
                    nextNode.g +
                    nextNode.h;


                openList.push(
                    nextNode
                );
            }
        }
    }


    // 시작점이 이미 가장 가까운 안전 위치
    if (
        bestRow == startRow &&
        bestCol == startCol)
    {
        std::vector<std::pair<int, int>> path;

        path.push_back(
            std::make_pair(
                startRow,
                startCol
            )
        );

        return path;
    }


    // 가장 가까운 안전 위치부터 역추적
    std::vector<std::pair<int, int>> path;


    int pathRow =
        bestRow;

    int pathCol =
        bestCol;


    while (
        pathRow != startRow ||
        pathCol != startCol)
    {
        path.push_back(
            std::make_pair(
                pathRow,
                pathCol
            )
        );


        int previousRow =
            parentRow[
                pathRow
            ][
                pathCol
            ];


        int previousCol =
            parentCol[
                pathRow
            ][
                pathCol
            ];


        // 경로 복원 실패
        if (
            previousRow == -1 ||
            previousCol == -1)
        {
            return emptyPath;
        }


        pathRow =
            previousRow;

        pathCol =
            previousCol;
    }


    // 시작 위치 추가
    path.push_back(
        std::make_pair(
            startRow,
            startCol
        )
    );


    // 순서 뒤집기
    std::reverse(
        path.begin(),
        path.end()
    );


    return path;
}