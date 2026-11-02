# Wine Cellar Monitor – SPEC

## 1. Project Description

My project is a Wine Cellar Monitor for ECE 361.

The device checks temperature and humidity to keep the wine cellar in good condition.

It uses the iom361 library to simulate sensors, switches, and LEDs. The program reads the sensor values, changes states, saves records, and creates reports.

## 2. Device States

The device has four states:

- **STABLE:** The device is not cooling or humidifying.
- **COOLING:** The cooler is running.
- **HUMIDIFY:** The humidifier is running.
- **ALARM:** The temperature is outside the safe range.

The device starts in STABLE.

## 3. Events

The state machine uses these events:

| Event | Meaning |
|---|---|
| EV_NONE | No change is needed |
| EV_TOO_HOT | Temperature is above 14.0°C |
| EV_COOL_DONE | Cooling reaches 12.5°C or lower |
| EV_TOO_DRY | Humidity is below 60% |
| EV_HUMID_DONE | Humidity reaches 68% or higher |
| EV_ALARM | Temperature is above 18.0°C or below 8.0°C |
| EV_ACK | ACK is pressed while the alarm temperature is safe |

ACK is a press event. It only counts when the switch changes from 0 to 1.

The function `fsm_get_event()` chooses an event using the reading and ACK input. The function `fsm_next()` uses the transition table to find the next state.

## 4. State Transition Table

The table uses the current state and event to choose the next state.

| Current State | Event | Next State | Action |
|---|---|---|---|
| STABLE | EV_NONE | STABLE | Keep outputs off |
| STABLE | EV_TOO_HOT | COOLING | Turn on cooler |
| STABLE | EV_TOO_DRY | HUMIDIFY | Turn on humidifier |
| STABLE | EV_ALARM | ALARM | Turn on alarm |
| COOLING | EV_NONE | COOLING | Keep cooler on |
| COOLING | EV_TOO_HOT | COOLING | Keep cooler on |
| COOLING | EV_COOL_DONE | STABLE | Turn off cooler |
| COOLING | EV_TOO_DRY | COOLING | Keep cooler on |
| COOLING | EV_ALARM | ALARM | Stop cooler and turn on alarm |
| HUMIDIFY | EV_NONE | HUMIDIFY | Keep humidifier on |
| HUMIDIFY | EV_TOO_DRY | HUMIDIFY | Keep humidifier on |
| HUMIDIFY | EV_HUMID_DONE | STABLE | Turn off humidifier |
| HUMIDIFY | EV_TOO_HOT | COOLING | Stop humidifier and start cooler |
| HUMIDIFY | EV_ALARM | ALARM | Stop humidifier and turn on alarm |
| ALARM | EV_NONE | ALARM | Keep alarm on |
| ALARM | EV_ALARM | ALARM | Keep alarm on |
| ALARM | EV_ACK | STABLE | Turn off alarm |

All other state-event combinations keep their current state in the present transition table.

The event-detection code prevents `EV_ACK` from being sent when the temperature is outside the safe range.


The program chooses one event each tick.

The transition table stores both the next state
and the action.

The main program uses the new state to choose
the LEDs, RGB color, and status word.

The action table is also checked by unit tests.

## 5. State Machine Diagram

```text
                  Temperature > 14.0 C
       +----------------------------------+
       |                                  v
    STABLE                             COOLING
       ^                                  |
       +----------------------------------+
                 Temperature <= 12.5 C

                  Humidity < 60%
       +----------------------------------+
       |                                  v
    STABLE                             HUMIDIFY
       ^                                  |
       +----------------------------------+
                  Humidity >= 68%

    HUMIDIFY ---- Temperature > 14.0 C ----> COOLING

    STABLE --------+
    COOLING -------+---- Unsafe temperature ----> ALARM
    HUMIDIFY ------+

    ALARM ---- Safe temperature + ACK ----> STABLE
```

## 6. Operating Rules and Design Decisions

Cooling starts when the temperature is above 14.0°C and continues until it reaches 12.5°C or lower.

Humidifying starts when humidity is below 60% and continues until it reaches 68% or higher.

