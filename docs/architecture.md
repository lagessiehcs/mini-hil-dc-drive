# Architecture

## Responsibilities

| Component | Responsibility |
| --- | --- |
| MATLAB / Simulink | derive the DC-motor model, select a sample time, compare continuous and discrete behavior, and create reference results |
| XMC Relax Lite | execute the discrete plant, publish simulated sensors, apply disturbances/faults, and measure timing/overruns |
| Arduino Uno | execute periodic PI/PID control, bound the output, validate feedback, and enter a safe state on timeout |
| PC + Python | configure tests, collect logs, evaluate requirements, and produce plots/reports |

## Initial communications boundary

The Arduino and XMC communicate over UART. The exact serial framing is not yet defined and must be documented as part of the interface contract.

## Signal contract to define before integration

| Direction | Signal | Required definition |
| --- | --- | --- |
| ECU → plant | actuator command | units, range, saturation, frame field |
| Plant → ECU | angular speed / RPM | units, validity encoding, timestamp or sequence number |
| Plant → ECU | optional current / diagnostic status | units and invalid-data handling |
| PC → plant | load torque / fault command | units, range, trigger timing |

The interface contract is maintained in `docs/`.

## Model reference

The armature-controlled DC-motor electrical and mechanical equations are based on the University of Michigan Control Tutorials reference: [DC Motor Speed: System Modeling](https://ctms.engin.umich.edu/CTMS/?example=MotorSpeed&section=SystemModeling). It provides the armature-circuit/free-body figure and derives the voltage, back-EMF, torque, inertia, and viscous-friction relationships.

The HIL plant adds an external load-torque term, `T_L`, to the mechanical equation so the simulated load can be varied during testing.
