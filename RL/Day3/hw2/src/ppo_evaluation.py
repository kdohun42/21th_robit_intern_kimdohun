import gymnasium as gym
import numpy as np

from stable_baselines3 import PPO
from stable_baselines3.common.monitor import Monitor
from stable_baselines3.common.evaluation import evaluate_policy

model = PPO.load("ppo_humanoid_A_204800")

env = Monitor(gym.make("Humanoid-v5"))

rewards, lengths = evaluate_policy(
    model,
    env,
    n_eval_episodes=20,
    deterministic=True,
    return_episode_rewards=True
)

print("평균 Episode Reward:", np.mean(rewards))
print("Episode Reward 표준편차:", np.std(rewards))
print("평균 Episode Length:", np.mean(lengths))
print("최소 Episode Length:", np.min(lengths))
print("최대 Episode Length:", np.max(lengths))

env.close()