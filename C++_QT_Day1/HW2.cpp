
#include <iostream>
#include <random>
#include <cmath>
#include "HW2.hpp"
using namespace std;
 
vector<PointXY> p;   // 전역 변수의 실제 정의 (헤더에는 extern 선언만 있었음)
 
// 두 점 사이의 거리 
double PointDistAnalyzer::getDistance(PointXY a, PointXY b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
 
void PointDistAnalyzer::analyze(int num) {
    // 첫 번째 쌍의 거리로 초기화 (0으로 시작하면 안 됨)
    maxDist = getDistance(p[0], p[1]);
    minDist = maxDist;
    maxPoint1 = p[0]; maxPoint2 = p[1];
    minPoint1 = p[0]; minPoint2 = p[1];
 
    for (int i = 0; i < num; i++) {
        for (int j = i + 1; j < num; j++) {
 
            double d = getDistance(p[i], p[j]);
 
            if (d > maxDist) {
                maxDist = d;
                maxPoint1 = p[i];
                maxPoint2 = p[j];
            }
            if (d < minDist) {
                minDist = d;
                minPoint1 = p[i];
                minPoint2 = p[j];
            }
        }
    }
}
 
void PointDistAnalyzer::printResult() {
    cout << "최대 거리 = " << maxDist;
    cout << "  점1 (" << maxPoint1.x << ", " << maxPoint1.y << "), 점2(" << maxPoint2.x << ", " << maxPoint2.y << ")" << endl;
 
    cout << "최소 거리 = " << minDist;
    cout << " 점1 (" << minPoint1.x << ", " << minPoint1.y << "), 점2(" << minPoint2.x << ", " << minPoint2.y << ")" << endl;
}

int main() {

    int point_count = 0, point_min = 0, point_max = 0;

    cout << "점의 개수를 입력하세요:";
    cin >> point_count;

    p.resize(point_count); // 배열 크기 설정

    cout << "좌표의 범위(최솟값)입력하세요:";
    cin >> point_min;
    cout << "좌표의 범위(최댓값) 입력하세요:";
    cin >> point_max;

    // 난수 생성 LLM 도움 받음
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(point_min, point_max);

    cout << "\n랜덤 점 생성" << endl;
    for (int i = 0; i < point_count; i++) {
        p[i].x = dis(gen);
        p[i].y = dis(gen);
        cout << "점" << i << ": (" << p[i].x << ", " << p[i].y << ")" << endl;
    }

    cout << "\n========= 결과 ========" << endl;

    PointDistAnalyzer analyzer;
    analyzer.analyze(point_count);
    analyzer.printResult();

    return 0;
}