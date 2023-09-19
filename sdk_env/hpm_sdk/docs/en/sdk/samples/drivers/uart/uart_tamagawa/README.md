# UART tomagawa

## Overview

This example shows UART communicate with tamarawa encoder.
In this example, configure UART sending to be triggered by hardware. UART first writes the command data, which is then saved in the FIFO. After a hardware trigger event occurs, UART will issue the command and receive the encoder's return data. After receiving the specified length of data, an interrupt is generated to complete a communication.

## Port Settings

- Serial port baud rate is set to ``25000000bps``, with ``one stop bit`` and ``no parity bit``
## Board Setting

- connect uart tx/rx pin to tamagawa encoder(Please refer to [Pin Description](lab_board_resource)) 

## Running the example

When the project runs correctly, the serial port terminal will output the information like:
```console
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0xb4 0xbd 0x1c 0x37
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0x50 0xa9 0x78 0xa3
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0x81 0xe5 0x6e 0x28
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0x71 0x26 0x5c 0x29
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0xa4 0x4d 0x39 0xf2
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0xd6 0x3a 0x17 0xd9
uart tamagawa transfer complete.
uart sent cmd: 0x02
uart receive excepted count data: 0x02 0x20 0x43 0x7f 0x03 0x1d
```