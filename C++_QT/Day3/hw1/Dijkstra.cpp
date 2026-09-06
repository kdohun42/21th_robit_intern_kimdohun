#include <iostream>
#include <vector>

using namespace std;

vector<pair<int, int> > graph[5]; // 그래프 인접 리스트로 구현하기 노드 개수 5개

vector<int> Dijkstra(int start); //다익스트라 함수

int main(){

    int start_node; // 시작 노드
    vector<int> output_dist; //

    // 그래프 그리기
    // 노드A
    graph[0].push_back({1, 7});
    graph[0].push_back({4, 1});
    // 노드B
    graph[1].push_back({0, 7});
    graph[1].push_back({2, 3});
    graph[1].push_back({4, 8});
    // 노드C
    graph[2].push_back({1, 3});
    graph[2].push_back({4, 2});
    graph[2].push_back({3, 6});
    // 노드D
    graph[3].push_back({2, 6});
    graph[3].push_back({4, 7});
    // 노드E
    graph[4].push_back({0, 1});
    graph[4].push_back({1, 8});
    graph[4].push_back({2, 2});
    graph[4].push_back({3, 7});


    cout << "0, 1, 2, 3, 4 중 시작 노드를 입력해주세요:";
    cin >> start_node;

    output_dist = Dijkstra(start_node); // 다익스트라 함수 

    cout << "최종 결과" << endl;
    for(int i = 0; i < 5; i++){
        cout << output_dist[i] << " ";
    }

    return 0;
}

vector<int> Dijkstra(int start){

    int INF = 2147483647; // int 자료형 최댓값
    vector<int> dist(5, INF); // 
    vector<bool> visited(5,false); // 

    dist[start] = 0; // 시작 노드 설정

    for(int i = 0; i < 5; i++){

        int u = -1;
        for(int j = 0; j < 5; j++){
            if(!visited[j] && (u == -1 || dist[j] < dist[u])){
                u = j;
            }
        }

        if(dist[u] == INF){
            break;
        }

        visited[u] = true;

        for(auto& a : graph[u]){
            int v = a.first;
            int weight = a.second;

            if(dist[u] + weight < dist[v]){
                dist[v] = dist[u] + weight;
            }
        }

        for(int i = 0; i < 5; i++){
            cout << dist[i] << " ";
        }

        cout << "\n";
    }
    return dist;
}

