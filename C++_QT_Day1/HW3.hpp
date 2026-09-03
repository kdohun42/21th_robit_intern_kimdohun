#pragma once 

class Monster{
    public:
        int HP, x, y;
        //Monster();
        Monster(int x, int y, int HP); // 몬스터 최초 상태
        int Be_Attacked(int damage); // 몬스터 공격 당함 
};

class Player{
    public:
        int HP, MP, x, y;
        //Player();
        Player(int x, int y); // 플레이어 최초 상태
        void Attack(Monster &target); // 몬스터 공격 함수
        void Show_status(); // 현재 플레이어 상태 정보 함수
        void X_move(int move); // 플레이어 X로 이동하는 함수
        void Y_move(int move); // 플레이어 y로 이동하는 함수
};