# ECE 361 Homework 1

## Description

This homework implements a small C library for bit manipulation and decoding a 16-bit thermostat status word.

The bit library contains four functions:

- print_binary
- get_field
- set_field
- sign_extend

The thermostat library uses these functions to unpack fields from a 16-bit thermostat status word.

## Build

Run:

make

## Run Tests

Run:

make test

## Clean

Run:

make clean

## Valid Input Ranges

For the bit functions:

- width must be between 1 and 32.
- pos must be between 0 and 31.
- pos + width must not be greater than 32.

## Out-of-Range Behavior

For get_field, invalid arguments return 0.

For set_field, invalid arguments return the original word unchanged.

For print_binary, an invalid width causes the function to return without printing.

For sign_extend, an invalid width returns 0.

## Width 32

A width of 32 is handled separately because shifting a 32-bit integer by 32 bits is not valid in C.

## Invalid Thermostat Modes

Valid thermostat modes are 0 through 4.

Modes 5, 6, and 7 are invalid.

status_unpack leaves the mode value unchanged, so an invalid mode is returned as 5, 6, or 7.