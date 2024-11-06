# Philosophers Project - 42 School

## Table of Contents
- [Overview](#overview)
- [Usage](#usage)
- [Objectives](#objectives)
- [Mandatory Part](#mandatory-part)
- [Bonus Part](#bonus-part)
- [Implementation](#implementation)
- [Additional Notes](#adittional-notes)
- [Resources](#resources)
- [Usage](#usage)

---

## Overview
This project simulates the Dining Philosophers Problem, a classic synchronization problem first proposed by Edsger Dijkstra. The problem involves a number of philosophers seated around a table with a fork between each pair. Philosophers alternate between eating, thinking, and sleeping. To eat, a philosopher needs two forks (or "chopsticks"), one from the left and one from the right. This project demonstrates how to handle concurrency issues such as deadlocks and race conditions.

---

## Usage

To compile and run the program:

  ```bash
    make
    ./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
Example:
  ```bash
    ./philosophers 5 800 200 200
```
Arguments:

<number_of_philosophers>: Number of philosophers and forks.

<time_to_die>: Time (in ms) that a philosopher can go without eating before dying.

<time_to_eat>: Time (in ms) that a philosopher spends eating.

<time_to_sleep>: Time (in ms) that a philosopher spends sleeping.

[number_of_times_each_philosopher_must_eat] (optional): The number of times each philosopher must eat before the simulation ends. If not provided, the simulation will run indefinitely until a philosopher dies.

---

## Objectives

The goal of this project is to:

- Understand and implement concurrency using threads.
- Properly manage resources with mutexes to avoid race conditions and deadlocks.
- Optionally, manage inter-process communication using semaphores and processes for the bonus.

---

## Mandatory Part

The mandatory part of this project requires implementing the dining philosophers simulation using threads and mutexes.

- Key Requirements:
Each philosopher must be a separate thread.
Forks are represented as mutexes to avoid simultaneous access.
The program should log when each philosopher starts eating, sleeping, or thinking.
If a philosopher does not start eating within time_to_die milliseconds since their last meal, they are considered dead, and the simulation stops.
Output format must be strictly followed for grading.

Program Workflow:
Each philosopher attempts to pick up the two forks adjacent to them.
After obtaining both forks, the philosopher eats, then releases the forks and goes to sleep.
After sleeping, the philosopher starts thinking and the cycle continues.
If a philosopher exceeds time_to_die without eating, the program should print that the philosopher has died and terminate.
Example Output:
csharp
Copiar código
0ms 1 is thinking
5ms 1 is eating
15ms 2 is sleeping
...
500ms 3 has died
Bonus Part
The bonus part of the project extends the requirements by using processes and semaphores instead of threads and mutexes.

Key Requirements:
Implement each philosopher as a separate process.
Use semaphores to manage the forks.
Each philosopher process monitors their own state to check if they exceed the time_to_die.
The main process should be able to track the philosophers and stop the simulation when a philosopher dies.
You must handle inter-process communication and termination properly to avoid zombie processes.
Additional Bonus Features:
Add a semaphore to control when all philosophers start eating, so they pick up forks at nearly the same time.
Ensure robust cleanup of all processes and semaphores to prevent resource leakage.
Implementation
Files and Structure
philosophers.c: Main file where philosophers are created, and threads or processes are managed.
philosophers.h: Header file containing function prototypes and shared data structures.
Makefile: To compile and clean the project.
utils.c: Utility functions for time management, logging, etc.
Key Functions
init_philosophers(): Initializes philosopher data, threads/processes, and semaphores or mutexes.
monitor_philosophers(): Monitors philosophers for death and terminates the simulation if any philosopher dies.
philosopher_routine(): Main routine for each philosopher to alternate between eating, thinking, and sleeping.
cleanup_resources(): Cleans up allocated memory, mutexes, and semaphores.
Additional Notes
Error Handling
Validate all input arguments and print an error message for invalid inputs.
Ensure all resources (threads, mutexes, processes, semaphores) are properly cleaned up to avoid memory leaks and dangling processes.
Avoiding Deadlocks
Use resource ordering or a timeout mechanism to avoid deadlocks where philosophers wait indefinitely for forks.
Synchronization
Implement careful synchronization for shared variables like the death status and log output to avoid race conditions.
Resources
For more information on the Dining Philosophers Problem and related synchronization issues, see:

Dining Philosophers Problem on Wikipedia
POSIX Threads Programming
Semaphore Tutorial
