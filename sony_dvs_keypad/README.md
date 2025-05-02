# sony_dvs_keypad
This is a test for reading the keyboard matrix found in the DVS

Most inputs are latched through a flip-flop

Addressing the LED matrix happens on the same bus, so support could be added as part of the same sketch

# Setup:
## Pins:
The code is currently set up for use with an Arduino Pro Micro / Leonardo, and the pins/ports reflect this. Other boards are supported but the configuration needs to be adjusted accordingly.

| Wire description | CN1 pin | Arduino Pin | Notes |
| ---------------- | ------- | ----------- | ----- |
| Clock | 1 | D2 | This pin is marked as PortD's 2nd bit |
| Row Data | 3 | D3 |  |
| Mode Select | 4 | D4 |  |
| PISO Data | 6 | D5 | This pin is marked as PortC's 7th bit |

## Modules:
This code has been set up for 2 modules, a `Key frame control` module followed by a `Numeric keypad` module.

The #defines and receiving struct could be adjusted to allow for more modules.