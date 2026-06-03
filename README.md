*This project has been created as part of the 42 curriculum by tigondra.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic dining philosophers problem. Coders sit around a circular table and need two USB dongles to compile quantum code. They repeatedly compile, debug and refactor until either one coder burns out or every coder has compiled the required number of times.

The project focuses on POSIX threads, mutexes, condition variables, timing precision, resource sharing, cooldown management and scheduling policies.

## Instructions

Compile the project with:

```bash
make
```

Run it with:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 5 800 200 200 200 3 0 fifo
./codexion 5 800 200 200 200 3 50 edf
```

The scheduler argument must be either:

- `fifo`: requests are ordered by arrival time.
- `edf`: requests are ordered by earliest burnout deadline.

Clean build files with:

```bash
make clean
make fclean
make re
```

## Feature list

- One thread per coder.
- One monitor thread for burnout detection.
- Serialized logging with a print mutex.
- Stop state protected by a mutex.
- Shared dongles protected by mutexes.
- Dongle cooldown after release.
- FIFO and EDF scheduling using a custom binary heap.
- Simulation stop when a coder burns out or when all coders reach the required compile count.
- Special handling for the single-coder case.

## Blocking cases handled

### Deadlock prevention

The scheduler grants permission to coders through a centralized priority queue before any compile attempt. A coder can reserve both dongles only when they are available. This avoids the classic situation where several coders each hold one dongle while waiting forever for another one.

### Starvation prevention

Requests are stored in a binary heap. With FIFO, the oldest request is selected first. With EDF, the request with the closest burnout deadline is selected first, with the coder id used as a deterministic tie-breaker.

### Cooldown handling

Each dongle stores a `cooldown_until` timestamp. A dongle can only be reserved if it is not in use and its cooldown has expired. When a coder releases dongles, their cooldown timestamps are updated.

### Precise burnout detection

A separate monitor thread checks every coder frequently. If the time since the coder's last compile start exceeds `time_to_burnout`, the monitor prints the burnout message and stops the simulation.

### Log serialization

All output goes through a print mutex, ensuring two messages are never mixed on the same line.

## Thread synchronization mechanisms

The project uses several synchronization primitives:

- `pthread_mutex_t` for logging, stop state, coder state, scheduler queue and dongle state.
- `pthread_cond_t` to wake scheduler waiters when the queue or dongle availability changes.
- A custom binary heap to order scheduler requests.

Shared fields such as `last_compile`, `compile_count`, `stop`, `in_use` and `cooldown_until` are accessed under mutex protection. The monitor and coder threads communicate through the shared stop flag and the condition variable used by the scheduler.

## Resources

Useful references for this project:

- POSIX threads documentation.
- `pthread_mutex_lock`, `pthread_mutex_unlock` manual pages.
- `pthread_cond_broadcast` manual page.
- `gettimeofday` manual page.
- Dining philosophers problem explanations.
- Binary heap / priority queue algorithm references.

AI was used to effectively test the project by running a series of tests and verifying that the output did not contain any inconsistencies.

Thanks to sservant for creating a visualizer that lets us see exactly what our output looks like : https://codexion-visualizer.sacha-dev.me/ 