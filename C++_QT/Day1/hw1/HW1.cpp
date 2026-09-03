#include <iostream>
#include <algorithm>
#include "HW1.hpp"
using namespace std;
 
void cl::arraySize(int n){
    num = n;
    array.resize(num); // 배열 크기 설정
}
 
void cl::min_max(){
 
    smallest = array[0]; 
    largest = array[0];
 
    for(int i = 0; i < num; i++){
        smallest = min(smallest, array[i]); // 최솟값
        largest = max(largest, array[i]); // 최댓값
    }
}
 
void cl::sum_make(){ // 합계
    for(int i = 0; i < num; i++){ 
        sum += array[i];
    }
}
 
void cl::avg_make(){
    avg = (double)sum / num; // 평균
}

int main(){

    cl c1; // 객체 생성

    cout << "몇 개의 원소를 할당하겠습니까? :"; 
    cin >> c1.num;

    c1.arraySize(c1.num);

    for(int i = 0; i < c1.num; i++){
        cout << "정수형 데이터 입력:";
        cin >> c1.array[i];
    }


    c1.min_max();

    cout << "최댓값: " << c1.largest << endl;
    cout << "최솟값: " << c1.smallest << endl;

    c1.sum_make();
    cout << "전체합: " << c1.sum << endl;

    c1.avg_make();
    cout<<fixed;
    cout.precision(6);
    cout << "평 균:" << c1.avg << endl;

    return 0;
}