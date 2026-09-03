#include <vector>
using namespace std;
 
class cl{
 
public:
    int num;
    vector<int> array; // 배열 크기 설정
    void arraySize(int n);
    int smallest; // 최솟갑 
    int largest; // 최댓값
    void min_max(); // 최댓값, 최솟값 찾는 함수
    int sum = 0; // 합계 선언 및 초기화
    void sum_make(); // 합계 구하는 함수
    double avg = 0; // 평균 선언 및 초기화
    void avg_make(); // 평군 구하는 함수
 
}; 
 