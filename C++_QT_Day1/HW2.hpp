#pragma once
#include <vector>
using namespace std;
 
struct PointXY {
    int x;
    int y;
};
 
extern vector<PointXY> p;   // 전역 변수 선언 (실제 정의는 .cpp에서)
 
class PointDistAnalyzer {
 
public:
    // 계산 결과를 저장할 멤버 변수들
    double maxDist;
    double minDist;
    PointXY maxPoint1, maxPoint2;   // 최댓값을 만든 두 점
    PointXY minPoint1, minPoint2;   // 최솟값을 만든 두 점
 
    // 두 점 사이 거리 계산
    double getDistance(PointXY a, PointXY b);
 
    // 모든 점 쌍을 비교해서 최댓값/최솟값과 그 좌표들을 멤버 변수에 저장
    void analyze(int num);
 
    // 결과 출력
    void printResult();
};
 