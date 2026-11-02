from stable_baselines3 import PPO
from stable_baselines3.common.env_util import make_vec_env

env = make_vec_env(
    "Humanoid-v5",
    n_envs=1,
    seed=0,
    monitor_dir="logs/A_seed0"
)

model = PPO(
    "MlpPolicy",
    env,
    learning_rate=3e-4,
    seed=0,
    verbose=1,
    device="cpu"
)

#model.learn(total_timesteps=10_000)
model.learn(total_timesteps=204_800)

#model.save("ppo_humanoid_A_seed0")
model.save("ppo_humanoid_A_204800")
env.close()