Cooling has priority when both temperature and humidity need control. The cooler and humidifier do not run together.

When cooling stops, the program checks whether humidifying is needed on the next tick.

If the temperature goes above 18.0°C or below 8.0°C, the device enters ALARM.

The alarm stays active until the temperature returns to the safe range and ACK is pressed.

ACK cannot clear an alarm while the temperature is unsafe.

When an alarm is cleared, the state becomes STABLE. Other control needs are checked again on the next tick.

The event-detection logic is in `fsm.c`. This keeps sensor decisions close to the state-machine rules. The main program handles hardware I/O and scenario replay.

## 7. LEDs and RGB Colors

The device uses three normal LEDs.

| LED bit | Name | Meaning |
|---|---|---|
| 0 | COOLER | Cooler is on |
| 1 | HUMIDIFIER | Humidifier is on |
| 2 | ALARM_LAMP | Alarm is on |

The RGB LED shows a different color for each state.

| State | RGB color |
|---|---|
| STABLE | Green |
| COOLING | Blue |
| HUMIDIFY | Cyan |
| ALARM | Red |

Only the LED for the active function is on. All three normal LEDs are off in STABLE.

## 8. Sensor Readings

The `sensor.c` module reads temperature and humidity from the iom361 registers.

Each reading is stored using `reading_t`, which contains:

- Timestamp (`time_t`)
- Temperature (`float`)
- Humidity (`float`)

Temperature conversion:

`Temperature = (raw_temp / 1048576.0) × 200.0 - 50.0`

Humidity conversion:

`Humidity = (raw_humid / 1048576.0) × 100.0`

The results are rounded to one decimal place.

The sensor function returns 0 when successful and -1 when it fails.

The sensor module does not print anything.

## 9. Time and Clock

The device uses `time_t` to store timestamps.

The scenario file gives the starting Unix timestamp. The program then advances one second for each tick.

The timestamp of a reading is:

`timestamp = start_time + tick`

All displayed times are UTC.

The program uses `gmtime()` and `strftime()` to print the date and time.

For example:

`2026-10-06 08:00:05`

The first reading is at tick 0.

## 10. Status Word Layout

The status word uses one `uint32_t` value.

| Bits | Meaning |
|---|---|
| 0–1 | Current state |
| 2 | COOLER |
| 3 | HUMIDIFIER |
| 4 | ALARM_LAMP |
| 5 | Alarm not acknowledged |
| 6–31 | Unused, always 0 |

State values:

- STABLE = 0
- COOLING = 1
- HUMIDIFY = 2
- ALARM = 3

The current program packs the status fields using `set_field()`.

The unused bits stay at zero.

The status word records the device state and output information for each reading.

## 11. Binary Search Tree (BST)

The device uses a binary search tree to store sensor records.

Each `record_t` contains:

- A sensor reading
- The state
- The status word

The BST is hidden inside `store.c` using an opaque `store_t` type.

Records are ordered by temperature using 0.5°C steps.

The temperature key is calculated using:

`key = floor(temperature × 2)`

For example:

- 13.0°C has key 26.
- 13.2°C has key 26.
- 13.5°C has key 27.

For duplicate keys, the tree keeps a linked list of records in that node.

This means no records are lost when different temperatures have the same key.

The BST supports creating, inserting, finding, walking, counting, measuring height, and destroying the tree.

`store_find()` returns the first record with the matching key. `store_walk()` visits every stored record, including duplicate keys.

An in-order walk visits the temperature keys from smallest to largest.

I chose a linked list for duplicate records because it keeps the code simple and preserves every reading.

## 12. Hash Table with Separate Chaining

The device uses a hash table with exactly 3 buckets.

The hash function is:

`hash(state) = state % 3`

The states use these numbers:

- STABLE = 0
- COOLING = 1
- HUMIDIFY = 2
- ALARM = 3

The buckets are:

| Bucket | States |
|---|---|
| 0 | STABLE and ALARM |
| 1 | COOLING |
| 2 | HUMIDIFY |

STABLE and ALARM share bucket 0. This is a collision.

