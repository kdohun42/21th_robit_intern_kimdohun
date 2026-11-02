import gymnasium as gym
from stable_baselines3 import PPO

env = gym.make("Humanoid-v5", render_mode="human")
model = PPO.load("ppo_humanoid_A_1024000")
print("학습 스텝:", model.num_timesteps)

obs, info = env.reset()

for step in range(1000):
    action, _ = model.predict(obs, deterministic=True)
    obs, reward, terminated, truncated, info = env.step(action)

    if terminated or truncated:
        obs, info = env.reset()

env.close()