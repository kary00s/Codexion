*This project has been created as part of the 42 curriculum by kanahiz.*

# codexion

## Description

`codexion` simulates a table of coders who share a limited set of "dongles" (hardware
keys) to do their work. Each coder repeatedly cycles through four phases:

1. **Acquire dongles** — a coder needs *two* dongles (their left and their right) before
   they can compile.
2. **Compile** — holding both dongles for `time_to_compile` ms.
3. **Debug** — `time_to_debug` ms, no dongles needed.
4. **Refactor** — `time_to_refactor` ms, no dongles needed, after which the coder
   immediately tries to acquire dongles again.

Dongles are arranged around the table coder `N` sits between coder `N-1` and coder `N+1`, and shares one
dongle with each neighbor. If a coder does not start compiling within
`time_to_burnout` ms of the beginning of their last compile (or the start of the
simulation), they burn out and the simulation stops.

The goal of the project is to implement this concurrent system correctly in C using
POSIX threads: one thread per coder, a monitor thread watching for burnout, and a
controller thread driving the simulation — all without a single global variable, and
with a hand-rolled priority queue (min-heap) to arbitrate dongle access under either a
`fifo` or `edf` (Earliest Deadline First) scheduling policy.

## Instructions

### Compilation

```sh
make        # builds the codexion binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

The code compiles with `-Wall -Wextra -Werror -pthread`.

### Usage

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
|---|---|
| `number_of_coders` | Number of coders (and dongles) at the table |
| `time_to_burnout` | ms since last compile start (or simulation start) before a coder burns out |
| `time_to_compile` | ms spent compiling (holding both dongles) |
| `time_to_debug` | ms spent debugging |
| `time_to_refactor` | ms spent refactoring, after which the coder immediately tries to compile again |
| `number_of_compiles_required` | simulation stops successfully once every coder has reached this many compiles |
| `dongle_cooldown` | ms a dongle stays unusable after being released |
| `scheduler` | `fifo` or `edf` — how contested dongles are granted |

All arguments are mandatory. Invalid input (negative numbers, non-integers, or a
scheduler other than `fifo`/`edf`) is rejected.

Example:

```sh
./codexion 4 800 200 200 200 3 5 edf
```

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions).** A coder needs both its left and right
  dongle at once. `codexion` breaks the circular wait by having odd- and even-numbered coders
  acquire their two dongles in opposite order (one neighbor first for odd coders, the
  other neighbor first for even coders). This removes the possibility of every coder
  holding one dongle while waiting forever for the other.

- **Starvation prevention.** Coders waiting for a dongle are not busy-polling; they are
  queued and woken with `pthread_cond_broadcast` as
  soon as a dongle becomes available and out of cooldown. This guarantees every waiting
  coder is eventually serviced rather than being skipped indefinitely.

- **Cooldown handling.** Each dongle tracks its own release timestamp and an
  "is_cold" state. A dongle is only handed out once it is both available and its
  cooldown has elapsed; 

- **Precise burnout detection.** A dedicated monitor thread tracks each coder's last
  compile-start time and wakes up (via timed condition waits) so a burnout is detected and logged
  **within the required 10 ms window.**

- **Log serialization.** All state-change messages (`has taken a dongle`, `is
  compiling`, `is debugging`, `is refactoring`, `burned out`) are printed under a single
  print mutex so two messages can never interleave on the same line.

## Thread synchronization mechanisms

Every shared resource is protected by a `pthread_mutex_t`/`pthread_cond_t` pair
(bundled together in a small `t_mutex_cond` helper struct) rather than any global
state — all shared data lives inside a heap-allocated "representer" struct that is
passed by pointer to every thread:

- **Per-dongle mutex + condition variable** — protects each dongle's `is_available`
  and `is_cold` flags. A coder trying to take a dongle locks the dongle's mutex, checks
  both flags, and calls `pthread_cond_wait` if the dongle isn't ready;

- **Queue mutex** — the shared min-heap used for `fifo`/`edf` ordering is protected by
  its own mutex, so concurrent inserts/pops from multiple coder threads can't corrupt
  the heap or interleave with a scheduling decision made by the monitor/controller.

- **Per-coder mutex + condition variable** — used to coordinate each coder's own state
  transitions (e.g. being told it's safe to start, or being woken by the controller)
  without polling.

- **Ready-coders counter mutex + condition variable** — acts as a start barrier: every
  coder thread signals it is initialized and ready, and the controller only lets the
  simulation begin once all coders have checked in, so timestamps are comparable from a
  common t=0.

- **Finished-coders mutex** — protects the shared counter of coders that have reached
  `number_of_compiles_required`, so the controller can safely check for a clean
  simulation end from multiple coder threads incrementing it concurrently.
- **is_burnout mutex** — protects the single shared "a coder has burned out" flag that
  the monitor thread sets and the controller thread polls to decide when to stop the
  simulation, avoiding a torn read/write of that flag between threads.
- **Print mutex** — the only lock touched by every thread purely for output; it
  guarantees each log line is written atomically.

Together these avoid the classic race conditions of this kind of problem: two coders
can never both believe they hold the same dongle (dongle mutex serializes the
check-and-take), the scheduling queue can never be corrupted by concurrent
modification (queue mutex), and the monitor and controller always observe a consistent,
non-torn view of burnout/completion state (dedicated flag mutexes) instead of
communicating through unsynchronized shared memory.

## Resources


- `man` pages for `pthread_create`, `pthread_mutex_lock`, `pthread_cond_wait`,
  `pthread_cond_timedwait`, `gettimeofday`
- [Heap](https://www.geeksforgeeks.org/c/heap-in-c/)
- [Multithreading](https://youtu.be/PgDaJEjlBuI?si=1ywhksiYQynKk64U)
- I created a summary for this project that includes all the necessary information and the knowledge I gained 
  - [View the summary](https://www.tldraw.com/f/m-epIM9usLJ6qFnvk3shP?d=v-7460.6620.22687.13732.page)

## AI Usage
- AI tools were used to assist with:

  - Structuring and organizing the README
  - Explaining concepts — how EDF scheduling works
  - Style/norm (i had some problems with norminette) 