I use separate chaining, which means each bucket holds a linked list.

Each entry stores the state, how many readings were recorded in that state, and the total ticks spent in that state.

For each simulated second, the main program updates the entry for the current state with one tick.

The structure is hidden inside `aux.c`.


I tested the hash table using the every_state
scenario.

The results were:

- STABLE: 17 ticks
- COOLING: 4 ticks
- HUMIDIFY: 4 ticks
- ALARM: 4 ticks

Total = 29 ticks.

The total matches the number of readings
in the scenario.


## 13. Hash Table Comparison

I used separate chaining instead of linear probing.

Separate chaining stores entries in linked lists, so two states can share a bucket. Linear probing looks for another empty position in the table when a collision happens.

For a general hash table with a good hash function and a low load factor, insertion and lookup are usually O(1) on average for both methods. In the worst case, separate chaining can take O(n) when many keys are in one chain. Linear probing can also take O(n) when it must search through many positions.

Separate chaining is a good choice for this project because there are four states but only three buckets, so a collision is required. It is also simple to update an existing state entry.

## 14. Report Q6

The report finds the first and last reading when the temperature is strictly above 14.0°C.

It also counts the total number of readings that meet the condition.

The condition is:

`temperature > 14.0`

A reading at exactly 14.0°C is not counted.

The report uses `store_walk()` through `store.h`. It does not directly access the BST structure.

Because the tree is ordered by temperature rather than time, the report compares all matching timestamps to find the earliest and latest occurrences.

The report prints the first and last ticks relative to the scenario starting time.

If no readings meet the condition, the report prints:

`No readings above 14.0 C.`

For example, if the condition occurs from ticks 5–9 and 20–24, the first tick is 5, the last tick is 24, and the total count is 10.

## 15. Scenario Files

The program supports two modes:

`./cellar scenarios/normal.txt`

This prints one trace line per tick.

`./cellar scenarios/normal.txt --report`

This prints only the temperature report.

Scenario files contain a starting Unix timestamp and changes to the simulated temperature, humidity, and switches.

Between scenario lines, the values stay the same.

The program uses `_iom361_set_sensor1()` and `_iom361_set_switches()` to simulate the environment.

The sensor and FSM modules do not call these simulation functions.


## 16. Tests

I created unit tests for the BST, FSM, sensor,
hash table, report, and status word.

I also created eight scenarios:

1. Normal operation
2. Every state twice
3. Exact thresholds
4. Alarm and recovery
5. Unexpected switch inputs
6. Report with at least 30 readings
7. Midnight date change
8. February 28 to March 1, 2027

The tests check state changes, temperature
boundaries, duplicate BST records, hash
collisions, sensor conversions, status bits,
report results, and scenario output.

The hash table also checks that the total
number of ticks matches the scenario length.

The project builds and runs on Linux.

The latest make test result was:

Tests passed: 14, failed: 0

I also tested the project with AddressSanitizer
and UndefinedBehaviorSanitizer.


## 17. Year 2038 Problem

If `time_t` uses a signed 32-bit integer, it will run out of space on January 19, 2038.

The timestamp could overflow and become negative.

This could make the device show an incorrect date and time. It could also cause problems with reports and stored readings.

Using a 64-bit `time_t` avoids this particular limit.

## 18. Evaluation of the Given States

I think the four states are a good choice for this device because each state has a clear purpose. STABLE means nothing is running, COOLING controls high temperature, HUMIDIFY controls low humidity, and ALARM warns about unsafe temperature. These states make the program easier to understand and test. One limitation is that the device cannot cool and humidify at the same time. However, this is acceptable because the project requires cooling to have priority.

## 19. Program Organization

I divided the program into modules to keep the code organized.

- `sensor.c` reads temperature and humidity.
- `fsm.c` decides the next state.
- `store.c` saves records in a BST.
- `aux.c` tracks state counts and total ticks.
- `report.c` creates the temperature report.
- `main.c` connects all modules and runs the scenario.
- `Makefile` builds the program and runs tests.

This makes the program easier to understand, test, and update.
