# sony_dvs_hdsp_test
This is a test for driving the HDSP display modules found in the DVS

The `HDSP2003` chips have 4 digits each, and are all hooked to a serial bus.

Firstly, `7 * 4 * [display_module_count]` bits of row data are loaded into the modules, then a SIPO is used to scan each column. 

# Setup:
## Pins:
The code is currently set up for use with an Arduino Pro Micro / Leonardo, and the pins/ports reflect this. Other boards are supported but the configuration needs to be adjusted accordingly.

| Wire description | CN1 pin | Arduino Pin | Notes |
| ---------------- | ------- | ----------- | ----- |
| Digit Clock      | 7 | D15 | This pin is marked as PortB's 2nd bit |
| Display Blanking | 8 | A0 |  |
| SIPO Serial In   | 9 | D14 | This signal is used to initiate column scanning |
| SIPO Clock       | 10 | D16 | This signal is used for column scanning |
| Digit Serial In  | 11 | D10 | This pin is marked as PortB's 7nd bit |

## Modules:
This code has been set up for the 3 chip `Numeric keypad` module.

The #define `MODULE_COUNT` could be adjusted to allow for more display modules on the same bus.
