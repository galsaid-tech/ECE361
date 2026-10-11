# Wine Cellar Monitor - SPEC

## 1. Project

My project is a Wine Cellar Monitor.

It checks the temperature and humidity to keep the wine cellar in good condition.

## 2. Device States

The device has four states:

- **STABLE:** The temperature and humidity are okay.
- **COOLING:** The cooler is on because the temperature is too high.
- **HUMIDIFY:** The humidifier is on because humidity is too low.
- **ALARM:** The temperature is outside the safe range.

The device starts in STABLE.

## 3. Events

Events are things that can make the device change states.

- Temperature goes above 14.0°C.
- Temperature reaches 12.5°C or lower.
- Humidity goes below 60%.
- Humidity reaches 68% or higher.
- Temperature goes above 18.0°C or below 8.0°C.
- The user presses the ACK switch.
- Temperature returns to the safe range.

## 4. State Changes

| Current State | Condition                               | Next State |
|---------------|-----------------------------------------|------------|
| STABLE        | Temperature above 14.0°C                | COOLING    |
| STABLE        | Humidity below 60%                      | HUMIDIFY   |
| COOLING       | Temperature 12.5°C or lower             | STABLE     |
| HUMIDIFY      | Humidity 68% or higher                  | STABLE     |
| Any state     | Temperature above 18.0°C or below 8.0°C | ALARM      |
| ALARM         | ACK pressed and temperature safe        | STABLE     |
 ----------------------------------------------------------------------
Cooling has priority over humidifying.

## 5. Outputs

- Cooling LED shows when cooling is on.
- Humidity LED shows when humidifying is on.
- Alarm LED shows when the alarm is on.

## 6. Future Work

This is the starting plan for the project.

Later, I will add the code for reading sensors, changing states, saving data, and printing reports.


## 7. RGB LED Colors

Each state has a different RGB LED color.

- STABLE: Green
- COOLING: Blue
- HUMIDIFY: Yellow
- ALARM: Red


## 8. State Machine Diagram

```text
                 Temperature > 14.0 C
         +------------------------------+
         |                              v
      STABLE                         COOLING
         ^                              |
         +------------------------------+
                Temperature <= 12.5 C

                 Humidity < 60%
         +------------------------------+
         |                              v
      STABLE                         HUMIDIFY
         ^                              |
         +------------------------------+
                 Humidity >= 68%

Any State --> ALARM
  When temperature > 18.0 C or < 8.0 C

ALARM --> STABLE
  When ACK is pressed and temperature is safe
```


## 9. Status Word Layout

The status word uses 32 bits.

|Bits| Meaning                |
|----|------------------------|
|0-1 | Current state          |
| 2  | COOLER                 |
| 3  | HUMIDIFIER             |
| 4  | ALARM_LAMP             |
| 5  | Alarm not acknowledged |
|6-31| Unused (set to 0)      |

The state values are:

- STABLE = 0
- COOLING = 1
- HUMIDIFY = 2
- ALARM = 3

The status word will use get_field and set_field from Homework 1.


## 10. Cooling Priority

Cooling has priority over humidifying.

If the temperature is above 14.0 C and humidity is below 60%, the device will start cooling first.

The cooler will stay on until the temperature reaches 12.5 C or lower.

After cooling stops, if humidity is still below 60%, the device will start humidifying.

The cooler and humidifier will not run at the same time.


## 11. Alarm Rules

The device enters ALARM if the temperature is above 18.0 C or below 8.0 C.

While ALARM is active:
- The alarm LED is on.
- The cooler and humidifier are off.
- The RGB LED is red.
- Pressing ACK while the temperature is unsafe does nothing.

If the temperature becomes safe, the alarm stays on until ACK is pressed.

When ACK is pressed and the temperature is safe, the device returns to STABLE.

After that, the device checks if cooling or humidifying is needed.

ACK only counts when the switch changes from off to on.


## 12. Year 2038 Problem

If time_t uses a signed 32-bit number, it will run out of space on January 19, 2038.

The time value may become negative, and the device could show the wrong date and time.

This could cause problems with the timestamps and reports.

Using a 64-bit time_t can prevent this problem.


## 13. Storing Sensor Readings

The device will use a Binary Search Tree (BST) to store sensor readings.

Each reading will have:
- Temperature
- Humidity
- Time

The readings will be organized by temperature.

The BST will help the program:
- Add new readings.
- Find saved readings.
- Remove readings.
- Find readings above 14.0 C for the report.

The store module will handle these tasks.


## 14. Hash Table

The device will use a hash table with 3 buckets.

The hash table will track:
- How many times each state appears.
- How much time the device spends in each state.

It will use separate chaining to handle collisions.

The aux module will handle the hash table.
