# ECE 361 – Homework 5

## About This Homework

This homework is about a finite state machine (FSM) in C.

I created a combination lock that opens when the user enters the correct code: **3-1-4**.

The program uses a transition table to decide the next state and action.

## States

- **LOCKED:** The door is locked.
- **GOT1:** The first correct digit (3) was entered.
- **GOT2:** The first two correct digits (3, 1) were entered.
- **OPEN:** The correct code was entered, and the door is open.

## Events

- **E_RIGHT:** The correct next digit is entered.
- **E_WRONG:** A wrong digit is entered.
- **E_CLOSE:** The door closes.
- **E_TIMEOUT:** The timer runs out.

## State Diagram

```text
LOCKED --3--> GOT1 --1--> GOT2 --4--> OPEN
   ^            |            |           |
   |            |            |           |
   +----wrong---+----wrong---+           |
   |                                     |
   +--------------close/timeout----------+

Timeout from GOT1 or GOT2 goes to LOCKED.
Wrong digits in LOCKED stay in LOCKED.
Closing in LOCKED, GOT1, or GOT2 changes nothing.
Digits in OPEN do nothing.
```

## Transition Table

Each cell shows the next state and action.

| State     | E_RIGHT       | E_WRONG      | E_CLOSE      | E_TIMEOUT     |
|-----------|---------------|--------------|--------------|---------------|
| LOCKED    | GOT1, NONE    | LOCKED, BEEP | LOCKED, NONE | LOCKED, NONE  |
| GOT1      | GOT2, NONE    | LOCKED, BEEP | GOT1, NONE   | LOCKED, NONE  |
| GOT2      | OPEN, UNLOCK  | LOCKED, BEEP | GOT2, NONE   | LOCKED, NONE  |
| OPEN      | OPEN, NONE    | OPEN, NONE   | LOCKED, LOCK | LOCKED, LOCK  |

## Files

- `lock.h`: Defines the states, events, actions, and functions.
- `lock.c`: Contains the transition table and functions.
- `main.c`: Reads user input and prints the results.
- `tests/test_lock.c`: Tests the program.
- `data/codes.txt`: Contains sample input.
- `Makefile`: Builds and tests the program.

## How to Build

Open the terminal inside `hw05` and type:

```bash
make
```

## How to Run

```bash
printf "314" | ./lock
```

Expected output:

```text
LOCKED 3 RIGHT -> GOT1 NONE
GOT1 1 RIGHT -> GOT2 NONE
GOT2 4 RIGHT -> OPEN UNLOCK
final: OPEN
```

To run the sample input file:

```bash
./lock < data/codes.txt
```

## How to Test

```bash
make test
```

My test results:

```text
Summary: 46 passed, 0 failed
```

## How to Clean

```bash
make clean
```

This removes the compiled program and object files.
