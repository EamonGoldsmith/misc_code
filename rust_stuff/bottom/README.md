# A Real-Time System Monitor (TUI-based top)
A Terminal User Interface (TUI) that dynamically updates, handles keyboard input raw events, and samples system data.

- **The Goal**: Create a terminal dashboard that displays live CPU usage, memory consumption, and a list of running processes.

- **Skills learned**: Working with third-party crates, handling terminal escape codes/raw mode, and structuring an application event loop.

- **Core Requirements**:
Use the ratatui (or crossterm) crate for rendering the UI layout and handling keyboard events (like pressing 'q' to exit).
Use the sysinfo crate to poll system metrics.
Update the screen automatically every 500ms using a clean rendering loop.

- **Bonus Stretch**: Add a feature to filter the process list by name, or allow the user to select a process with the arrow keys and press 'k' to terminate it.
