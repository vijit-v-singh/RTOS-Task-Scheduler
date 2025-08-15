# Real-Time Task Scheduler with FreeRTOS on Raspberry Pi Pico

**Project Duration:** Dec 2024 - Jan 2025
**Type:** Academic Project

## Overview

This project implements a real-time multitasking system on a Raspberry Pi Pico using the FreeRTOS kernel. It demonstrates fundamental Real-Time Operating System (RTOS) concepts, including task scheduling and inter-task synchronization using semaphores.

The system runs two concurrent tasks:
1.  **Sensor Task**: Periodically reads the Pico's internal temperature sensor. If the temperature exceeds a predefined threshold, it signals the LED task.
2.  **LED Control Task**: Waits for a signal from the sensor task. Upon receiving the signal, it toggles the onboard LED to provide a visual alert.

A **binary semaphore** is used as the signaling mechanism to ensure thread-safe communication between the two tasks. 🚦



---

## Key Concepts Demonstrated

-   **Multitasking**: Two independent tasks (sensor reading and LED control) run concurrently, managed by the RTOS scheduler.
-   **Task Scheduling**: The FreeRTOS scheduler allocates CPU time to each task based on their priority and state (e.g., Running, Blocked, Ready).
-   **Inter-Task Communication**: The sensor task communicates with the LED task without direct function calls.
-   **Synchronization**: A semaphore is used to safely signal an event from one task to another, preventing race conditions and ensuring the LED task only acts when notified.

---

## Technologies & Tools Used

-   **Hardware**: Raspberry Pi Pico
-   **RTOS**: FreeRTOS Kernel
-   **Language**: C
-   **SDK**: Raspberry Pi Pico C/C++ SDK
-   **Build System**: CMake

---

## How It Works

1.  **Initialization**: The `main` function initializes the standard I/O, the onboard LED, and the internal temperature sensor. It then creates a binary semaphore.
2.  **Task Creation**: Two tasks, `vSensorTask` and `vLedTask`, are created with specific priorities.
3.  **Scheduler Start**: The FreeRTOS scheduler (`vTaskStartScheduler`) is started, which takes control of the CPU and begins managing the tasks.
4.  **Task Execution**:
    -   `vSensorTask` runs every 2 seconds. It reads the temperature. If the temperature is above `TEMPERATURE_THRESHOLD` (25.0°C), it "gives" the semaphore, releasing the other task.
    -   `vLedTask` spends most of its time "blocked," waiting to "take" the semaphore. It consumes zero CPU time in this state.
    -   When `vSensorTask` gives the semaphore, `vLedTask` unblocks, toggles the LED, prints a message, and immediately goes back to waiting for the next signal.

```
                  [Sensor Task]                                [LED Task]
                     (Runs every 2s)                        (Blocked, waiting...)
                           |                                        |
                  Read Temperature > 25°C?                          |
                           | Yes                                    |
                  -- Give Semaphore --> -- Signal Received -->  Unblocks
                           |                                        |
                 (Continues its loop)                         Toggle LED
                                                                    |
                                                              (Blocks again)
```

---

## Setup and Build Instructions

### Prerequisites

-   A Raspberry Pi Pico.
-   A development environment for Pico C/C++ development (VS Code with CMake/C++ extensions is recommended).
-   [ARM GNU Toolchain](https://developer.arm.com/downloads/-/gnu-rm).
-   [CMake](https://cmake.org/download/).

