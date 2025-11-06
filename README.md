# QSPI External Memory - MIKROE-3191 Flash 4 Click Driver

## Overview
This project implements a QSPI driver for the MIKROE-3191 (Flash 4 Click) external flash memory module, designed for the Infineon AURIX TC375 Lite Kit.

## Hardware Setup

### Components
- **MCU**: Infineon AURIX TC375 Lite Kit
- **External Flash**: MIKROE-3191 Flash 4 Click (based on Cypress/Infineon S25FL512S)
- **Interface**: QSPI (SPI Mode)

### Pin Connections (MikroBUS on TC375 Lite Kit)
The Flash 4 Click module connects to the TC375 Lite Kit via the MikroBUS connector using QSPI2:

| Flash 4 Click Pin | TC375 Pin | Function | Description |
|------------------|-----------|----------|-------------|
| SCK (MikroBUS) | P15.8 | QSPI2_SCLK | SPI Clock |
| MOSI (MikroBUS) | P15.6 | QSPI2_MTSR | Master Out Slave In |
| MISO (MikroBUS) | P15.7 | QSPI2_MRST | Master In Slave Out |
| CS (MikroBUS) | P15.1 | QSPI2_SLSO8 | Chip Select |
| RST (MikroBUS) | - | GPIO (Optional) | Reset (Not used in current implementation) |
| PWM (IO2) | - | GPIO (Optional) | Not used |
| INT (IO3) | - | GPIO (Optional) | Not used |

### Hardware Notes
1. The Flash 4 Click module should be inserted into the MikroBUS socket on the TC375 Lite Kit
2. Ensure proper 3.3V power supply connection
3. The module uses SPI Mode 0 (CPOL=0, CPHA=0)
4. Maximum SPI clock frequency: 50 MHz (currently configured at 1 MHz for reliability)

## Software Architecture

### File Structure
```
QSPI_Externalmemory/
├── Flash4_Driver.h          # Flash driver header file
├── Flash4_Driver.c          # Flash driver implementation
├── Cpu0_Main.c              # Main application with test code
├── README.md                # This file
└── Libraries/               # iLLD libraries (provided by Infineon)
```

### Key Features
- Full QSPI interface implementation using Infineon iLLD
- Support for 4-byte addressing (32-bit address space)
- Page program operation (up to 256 bytes per page)
- Sector erase operation
- Status register polling for operation completion
- Device ID reading for hardware verification
- Interrupt-driven QSPI communication

### Flash Memory Commands Supported
| Command | Description | Function |
|---------|-------------|----------|
| 0x06 | Write Enable | `Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN)` |
| 0x13 | Read (4-byte address) | `Flash4_ReadFlash4()` |
| 0x12 | Page Program (4-byte) | `Flash4_PageProgram4()` |
| 0xDC | Sector Erase (4-byte) | `Flash4_SectorErase4()` |
| 0x05 | Read Status Register 1 | `Flash4_ReadByte()` |
| 0x90 | Read Manufacturer ID | `Flash4_ReadManufacturerId()` |

## Usage

### Initialization
```c
#include "Flash4_Driver.h"

// Initialize the Flash4 driver
Flash4_Init();

// Wait for initialization to complete
waitMs(100);
```

### Reading Device ID
```c
uint8 deviceId[2];
Flash4_ReadManufacturerId(deviceId);

// deviceId[0] = Manufacturer ID (0x01 for Cypress/Infineon)
// deviceId[1] = Device ID (0x19 for S25FL512S)
```

### Writing Data to Flash
```c
uint8 writeData[16] = "MikroElektronika";
uint32 address = 0x00001234;

// Step 1: Enable write operation
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// Step 2: Erase sector (required before programming)
Flash4_SectorErase4(address);
Flash4_WaitReady(5000);  // Wait up to 5 seconds for erase

// Step 3: Enable write again
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// Step 4: Program page
Flash4_PageProgram4(writeData, address, 16);
Flash4_WaitReady(1000);  // Wait up to 1 second for programming
```

### Reading Data from Flash
```c
uint8 readData[16];
uint32 address = 0x00001234;

// Read data from flash
Flash4_ReadFlash4(readData, address, 16);
Flash4_WaitReady(1000);  // Wait for read to complete
```

### Checking Flash Status
```c
// Check if flash is busy
if(Flash4_CheckWIP())
{
    // Flash is busy, wait
}

// Check if write is enabled
if(Flash4_CheckWEL())
{
    // Write is enabled, proceed with programming
}
```

## Test Application

The included test application (`Cpu0_Main.c`) performs the following operations:

1. **Initialize**: Sets up QSPI interface and LED
2. **Device ID Check**: Reads and verifies manufacturer ID
3. **Erase**: Erases a sector at address 0x00001234
4. **Write**: Writes "MikroElektronika" string to flash
5. **Read**: Reads back the data
6. **Verify**: Compares written and read data
7. **Indicate**: Turns on LED1 if test passes

### LED Indicators
- **LED1 ON**: Test passed - data written and read successfully
- **LED1 OFF**: Test failed or flash not connected

## Memory Specifications (S25FL512S)

- **Capacity**: 512 Mbit (64 MByte)
- **Organization**: 
  - 256 sectors of 256 KByte each
  - Each sector: 256 pages of 1 KByte
  - Each page: 256 bytes
- **Address Range**: 0x00000000 to 0x03FFFFFF
- **Page Program Time**: Typical 170 µs, Max 700 µs
- **Sector Erase Time**: Typical 650 ms, Max 2.6 s

## Configuration

