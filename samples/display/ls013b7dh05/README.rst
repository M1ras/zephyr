.. _ls013b7dh05-sample:

LS013B7DH05 Display Driver
##########################

Overview
********

This sample draws a small (8x4) smiley face at the center of the display.

Requirements
************

This sample requires the following hardware:

- `Nordic nRF52840`_
- `Sharp LS013B7DH05`_

This sample has been tested with the following hardware:

- `makerdiary nRF52840-MDK IoT Development Kit`_
- `Adafruit SHARP Memory Display Breakout (1.3" 168x144 Monochrome)`_

Wiring
******

The Nordic nRF52840 should be connected to the Sharp LS013B7DH05 as follows:

+------------+----------------+---------------+-------------------------+
| | nRF52840 | | nRF52840-MDK | | LS013B7DH05 | | Adafruit SHARP Memory |
| | Pin      | | Pin          | | Pin         | | Display Breakout Pin  |
+============+================+===============+=========================+
| AC17       | P0.21          | SCLK          | CLK                     |
+------------+----------------+---------------+-------------------------+
| AD12       | P0.17          | SI            | DI                      |
+------------+----------------+---------------+-------------------------+
| AC11       | P0.16          | NC            | NC                      |
+------------+----------------+---------------+-------------------------+
| AD10       | P0.15          | SCS           | CS                      |
+------------+----------------+---------------+-------------------------+

Building and Running
********************

This sample should work on any board that has SPI and GPIO. For example, it can
be run on the makerdiary nRF52840-MDK IoT Development Kit as described below:

.. zephyr-app-commands::
   :zephyr-app: samples/display/ls013b7dh05
   :board: nrf52840_mdk
   :goals: build
   :compact:

References
**********

.. _nRF52840 Datasheet: http://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.0.pdf
.. _LS013B7DH05 Datasheet: https://media.digikey.com/pdf/Data%20Sheets/Sharp%20PDFs/LS013B7DH05.pdf
