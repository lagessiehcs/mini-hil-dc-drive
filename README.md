# Mini Automotive HIL Bench

A small, educational hardware-in-the-loop (HIL) bench definition: an Arduino Uno is the ECU, an XMC Relax Lite hosts the real-time DC-motor plant model, MATLAB/Simulink supports model development and validation, and Python supports test automation and reporting.

This is a miniature real-time HIL prototype. UART is the defined communication path between the Arduino and XMC.

## System boundary

```text
target speed -> Arduino ECU (PI/PID) -> XMC plant simulator
                   ^                       |
                   |---- simulated feedback-+
```

- **Arduino Uno:** controller / hardware under test; the controller updates every 10 ms (100 Hz).
- **XMC Relax Lite:** deterministic DC-motor plant and simulated sensors; the plant updates every 1 ms (1 kHz).
- **PC:** MATLAB/Simulink model development and Python-based test automation.

The bench runs continuously. The 10 ms value is the controller-update interval, not the duration of a demonstration or test. For every controller update, the plant normally advances through ten 1 ms simulation steps.

## Repository layout

- `docs/` — requirements, architecture, model assumptions, and test evidence.
- `model/` — MATLAB scripts and Simulink model files.
- `xmc-hil/` — XMC real-time plant firmware.
- `arduino-ecu/` — Arduino controller firmware.
- `tests/` — Python test automation and test definitions.
- `results/` — generated logs, plots, and reports (not source assets).

## Working conventions

- Use SI units internally: current in A, angular speed in rad/s, torque in N·m, time in s.
- Keep hardware-specific pin mappings and serial settings documented before firmware depends on them.
- Generated outputs belong in `results/` and are excluded from version control.
- Track MATLAB scripts (`.m`) and Simulink models (`.slx`) in `model/`; generated cache files (`.slxc`) and `slprj/` build artifacts are excluded.
