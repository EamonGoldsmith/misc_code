import numpy as np

def kalman_filter(measurements, initial_state, initial_covariance, F, H, Q, R):
    num_states = initial_state.shape[0]
    num_measurements = measurements.shape[0]

    x = initial_state.copy()
    P = initial_covariance.copy()

    filtered_states = []

    for z in measurements:
        # Predict
        x_pred = np.dot(F, x)
        P_pred = np.dot(F, np.dot(P, F.T)) + Q

        # Update
        y = z - np.dot(H, x_pred)
        S = np.dot(H, np.dot(P_pred, H.T)) + R
        K = np.dot(P_pred, np.dot(H.T, np.linalg.inv(S)))

        x = x_pred + np.dot(K, y)
        P = P_pred - np.dot(K, np.dot(H, P_pred))

        filtered_states.append(x.copy())

    return filtered_states

# Example usage (same parameters as FilterPy example)
dt = 1.0
initial_state = np.array([[0.], [0.]])
initial_covariance = np.array([[1000., 0.], [0., 1000.]])
F = np.array([[1., dt], [0., 1.]])
H = np.array([[1., 0.]])
Q = np.array([[0.001, 0.], [0., 0.001]])
R = np.array([[0.1]])
measurements = np.array([0.1, 0.5, 1.2, 1.8, 2.3, 2.9, 3.5, 4.0, 4.6, 5.1]).reshape(-1, 1)

filtered_states = kalman_filter(measurements, initial_state, initial_covariance, F, H, Q, R)

print("Filtered States (position, velocity) from custom implementation:")
for state in filtered_states:
    print(state.flatten())
