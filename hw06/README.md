# ECE 361 – Homework 6

## About This Homework

This homework uses the iom361 library to simulate sensors, switches, and LEDs.

The program reads temperature and humidity, checks switch presses, and toggles LEDs.

## Files

- `sensor.c` and `sensor.h` – Read temperature and humidity.
- `panel.c` and `panel.h` – Handle switches and LEDs.
- `scenario.c` and `scenario.h` – Read scenario files.
- `main.c` – Run the simulation and print results.
- `iom361_r4.c` and `iom361_r4.h` – Simulate the hardware.
- `tests/test_hw06.c` – Test the functions.
- `regress.sh` – Compare program output with expected results.

## How to Build

Run:

```bash
make
```

## How to Run

Run:

```bash
./replay scenarios/test1.txt
```

## How to Test

Run:

```bash
make test
```

This runs the unit tests and regression tests.

## How to Clean

Run:

```bash
make clean
```

## Test Results

All unit tests passed.

Both regression tests passed.