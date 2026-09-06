#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

const int ROW = 5;
const int COL = 5;
const int INF = 2147483647;

// A* 노드 정보
struct Node {
    int x, y;
    int g, h, f;
};

// f값이 작은 노드 우선
struct Compare {
    bool operator()(const Node& a, const Node& b) { return a.f > b.f; }
};

// 맨해튼 거리 계산
int heuristic(int x, int y, int endX, int endY) {
    return abs(x - endX) + abs(y - endY);
}

// A* 알고리즘
vector<pair<int, int>> AStar(vector<vector<int>>& maze, int startX, int startY, int endX, int endY) {
    vector<vector<int>> dist(ROW, vector<int>(COL, INF));
    vector<vector<bool>> visited(ROW, vector<bool>(COL, false));
    vector<vector<pair<int, int>>> parent(ROW, vector<pair<int, int>>(COL, {-1, -1}));

    priority_queue<Node, vector<Node>, Compare> pq;

    // 상하좌우 이동
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    // 시작 노드 설정
    dist[startX][startY] = 0;
    int h = heuristic(startX, startY, endX, endY);
    pq.push({startX, startY, 0, h, h});

    while(!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int x = current.x;
        int y = current.y;

        if(visited[x][y]) continue;

        visited[x][y] = true;

        cout << "탐색 위치: (" << x << ", " << y << ") ";
        cout << "g: " << current.g << " ";
        cout << "h: " << current.h << " ";
        cout << "f: " << current.f << endl;

        // 목표 지점 도착
        if(x == endX && y == endY) break;

        // 주변 노드 탐색
        for(int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // 범위 밖
            if(nx < 0 || nx >= ROW || ny < 0 || ny >= COL) continue;

            // 벽
            if(maze[nx][ny] == 1) continue;

            int newG = dist[x][y] + 1;

            // 더 짧은 경로 발견
            if(newG < dist[nx][ny]) {
                dist[nx][ny] = newG;
                parent[nx][ny] = {x, y};

                int newH = heuristic(nx, ny, endX, endY);
                int newF = newG + newH;

                pq.push({nx, ny, newG, newH, newF});
            }
        }
    }

    // 경로 없음
    if(dist[endX][endY] == INF) {
        cout << "경로를 찾을 수 없습니다." << endl;
        return {};
    }

    // 최종 경로 저장
    vector<pair<int, int>> path;

    int x = endX;
    int y = endY;

    while(true) {
        path.push_back({x, y});

        if(x == startX && y == startY) break;

        pair<int, int> p = parent[x][y];
        x = p.first;
        y = p.second;
    }

    reverse(path.begin(), path.end());

    return path;
}

int main() {
    // 0은 길, 1은 벽
    vector<vector<int>> maze = {
        {0, 0, 1, 0, 0},
        {1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    int startX = 0, startY = 0;
    int endX = 4, endY = 4;

    cout << "A* 알고리즘 탐색 시작" << endl << endl;

    vector<pair<int, int>> path = AStar(maze, startX, startY, endX, endY);

    if(path.empty()) return 0;

    cout << endl << "최종 경로" << endl;

    for(auto& p : path) cout << "(" << p.first << ", " << p.second << ") ";

    cout << endl;
    cout << "최종 경로 비용: " << path.size() - 1 << endl;

    cout << endl << "미로 결과" << endl;

    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(i == startX && j == startY) cout << "S ";
            else if(i == endX && j == endY) cout << "E ";
            else if(maze[i][j] == 1) cout << "# ";
            else {
                bool isPath = false;

                for(auto& p : path) {
                    if(p.first == i && p.second == j) {
                        isPath = true;
                        break;
                    }
                }

                if(isPath) cout << "* ";
                else cout << ". ";
            }
        }

        cout << endl;
    }

    return 0;
}