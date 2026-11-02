import gymnasium as gym
import torch
import torch.nn as nn
import numpy as np
import matplotlib.pyplot as plt
import random
from collections import deque


# 1. 기본 설정
SEED = 0
STEPS = 5000

GAMMA = 0.95  # 비교 실험 시 직접 변경

BATCH_SIZE = 64
MEMORY_SIZE = 20000 # Replay Buffer 사이즈
LEARNING_RATE = 5e-4

WARMUP_STEPS = 1000
TRAIN_INTERVAL = 4
TARGET_UPDATE = 500

EVAL_INTERVAL = 5000
EVAL_EPISODES = 50

torch.manual_seed(SEED)
np.random.seed(SEED)
random.seed(SEED)

env = gym.make("CartPole-v1")
env.action_space.seed(SEED)


# 2. Q-Network
def make_net():
    return nn.Sequential(
        nn.Linear(4, 128),
        nn.ReLU(),
        nn.Linear(128, 128),
        nn.ReLU(),
        nn.Linear(128, 2)
    )


online = make_net()
target = make_net()

target.load_state_dict(online.state_dict())
target.eval()


# 3. Replay Buffer
memory = deque(maxlen=MEMORY_SIZE)


# 4. Optimizer / Loss Function
optimizer = torch.optim.Adam(
    online.parameters(),
    lr=LEARNING_RATE
)

loss_fn = nn.SmoothL1Loss()


# 5. Epsilon-Greedy
def select_action(state, epsilon):

    if random.random() < epsilon:
        return env.action_space.sample()

    with torch.no_grad():
        x = torch.as_tensor(
            state,
            dtype=torch.float32
        ).unsqueeze(0)

        action = online(x).argmax(dim=1).item()

    return action


# 6. DQN 학습
def optimize_batch():

    batch = random.sample(memory, BATCH_SIZE)

    states, actions, rewards, next_states, terms = zip(*batch)

    S = torch.tensor(
        np.array(states),
        dtype=torch.float32
    )

    A = torch.tensor(
        actions,
        dtype=torch.int64
    ).unsqueeze(1)

    R = torch.tensor(
        rewards,
        dtype=torch.float32
    )

    NS = torch.tensor(
        np.array(next_states),
        dtype=torch.float32
    )

    T = torch.tensor(
        terms,
        dtype=torch.float32
    )

    # Online Network 예측 Q-value
    prediction = online(S).gather(1, A).squeeze(1)

    # Target Network로 TD Target 계산
    with torch.no_grad():
        next_q = target(NS).max(dim=1).values
        y = R + GAMMA * (1 - T) * next_q

    # TD Loss
    loss = loss_fn(prediction, y)

    # 역전파
    optimizer.zero_grad()
    loss.backward()

    # Gradient Clipping
    nn.utils.clip_grad_norm_(
        online.parameters(), 10
    )

    optimizer.step()

    return loss.item()


# 7. Greedy / Random 정책 평가
def evaluate(
    model,
    policy="greedy",
    episodes=50,
    show_window=False
):

    # show_window=True면 실제 CartPole 창 출력
    render_mode = "human" if show_window else None

    eval_env = gym.make(
        "CartPole-v1",
        render_mode=render_mode
    )

    eval_env.action_space.seed(100)

    rewards = []

    if model is not None:
        was_training = model.training
        model.eval()

    for episode in range(episodes):

        state, _ = eval_env.reset(
            seed=1000 + episode
        )

        total_reward = 0

        while True:

            if policy == "random":
                action = eval_env.action_space.sample()

            else:
                with torch.no_grad():

                    x = torch.as_tensor(
                        state,
                        dtype=torch.float32
                    ).unsqueeze(0)

                    action = model(x).argmax(dim=1).item()

            state, reward, term, trunc, _ = eval_env.step(action)

            total_reward += reward

            if term or trunc:
                break

        rewards.append(total_reward)

        # 최종 시각화 평가 시 매 Episode 결과 출력
        if show_window:
            print(
                f"Evaluation Episode {episode + 1}/{episodes} | "
                f"Reward: {total_reward:.1f}"
            )

    print("50회 평가 반복문 종료", flush=True)

    if show_window:
        import pygame
        pygame.display.quit()
    else:
        eval_env.close()

    print("환경 종료 완료", flush=True)

    if model is not None and was_training:
        model.train()

    return np.array(rewards, dtype=float)


# 8. 학습 루프
state, _ = env.reset(seed=SEED)

train_rewards = []

eval_steps = []
eval_means = []
eval_stds = []

loss_steps = []
loss_history = []

epsilon_steps = []
epsilon_history = []

episode_reward = 0
episode_count = 0

last_loss = 0.0

# [수정 1] Optimizer Update 횟수 변수 추가
optimizer_updates = 0

online.train()

