# ECE 361 - Homework 2

## About the Program

This program reads sensor data and prints a summary.

It shows the number of readings, skipped lines, minimum, maximum, average, longest run above a temperature threshold, and a histogram.

The original program was split into smaller modules to make the code easier to read and understand.

## Modules

- **main.c:** Controls the program and calls the other modules.
- **reading.c:** Reads sensor data and skips invalid lines.
- **stats.c:** Calculates the minimum, maximum, mean, and longest run.
- **histogram.c:** Prints a histogram of temperatures.

## Module Diagram

```text
              +-----------+
              |  main.c   |
              +-----------+
               /    |    \
              v     v     v
       +---------+ +---------+ +------------+
       | reading | |  stats  | | histogram  |
       +---------+ +---------+ +------------+
```

`main.c` calls the reading, stats, and histogram modules. These three modules do not call each other.

## How to Build

Open the terminal inside the `hw02` folder and run:

`make`

This builds the program named `readings`.

## How to Run

Run the program using normal sensor data:

`./readings < data/normal.txt`

Run the program with a temperature threshold of 25°C:

`./readings 25 < data/edge.txt`

## How to Test

Run:

`make test`

This runs the statistics tests and regression tests.

## How to Clean

Run:

`make clean`

This removes the compiled files.

## Design

The program does not use global variables. The arrays are stored inside `main()` and passed to other functions.

The maximum function uses recursion, and the same statistics functions work for temperature and humidity.
