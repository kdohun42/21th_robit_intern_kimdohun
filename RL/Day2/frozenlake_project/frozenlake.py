import gymnasium as gym
import numpy as np
import matplotlib.pyplot as plt
from collections import Counter

# State(상태)
env = gym.make(
    "FrozenLake-v1",
    map_name="4x4",
    is_slippery=False # 상태 전이 확률 1
)

rng = np.random.default_rng(0)

#Q-table 16 * 4
Q = np.zeros((
    env.observation_space.n,
    env.action_space.n # 0:왼쪽, 1:아래쪽, 2:오른쪽, 3:위쪽
))

#학습률
alpha = 0.1
#할인율 
gamma = 0.95
#에피소드
episodes = 5000

returns = []
train_successes = []


# Policy 함수(앱실론-그리디 정책)
def choose_action(state, epsilon, rng):
    # 탐험: 무작위 행동 선택
    if rng.random() < epsilon:
        return int(rng.integers(4))

    # 활용: 최대 Q-value 행동 선택
    best = np.flatnonzero(Q[state] == Q[state].max())

    # 동률인 경우 무작위 선택
    return int(rng.choice(best))


# Q-learning 학습
for episode in range(episodes): #에피소드 반복
    # 매에피소드마다 0으로 상태 초기화 
    state, _ = env.reset(seed=0 if episode == 0 else None)
    epsilon = max(0.05, 0.999 ** episode) # 에피소드가 증가할수록 앱실론 감소
    total = 0
    while True:
        #행동 선택(앱실론-그리디 정책)
        action = choose_action(state, epsilon, rng)
        #보상 R
        ns, r, terminated, truncated, _ = env.step(action)
        # Q-learning target 계산
        if terminated: #에피소드가 종료되었을 때
            target = r 
        else: #에피소드가 종료되지 않았을 때
            target = r + gamma * Q[ns].max() # 미래 가지 고려 계산
        # Q-value 업데이트
        Q[state, action] += alpha * (target - Q[state, action])
        total += r #보상 단순 합계
        state = ns
        if terminated or truncated: #종료되었다면 by 구명, 시간
            break
    returns.append(total)
    train_successes.append(int(total == 1))



# Q-table 저장 및 재로딩
np.save("q_table.npy", Q)
Q = np.load("q_table.npy")

print("Q-table 저장 및 재로딩 완료")
print(Q)


#학습 보상 및 성공률 곡선
window = 100

reward_avg = np.convolve(
    returns,
    np.ones(window) / window,
    mode="valid"
)

success_avg = np.convolve(
    train_successes,
    np.ones(window) / window,
    mode="valid"
)

x = np.arange(window, episodes + 1)

fig, axes = plt.subplots(1, 2, figsize=(12, 4))

# 학습 보상 곡선
axes[0].plot(x, reward_avg)
axes[0].set_title("Training Reward")
axes[0].set_xlabel("Episode")
axes[0].set_ylabel("Average Reward")
axes[0].set_ylim(0, 1.05)
axes[0].grid(True)

# 학습 성공률 곡선
axes[1].plot(x, success_avg)
axes[1].set_title("Training Success Rate")
axes[1].set_xlabel("Episode")
axes[1].set_ylabel("Success Rate")
axes[1].set_ylim(0, 1.05)
axes[1].grid(True)

plt.tight_layout()
#plt.show()



# Q-table Heatmap

plt.figure(figsize=(8, 7))

plt.imshow(Q, aspect="auto")
plt.colorbar(label="Q-value")

plt.title("Q-table Heatmap")
plt.xlabel("Action")
plt.ylabel("State")

plt.xticks(
    [0, 1, 2, 3],
    ["Left", "Down", "Right", "Up"]
)

plt.yticks(range(16))

# 각 칸에 Q-value 표시
for s in range(16):
    for a in range(4):
        plt.text(
            a, s,
            f"{Q[s, a]:.2f}",
            ha="center",
            va="center",
            fontsize=8
        )

plt.tight_layout()
#plt.show()



# 정책(Policy) 격자

desc = env.unwrapped.desc.astype(str)

arrows = {0: "LEFT", 1: "DOWN", 2: "RIGHT", 3: "UP"}

policy_grid = np.empty((4, 4), dtype=object)

for s in range(16):

    row, col = divmod(s, 4)
    cell = desc[row, col]

    if cell == "H":
        policy_grid[row, col] = "H"

    elif cell == "G":
        policy_grid[row, col] = "G"

    else:
        best_action = np.argmax(Q[s])

        if cell == "S":
            policy_grid[row, col] = "S/" + arrows[best_action]
        else:
            policy_grid[row, col] = arrows[best_action]


# 터미널에 정책 격자 출력
print("\nPolicy Grid")

for row in policy_grid:
    print(" ".join(f"{v:>3}" for v in row))


# 정책 격자 시각화
fig, ax = plt.subplots(figsize=(6, 6))

ax.set_xlim(-0.5, 3.5)
ax.set_ylim(3.5, -0.5)

ax.set_xticks(range(4))
ax.set_yticks(range(4))

ax.set_xticks(np.arange(-0.5, 4, 1), minor=True)
ax.set_yticks(np.arange(-0.5, 4, 1), minor=True)

ax.grid(which="minor")

for r in range(4):
    for c in range(4):
        ax.text(
            c, r,
            policy_grid[r, c],
            ha="center",
            va="center",
            fontsize=20
        )

ax.set_title("Greedy Policy Grid")

plt.tight_layout()
plt.show()



# 별도 200회 Greedy 평가

eval_env = gym.make(
    "FrozenLake-v1",
    map_name="4x4",
    is_slippery=False
)

eval_rng = np.random.default_rng(1000)

wins = 0
success_path_lengths = []

for ep in range(200):

    state, _ = eval_env.reset(
        seed=1000 if ep == 0 else None
    )

    steps = 0

    while True:

        # epsilon=0 : 탐험 없이 활용
        action = choose_action(state, 0.0, eval_rng)

        state, r, term, trunc, _ = eval_env.step(action)

        steps += 1

        if term or trunc:

            if r == 1:
                wins += 1
                success_path_lengths.append(steps)

            break



# 평가 결과 출력
success_rate = wins / 200

print("\n Result ")

print(f"Total Episodes: 200")
print(f"Success Count: {wins}")
print(f"Success Rate: {success_rate:.2%}")

if success_path_lengths:

    print(
        f"Average Successful Path Length: "
        f"{np.mean(success_path_lengths):.2f}"
    )

    print(
        f"Min Successful Path Length: "
        f"{np.min(success_path_lengths)}"
    )

    print(
        f"Max Successful Path Length: "
        f"{np.max(success_path_lengths)}"
    )

    print(
        f"Successful Path Length Distribution: "
        f"{dict(sorted(Counter(success_path_lengths).items()))}"
    )

else:
    print("No successful episodes.")

eval_env.close()
env.close()