# ECE 361 – Homework 5 Bonus

## About the Bonus

I created a table checker for my finite state machine (FSM).

The checker looks for problems in the transition table and prints the errors.

It checks for:

- Missing table entries
- Invalid next states
- Unreachable states
- States with no way out

## How the Checker Works

First, the checker goes through every entry in the transition table.

Second, it checks if an action is undefined or if the next state is invalid.

Third, it uses breadth-first search (BFS) to find states that can be reached from `LOCKED`.

Finally, it checks if a state has no valid way to leave.

The function returns the number of problems found.

## Test 1: Missing Entry

I changed the action for `GOT1` and `E_RIGHT` to `A_UNDEFINED`.

Checker output:

```text
GOT1 RIGHT: undefined entry
GOT2: unreachable
OPEN: unreachable
```

This is a problem because the lock cannot correctly move forward after entering the first digit. The checker also finds that later states cannot be reached.

## Test 2: Invalid Next State

I changed the next state for `GOT2` and `E_RIGHT` to `N_STATES`, which is outside the valid range.

Checker output:

```text
GOT2 RIGHT: invalid next state
OPEN: unreachable
```

This is a problem because the lock could try to use a state that does not exist, and the correct code cannot open the door.

## Test 3: Unreachable States

I changed `LOCKED` and `E_RIGHT` so the next state is still `LOCKED`.

Checker output:

```text
LOCKED: no way out
GOT1: unreachable
GOT2: unreachable
OPEN: unreachable
```

This is a problem because the lock cannot move forward, even when the correct digit is entered.

## Additional Test: No Way Out

I changed all next states from `GOT1` to `GOT1`.

Checker output:

```text
GOT1: no way out
GOT2: unreachable
OPEN: unreachable
```

This is a problem because the lock gets stuck after entering the first correct digit.

## How to Run the Bonus

From the `hw05` folder, run:

```bash
make bonus
```

## Test Results

```text
Bonus summary: 5 passed, 0 failed
```

The correct table passed, and the checker found problems in the broken tables.