# Baseline requirements

## Purpose and interpretation

These are the baseline requirements for the miniature DC-motor HIL bench. Each requirement has an observable acceptance criterion. Values marked **TBD** are configuration choices not yet defined in the project documentation.

## Timing and real-time execution

### REQ-RT-01 — Plant execution period

The XMC plant simulator shall update the complete plant state once per nominal **1 ms** period using a hardware-timer-triggered task.

**Verification:** record at least 1,000 consecutive task timestamps. The report shall state the nominal period, observed minimum/maximum period, and measurement method.

### REQ-RT-02 — Overrun detection

The XMC simulator shall increment an overrun counter whenever a plant-task computation fails to complete before the next 1 ms deadline.

**Verification:** expose the counter in telemetry or a test interface and demonstrate a deliberately induced overrun increases it. The normal long-run result shall report the counter value.

## ECU behavior

### REQ-ECU-01 — Controller execution period

The Arduino ECU shall execute its PI/PID control update once per nominal **10 ms** period.

**Verification:** record at least 100 consecutive controller timestamps and report the nominal period and observed range.

### REQ-ECU-02 — Bounded controller output

The ECU shall clamp its actuator command to configurable lower and upper limits before transmitting it to the plant simulator.

**Verification:** command an error large enough to demand output beyond both limits and confirm the transmitted command equals the applicable bound. Initial limits are **TBD** with the actuator-command units.

## Communication and safe state

### REQ-COM-01 — Feedback timeout

If valid speed feedback is absent for more than **50 ms**, the ECU shall enter a defined safe state.

**Safe state definition:** actuator command is zero, the controller integrator is reset or held (**implementation choice must be documented**), and the state is externally observable through telemetry or a status indicator.

**Verification:** stop valid feedback for at least 50 ms and confirm the safe state; resume valid feedback and document the recovery rule.

## HIL plant and fault injection

### REQ-HIL-01 — Configurable load torque

The plant simulator shall provide a configurable load-torque input used by the DC-motor model.

**Verification:** run two otherwise identical plant simulations with distinct load-torque values and show a corresponding difference in speed or current response. The supported range and units are **TBD**.

### REQ-HIL-02 — Injectable faults

The plant simulator shall support these independently selectable, externally triggered faults:

1. sensor freeze (feedback remains at its last valid value);
2. invalid sensor value; and
3. communication drop (no feedback message transmitted).

**Verification:** invoke each fault in a controlled test and record its activation, observable output, and ECU response.

## Test evidence

### REQ-TEST-01 — Reproducible automated test results

Python automation shall run the defined test cases from a documented command and generate machine-readable logs plus an unambiguous pass/fail summary.

**Verification:** run the same test command twice with the same configuration; both runs shall produce a summary and logs. The test suite contains at least five automated test cases mapped to requirement IDs.

## Traceability matrix

| Requirement | Primary owner | Test category |
| --- | --- | --- |
| REQ-RT-01, REQ-RT-02 | XMC plant firmware | timing / overrun |
| REQ-ECU-01, REQ-ECU-02 | Arduino ECU firmware | timing / saturation |
| REQ-COM-01 | Arduino ECU + UART protocol | timeout / recovery |
| REQ-HIL-01, REQ-HIL-02 | XMC plant firmware | response / fault injection |
| REQ-TEST-01 | Python test automation | regression evidence |

## Undecided configuration

- Confirm the UART electrical interface, baud rate, frame format, checksum, and timeout semantics.
- Set the actuator-command range/units and corresponding controller saturation limits.
- Set the supported load-torque range/units.
- Choose and document the safe-state recovery policy.
- Select DC-motor parameters and their source or justification.
