#include <iostream>
#include "HW3.hpp"
using namespace std;

// 몬스터의 최초 상태
Monster::Monster(int start_x, int start_y, int start_HP){
    x = start_x;
    y = start_y;
    HP = start_HP;
}

int Monster::Be_Attacked(int damage){
    HP = HP - damage;
    cout << "남은 체력:" << HP << endl;
    return HP;
}

// 플레이어의 최초 상태
Player::Player(int start_x, int start_y){
    HP = 50;
    MP = 10;
    x = start_x;
    y = start_y;
}

// 플레이어의 위치와 몬스터의 위치가 동일할 때 공격이 가능한 것과 공격 시 몬스터의 HP가 감소하는 것을 LLM의 도움을 받음
void Player::Attack(Monster &target){
    if(x == target.x && y == target.y){
        cout << "공격 성공!" << endl;
        target.Be_Attacked(10);
        MP = MP - 1;
    }
    else{
        cout << "공격 실패!" << endl;
    }
}

void Player::Show_status(){
    cout << "HP:" << HP << endl;
    cout << "MP:" << MP << endl;
    cout << "Position:" << x << "," << y << endl;
}

void Player::X_move(int move){
    x = x + move; // x 방향으로 이동
    cout << "X Position 1 moved!" << endl;
}

void Player::Y_move(int move){
    y = y + move; //  y 방형으로 이동
    cout << "Y Position 1 moved!" << endl;
}


int main(){

    //  플레이어 몬스터 처음 상태
    Player player(0,0);
    Monster monster(5,4,50);

    while(1){

    char command;

    cout << "Type Command(A/U/D/R/L/S)" << endl;
    cin >> command;

    // 명령 조건문
    if(command == 'A'){ // 공격
        player.Attack(monster);

        if(monster.HP <= 0){
            cout << "MONSTER DIED" << endl;
            break; // 프로그램 종료
        }

        if(player.MP <= 0){
            cout << "MP 부족" << endl;
            break; // 프로그램 종료
        }

    }

    if (command == 'U'){ // Y로 1
        player.Y_move(1);
    }

    if (command == 'D'){ // Y로 -1
       player.Y_move(-1);
    }

    if (command == 'R'){ // X로 1
       player.X_move(1);
    }

    if(command == 'L'){ // X로 -1
        player.X_move(-1);
    }
    if(command == 'S'){ // 현재 상태
        player.Show_status();
    }

    }

    return 0;
}