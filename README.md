# 🍝 Philosophers

A multithreaded simulation of the classic **Dining Philosophers Problem**, built in C using POSIX threads and mutexes. This project explores concurrency, synchronization, and deadlock avoidance.

> Part of the [42 school](https://42.fr/) curriculum.

---

## 📖 About

The Dining Philosophers problem is a well-known concurrency challenge first formulated by Edsger Dijkstra in 1965. In this simulation:

- A number of philosophers sit around a circular table.
- Each philosopher alternates between **eating**, **sleeping**, and **thinking**.
- There is one fork between each pair of philosophers. A philosopher must hold **both** adjacent forks to eat.
- If a philosopher does not eat within a given time window, they die of starvation, and the simulation ends.

This implementation uses **one thread per philosopher** and **mutexes** to protect shared resources (forks), ensuring data-race-free execution.

---

## ✨ Features

- **POSIX Threads** — each philosopher runs in its own thread
- **Mutex-based Fork Protection** — prevents simultaneous access to the same fork
- **Atomic Operations** — thread-safe counters for meals and timestamps without lock overhead
- **Precise Timing** — millisecond-accurate scheduling using `gettimeofday`
- **Death Monitoring** — a dedicated monitor thread detects starvation in real time
- **Deadlock Prevention** — fork acquisition order is strategically assigned to avoid circular wait
- **Edge Case Handling** — graceful handling of a single-philosopher scenario

---

## 🛠️ Build

### Prerequisites

- A C compiler (`cc`, `gcc`, or `clang`)
- `make`
- POSIX-compliant system (Linux / macOS)

### Compile

```bash
cd philo
make
```

Other Makefile targets:

| Target       | Description                              |
|--------------|------------------------------------------|
| `make`       | Build the project                        |
| `make clean` | Remove object files                      |
| `make fclean`| Remove object files and the executable   |
| `make re`    | Clean and rebuild from scratch           |

---

## 🚀 Usage

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]
```

| Argument                  | Description                                                                 |
|---------------------------|-----------------------------------------------------------------------------|
| `number_of_philosophers`  | Number of philosophers (and forks) at the table                             |
| `time_to_die` (ms)       | Time in milliseconds a philosopher can survive without eating               |
| `time_to_eat` (ms)       | Time in milliseconds it takes for a philosopher to eat                      |
| `time_to_sleep` (ms)     | Time in milliseconds a philosopher spends sleeping                          |
| `number_of_meals`        | *(Optional)* Simulation stops after every philosopher has eaten this many times |

### Examples

```bash
# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep
./philo 5 800 200 200

# Same as above, but stop after each philosopher has eaten 7 times
./philo 5 800 200 200 7

# 4 philosophers with a tighter death timer
./philo 4 410 200 200
```

### Output Format

Each state change is printed as a timestamped log:

```
timestamp_in_ms  philosopher_id  action
```

Available actions:

| Log Message                           | Meaning                            |
|---------------------------------------|------------------------------------|
| `X has taken a fork`                  | Philosopher X picked up a fork     |
| `X is eating`                         | Philosopher X is eating            |
| `X is sleeping`                       | Philosopher X is sleeping          |
| `X is thinking`                       | Philosopher X is thinking          |
| `X died`                              | Philosopher X died of starvation   |

---

## 🏗️ Project Structure

```
philo/
├── Makefile                # Build configuration
├── main.c                  # Entry point
├── lib/
│   └── philo.h             # Header file with structs and prototypes
└── src/
    ├── ft_atoi.c           # Custom string-to-integer converter
    ├── dinner_simulation.c # Core philosopher routine (eat, sleep, think)
    ├── exception.c         # Single-philosopher edge case
    ├── init_all.c          # Initialization of mutexes, forks, and philosophers
    ├── monitoring.c        # Monitor thread for death detection
    ├── parsing.c           # Command-line argument validation
    └── philo_utils.c       # Utilities (time, sleep, cleanup)
```

---

## 🧠 How It Works

1. **Parsing & Initialization** — Command-line arguments are validated, and the table (philosophers, forks, mutexes) is initialized.
2. **Simulation Start** — Each philosopher is spawned as a separate thread. A monitor thread is created to watch for starvation.
3. **Philosopher Routine** — Each philosopher repeatedly:
   - Picks up two forks (mutex lock)
   - Eats for `time_to_eat` ms
   - Puts down forks (mutex unlock)
   - Sleeps for `time_to_sleep` ms
   - Thinks until forks are available
4. **Death Detection** — The monitor thread continuously checks whether any philosopher has exceeded `time_to_die` ms since their last meal.
5. **Termination** — The simulation ends when a philosopher dies or when all philosophers have eaten the required number of meals.

---

## ⚙️ Technical Details

| Concept              | Implementation                                      |
|----------------------|-----------------------------------------------------|
| Threading            | `pthread_create` / `pthread_join`                   |
| Mutual Exclusion     | `pthread_mutex_t` for each fork                     |
| Atomic Variables     | `atomic_int` / `atomic_long` for shared counters    |
| Time Measurement     | `gettimeofday()` with millisecond precision         |
| Deadlock Avoidance   | Even/odd philosopher fork ordering                  |

---

## 📄 License

This project was developed as part of the 42 school curriculum.
