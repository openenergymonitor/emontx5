The emonTx5 firmware is available in the combined emonTx4, emonTx5 and emonPi2 firmware repository that can be found here:

[https://github.com/openenergymonitor/avrdb_firmware/](https://github.com/openenergymonitor/avrdb_firmware/).

Compile these firmwares with `#define EMONTX5` towards the top of the firmware file.

See other compile and serial config options for further configuration options.

Note: This change streamlines firmware development across all three hardware variants, aiming to simplify support and ongoing maintenance by reducing a growing list of similar firmwares. This change consolidates approximately 18 firmware versions across the three hardware variants into three main firmwares, with #define configuration options and several bug fixes in the serial configuration to provide the same feature sets.

