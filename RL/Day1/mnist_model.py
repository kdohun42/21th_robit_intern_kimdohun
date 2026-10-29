import torch
import matplotlib.pyplot as plt
from torch import nn
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms

# 난수 생성 시드 설정
num = 0
torch.manual_seed(num)
print("seed:",num)
# 여기서 난수는 초기 가중치와 학습 데이터와 검증 데이터 비율
# 역할 : 매실행마다 달라지는 난수에 의해 결과가 달라지는 것을 막기 위함.

#MNIST 학습 데이터 불러오기
full = datasets.MNIST("data", train=True, download=True, transform=transforms.ToTensor()) 
# train = True -> MNIST 학습용 데이터 60000장 불러오기


# 학습 데이터와 검증 데이터 분리
train, val = random_split(full, [55000, 5000], generator=torch.Generator().manual_seed(0))
# full에 저장되어 있는 60000장의 데이터를 무작위로 train과 val에 각각 55000개와 5000개로 나눔
# 역할 : 과적합을 방지함


#테스트 데이터 불러오기
test = datasets.MNIST("data", train=False, download=True, transform=transforms.ToTensor())
# train = False -> MNIST 테스트용 데이터 10000장 불러오기
# 테스트 데이터는 성능을 평가하는 용도로 사용됨.


# 신경망에 데이터 전달 코드
train_loader = DataLoader(train, batch_size=64, shuffle=True)
# batch_size=64, 55000개의 데이터를 64개씩 묶어 860번 전달 (55000 / 64 = 860) 
# shuffle=True, 한 배치(64개)에 있는 데이터를 무작위로 섞어 신경망에 전달

val_loader = DataLoader(val, batch_size=256)
#검증 데이터 전달
test_loader = DataLoader(test, batch_size=256)
#학습 데이터 전달

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


#신경망 구성
model = nn.Sequential(nn.Flatten(), # 28 * 28 = 784
                      nn.Linear(784, 128),
                      nn.ReLU(),
                      nn.Linear(128, 10)
                      ).to(device)

# 손실 함수 설정
criterion = nn.CrossEntropyLoss()

Learning_Rate= 1e-4
optimizer = torch.optim.Adam(model.parameters(), lr=Learning_Rate) #최적화 알고리즘 Adam 사용
print("Learning_Rate", Learning_Rate)

#학습 횟수
epochs = 5

# 실제 학습 (5번)
for epoch in range(epochs):

    model.train() #학습 모드
    total_loss = 0.0 #반복문이 돌때마다 초기화

    for images, labels in train_loader:

        # 설정한 연산 장치로 이동(이 코드에서는 CPU)
        images = images.to(device)
        labels = labels.to(device)

        optimizer.zero_grad() #기울기 초기화
        logits = model(images) #순전파 28 * 28 -> 784 -> 128 -> 10

        loss = criterion(logits, labels) #위에서 설정한 손실 함수로 손실값 계산

        # 역전파
        loss.backward() # 기울기 계산
        optimizer.step() # 가중치와 편항 수정

        total_loss += loss.item() * images.size(0)
        train_loss = total_loss / len(train)

    print("epoch:", epoch + 1, ", loss:", train_loss)

# 성능 평가 함수
def evaluate(loader):
    model.eval() #평가 모드
    loss_sum, correct, count = 0.0, 0, 0

    with torch.no_grad():
        for images, labels in loader:

            images, labels = images.to(device), labels.to(device)
            logits = model(images)

            # 전체 손실 합계
            loss_sum += criterion(logits, labels).item() * len(labels)
            # 모델이 정답을 맞힌 이미지 개수
            correct += (logits.argmax(1) == labels).sum().item()
            # 평가한 전체 이미지 개수
            count += len(labels)

    return loss_sum / count, correct / count

test_loss, test_accuracy = evaluate(test_loader)
print("[테스트 데이터]")
print("평균 손실", test_loss,", accuracy:", test_accuracy * 100, "%")

val_loss, val_accuracy = evaluate(val_loader)
print("[검증 데이터]")
print("평균 손실", val_loss,", accuracy:", val_accuracy * 100, "%")



# 오분류 사례 분석
model.eval()

wrong_images = [] # 잘못 분류된 이미지
wrong_labels = [] # 실제 정답 숫자
wrong_preds = [] # 모델이 예측한 숫자

with torch.no_grad():
    for images, labels in test_loader:
        #각각 이미지와 정답 256개씩 연산 장치로 이동 
        images = images.to(device) #이미지
        labels = labels.to(device) #정답

        # 모델 예측
        logits = model(images)
        preds = logits.argmax(dim=1) # 10개의 로짓중 최댓값을 찾음

        # 오분류된 이미지 찾기
        wrong_mask = preds != labels #정답과 예측값이 다르면 trun, 같으면 false

        # 오분류 결과 저장
        # trun 인 것들만 저장
        wrong_images.extend(images[wrong_mask].cpu())
        wrong_labels.extend(labels[wrong_mask].cpu().tolist())
        wrong_preds.extend(preds[wrong_mask].cpu().tolist())

        # 10개 이상 찾으면 종료
        if len(wrong_images) >= 10: 
            break


# 오분류 이미지 10개 출력, 2행 5열 10개
fig, axes = plt.subplots(2, 5, figsize=(12, 5))

for i, ax in enumerate(axes.flat):

    if i >= min(10, len(wrong_images)):
        ax.axis("off")
        continue

    ax.imshow(wrong_images[i].squeeze(), cmap="gray")
    ax.set_title(f"class: {wrong_labels[i]} / Pred: {wrong_preds[i]}", color="red")
    ax.axis("off")

plt.tight_layout()
plt.show()