### Adjusting SPI Clock Speed
Edit `Flash4_Driver.h`:
```c
#define FLASH4_QSPI_BAUDRATE    1000000     // 1 MHz (change as needed)
```

### Adjusting Interrupt Priorities
Edit `Flash4_Driver.h`:
```c
#define ISR_PRIORITY_FLASH4_TX  60
#define ISR_PRIORITY_FLASH4_RX  61
#define ISR_PRIORITY_FLASH4_ER  62
```

### Modifying Pin Assignments
If using different pins, edit `Flash4_Driver.c` in the `Flash4_Init()` function:
```c
const IfxQspi_SpiMaster_Pins flash4Pins = {
    &IfxQspi2_SCLK_P15_8_OUT,   // SCLK pin
    &IfxQspi2_MTSR_P15_6_OUT,   // MOSI pin
    &IfxQspi2_MRSTA_P15_7_IN,   // MISO pin
    IfxPort_PadDriver_cmosAutomotiveSpeed3
};
```

## Building and Flashing

### Prerequisites
- AURIX Development Studio (ADS)
- Infineon DAS (Debugger Access Server)
- TC375 Lite Kit with USB connection

### Build Steps
1. Open AURIX Development Studio
2. Import this project
3. Build the project (Project → Build Project)
4. Connect TC375 Lite Kit via USB
5. Debug/Flash: Run → Debug As → AURIX C/C++ Application

## Troubleshooting

### LED Stays OFF
1. **Check Hardware Connections**: Verify Flash 4 Click is properly seated in MikroBUS
2. **Check Power**: Ensure 3.3V is supplied to the module
3. **Check Pin Configuration**: Verify pins match your hardware setup
4. **Device ID Mismatch**: Use debugger to check `g_deviceId[0]` value

### Communication Errors
1. **SPI Clock Too Fast**: Reduce `FLASH4_QSPI_BAUDRATE`
2. **Check Wiring**: Verify SPI connections (SCLK, MOSI, MISO, CS)
3. **Check Pull-ups/Pull-downs**: MISO should have pull-down

### Write/Erase Failures
1. **Check Write Enable**: Always call `Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN)` before erase/program
2. **Timeout**: Increase timeout in `Flash4_WaitReady()` calls
3. **Sector Not Erased**: Flash must be erased (0xFF) before programming

## API Reference

### Initialization Functions
- `void Flash4_Init(void)` - Initialize QSPI interface
- `Flash4_t* Flash4_GetHandle(void)` - Get driver handle

### Command Functions
- `void Flash4_WriteCommand(uint8 cmd)` - Send command to flash

### Identification Functions
- `void Flash4_ReadManufacturerId(uint8 *deviceId)` - Read manufacturer and device ID
- `void Flash4_ReadIdentification(uint8 *outData, uint8 nData)` - Read extended ID
- `uint8 Flash4_ReadElectronicId(void)` - Read electronic signature

### Register Access Functions
- `uint8 Flash4_ReadByte(uint8 reg)` - Read register
- `void Flash4_WriteByte(uint8 reg, uint8 txData)` - Write register

### Memory Access Functions
- `void Flash4_ReadFlash4(uint8 *outData, uint32 addr, uint16 nData)` - Read data
- `void Flash4_PageProgram4(uint8 *inData, uint32 addr, uint16 nData)` - Write data
- `void Flash4_SectorErase4(uint32 addr)` - Erase sector

### Status Functions
- `uint8 Flash4_CheckWIP(void)` - Check if busy (Write In Progress)
- `uint8 Flash4_CheckWEL(void)` - Check if write enabled
- `uint8 Flash4_WaitReady(uint32 timeoutMs)` - Wait for operation completion
- `void Flash4_Reset(void)` - Software reset

## Example Application Code

```c
#include "Flash4_Driver.h"

void example_flash_usage(void)
{
    uint8 writeData[256];
    uint8 readData[256];
    uint32 address = 0x00000000;
    
    // Prepare data
    for(uint16 i = 0; i < 256; i++)
        writeData[i] = i;
    
    // Initialize flash
    Flash4_Init();
    waitMs(100);
    
    // Erase sector
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_SectorErase4(address);
    Flash4_WaitReady(5000);
    
    // Write page
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_PageProgram4(writeData, address, 256);
    Flash4_WaitReady(1000);
    
    // Read back
    Flash4_ReadFlash4(readData, address, 256);
    
    // Verify
    boolean match = TRUE;
    for(uint16 i = 0; i < 256; i++)
    {
        if(readData[i] != writeData[i])
        {
            match = FALSE;
            break;
        }
    }
}
```

## References

- [AURIX TC375 Lite Kit User Manual](https://www.infineon.com/cms/en/product/evaluation-boards/kit_a2g_tc375_lite/)
- [AURIX TC3xx User Manual](https://www.infineon.com/aurix)
- [iLLD Documentation](https://www.infineon.com/cms/en/product/microcontroller/32-bit-tricore-microcontroller/aurix-tc3x/)
- [S25FL512S Datasheet](https://www.infineon.com/cms/en/product/memories/nor-flash/standard-spi-nor-flash/)
- [MIKROE-3191 Flash 4 Click](https://www.mikroe.com/flash-4-click)

## License

This software is provided under the Boost Software License - Version 1.0 - August 17th, 2003.

## Author

Based on Infineon AURIX code examples and MikroElektronika Flash 4 Click driver.
Integrated for TC375 Lite Kit by the AURIX development team.

## Version History

- **v1.0.0** (2024) - Initial release
  - QSPI2 interface implementation
  - Support for read/write/erase operations
  - Example test application
  - Hardware abstraction using iLLD

