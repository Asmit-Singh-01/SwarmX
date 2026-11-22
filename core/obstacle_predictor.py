import numpy as np


class ObstacleTrajectoryPredictor:
    """Predicts future 2D/3D positions of moving obstacles using velocity."""

    def __init__(self, prediction_horizon_steps: int = 5):
        if prediction_horizon_steps < 1:
            raise ValueError("prediction_horizon_steps must be at least 1")
        self.horizon = prediction_horizon_steps

    def predict_trajectory(self, current_pos, current_velocity):
        pos = np.asarray(current_pos, dtype=float)
        vel = np.asarray(current_velocity, dtype=float)

        if pos.shape != vel.shape:
            raise ValueError("current_pos and current_velocity must have the same shape")

        return [(pos + vel * step).tolist() for step in range(1, self.horizon + 1)]

