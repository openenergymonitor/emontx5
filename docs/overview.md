---
github_url: "https://github.com/openenergymonitor/emontx5/blob/main/docs/overview.md"
---

# Overview

The emonTx5 builds directly on recent emonTx4 and emonPi2 development and replaces the emonTx4 as our latest energy monitoring transmitter node. The emonTx5 transmits data via an inbuilt 433MHz radio to an emonPi or emonBase. It is useful for expanding an emonPi2 monitoring setup where more CT inputs are required either in the same location or in a different part of the building.

Like the emonTx4 and emonPi2, the emonTx5 uses the same focus on higher accuracy, more CT channels, precision voltage sensing and the new microcontroller core. When combined with the emonVS voltage sensor, a selection of CT sensors and an emonPi2 or emonBase base station, it provides an end-to-end electricity monitoring solution perfect for monitoring home consumption, solar generation, EV charging, heat pumps, battery storage among other applications.

!\[emonTx5_complete_kit.jpg](img/emonTx5_complete_kit.jpg)

## Key features

- 6x clip-on CT current sensor inputs (suitable for a range of 333mV voltage output CT sensors).
- emonVs precision voltage sensor input.
- Full Real/Active power measurement & continuous sampling.
- Cumulative energy persisted on reboot.
- Both single and 3 phase support.
- 3 pluggable terminal block inputs for DS18B20 temperature sensing*, pulse counting or an analogue voltage input.
- On-board USB to UART converter for easier programming and serial output.
- 433 MHz radio transceiver
- Wall-mount aluminium enclosure

\*Temperature sensing is only available when using the single phase *emonTx5_CM_6CT_temperature* firmware based on the emonLibCM library. There is a slight impact on electricity monitoring accuracy when using temperature sensing, please see related forum post for the emonTx4 [here](https://community.openenergymonitor.org/t/emontx4-ds18b20-temperature-sensing-firmware-release-1-5-7/23496).

## Technical spec

- Microcontroller: [Microchip AVR128DB48](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/8-bit-mcus/avr-mcus/avr-db)
- Arduino core support [DxCore by SpenceKonde](https://github.com/SpenceKonde/DxCore)
- ADC Resolution: 12-bit
- Precision voltage reference: [MCP1502](https://www.microchip.com/en-us/product/MCP1502)
- On board USB to UART converter: CP2102
- RFM69cw 433 MHz radio

## Accuracy

The emonTx5 includes the same accuracy improvements included in the emonTx4. The largest source of improvement comes from: a higher accuracy voltage sensor, higher accuracy CT current sensors and a wider range of CT sensors with current ratings that can better suit the load being monitored (making sure the analog-to-digital range of the microcontroller is being put to full use).

The key sensor and component tolerances are now:

- CT Sensors: ±0.5%
- EmonVs voltage sensor: ±0.5%
- Analog voltage reference ±0.2%
- Combined: ±1.2%

We have also upgraded the microcontroller from the ATmega328 to and AVR128DB48. The main improvement is a higher resolution ADC at 12-bits (4x the resolution of the ATmega328) alongside the capability to monitor more channels.
