## 강화학습 교육 1일차 코드 Ubuntu 24.04 환경 기준 실행 방법

폴더 구조 <br>
mnist_project <br>
├── mnist_model.py<br>
└── data

mnist_Project 디렉토리 안에 코드와 데이터가 위치해 있어야 함. 코드 실행 시 데이터는 자동으로 다운로드 됨.

```
mkdir -p ~/mnist_project
cd ~/mnist_project
```

파이썬 설치
```
sudo apt update
sudo apt install -y python3-venv python3-pip
```

가상환경 생성 및 활성화
```
python3 -m venv .venv
source .venv/bin/activate
```

라이브러리 설치
```
python -m pip install --upgrade pip
python -m pip install torch torchvision --index-url https://download.pytorch.org/whl/cpu
python -m pip install matplotlib
```

터미널 실행
```
python mnist_model.py
```