for step in range(1, STEPS + 1):

    # Epsilon Decay
    epsilon = max(
        0.05,
        1 - 0.95 * (step - 1) / 15000
    )

    epsilon_steps.append(step)
    epsilon_history.append(epsilon)

    # 행동 선택
    action = select_action(state, epsilon)

    # 환경과 상호작용
    ns, reward, term, trunc, _ = env.step(action)

    # 보상 함수 추가
    # 카트 위치와 막대 각도
    # x = ns[0]
    # theta = ns[2]

    # 중앙 유지 및 수직 유지 보상
    # shaped_reward = (
    #     reward
    #     - 0.1 * abs(x) / 2.4
    #     - 0.2 * abs(theta) / 0.2095
    # )

    # Replay Buffer에 경험 저장
    memory.append((
        state.copy(),
        action,
        reward,
        ns.copy(),
        term
    ))

    episode_reward += reward

    # Online Network 학습
    if len(memory) >= WARMUP_STEPS and step % TRAIN_INTERVAL == 0:

        last_loss = optimize_batch()

        # [수정 2] 실제 Optimizer Update 횟수를 기록
        optimizer_updates += 1

        loss_steps.append(optimizer_updates)
        loss_history.append(last_loss)

    # Target Network 업데이트
    if step % TARGET_UPDATE == 0:

        target.load_state_dict(
            online.state_dict()
        )

    # 다음 상태
    state = ns

    # Episode 종료 또는 시간 제한
    if term or trunc:

        episode_count += 1

        train_rewards.append(episode_reward)

        episode_reward = 0

        state, _ = env.reset()

    # 주기적인 Greedy 평가 (화면 출력 X)
    if step % EVAL_INTERVAL == 0:

        eval_rewards = evaluate(
            online,
            policy="greedy",
            episodes=EVAL_EPISODES,
            show_window=False
        )

        eval_steps.append(step)
        eval_means.append(np.mean(eval_rewards))
        eval_stds.append(np.std(eval_rewards))

        print(
            f"Step: {step} | "
            f"Episode: {episode_count} | "
            f"Epsilon: {epsilon:.3f} | "
            f"TD Loss: {last_loss:.4f} | "
            f"Eval Reward: {np.mean(eval_rewards):.2f}"
        )

env.close()


# 9. 모델 저장
torch.save(
    online.state_dict(),
    "cartpole_dqn.pt"
)

print("\nModel saved: cartpole_dqn.pt")


# 10. 모델 재로딩
model = make_net()

model.load_state_dict(
    torch.load(
        "cartpole_dqn.pt",
        map_location="cpu",
        weights_only=True
    )
)

model.eval()

print("Model loaded: cartpole_dqn.pt")


# 11. 최종 50회 평가

# 무작위 정책 평가
random_rewards = evaluate(
    None,
    policy="random",
    episodes=50,
    show_window=False
)

# DQN Greedy 평가 (실제 CartPole 화면 출력 O)
print("\n===== DQN Greedy Evaluation =====")

greedy_rewards = evaluate(
    model,
    policy="greedy",
    episodes=50,
    show_window=True
)


# 12. 최종 평가 결과 출력
def print_result(name, rewards):

    print(f"\n===== {name} =====")

    print(f"Total Episodes: {len(rewards)}")
    print(f"Mean Reward: {np.mean(rewards):.2f}")
    print(f"Std Reward: {np.std(rewards):.2f}")
    print(f"Min Reward: {np.min(rewards):.2f}")
    print(f"Max Reward: {np.max(rewards):.2f}")


print_result("DQN Greedy", greedy_rewards)
print_result("Random Policy", random_rewards)

print("\n===== Performance Comparison =====")

print(
    f"Reward Difference: "
    f"{np.mean(greedy_rewards) - np.mean(random_rewards):+.2f}"
)


# 13. 그래프 출력
def moving_average(data, window=100):

    data = np.asarray(data, dtype=float)

    if len(data) < window:
        return data

    return np.convolve(
        data,
        np.ones(window) / window,
        mode="valid"
    )


fig, axes = plt.subplots(2, 2, figsize=(13, 9))


# 1. Training Reward
ax = axes[0, 0]

smoothed_rewards = moving_average(
    train_rewards, 100
)

episode_axis = np.arange(
    len(train_rewards) - len(smoothed_rewards) + 1,
    len(train_rewards) + 1
)

ax.plot(
    episode_axis,
    smoothed_rewards
)

ax.set_title("Training Reward (Moving Average 100)")
ax.set_xlabel("Episode")
ax.set_ylabel("Reward")
ax.grid(alpha=0.3)


# 2. Evaluation Reward
ax = axes[0, 1]

eval_means = np.array(eval_means)
eval_stds = np.array(eval_stds)

ax.plot(
    eval_steps,
    eval_means,
    marker="o"
)

ax.fill_between(
    eval_steps,
    eval_means - eval_stds,
    eval_means + eval_stds,
    alpha=0.2
)

ax.set_title("Greedy Evaluation Reward (50 Episodes)")
ax.set_xlabel("Training Step")
ax.set_ylabel("Mean Reward")
ax.grid(alpha=0.3)


# 3. TD Loss
ax = axes[1, 0]

smoothed_loss = moving_average(
    loss_history, 200
)

ax.plot(
    loss_steps[-len(smoothed_loss):],
    smoothed_loss
)

ax.set_title("TD Loss (Moving Average 200)")

# [수정 3] X축을 Optimizer Update로 변경
ax.set_xlabel("Optimizer Update")

ax.set_ylabel("Smooth L1 Loss")
ax.grid(alpha=0.3)


# 4. Epsilon
ax = axes[1, 1]

ax.plot(
    epsilon_steps,
    epsilon_history
)

ax.set_title("Epsilon Decay")
ax.set_xlabel("Training Step")
ax.set_ylabel("Epsilon")
ax.grid(alpha=0.3)


plt.tight_layout()

print("그래프 출력 시작")
print("Matplotlib Backend:", plt.get_backend())

plt.show(block=True)

print("그래프 창 종료")
