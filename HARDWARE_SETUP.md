# Hardware Setup Guide - MIKROE-3191 Flash 4 Click with TC375 Lite Kit

## Table of Contents
1. [Required Components](#required-components)
2. [Hardware Connections](#hardware-connections)
3. [Pin Mapping Details](#pin-mapping-details)
4. [Power Requirements](#power-requirements)
5. [Assembly Instructions](#assembly-instructions)
6. [Verification Steps](#verification-steps)
7. [Common Issues](#common-issues)

## Required Components

### Main Components
- **Infineon AURIX TC375 Lite Kit** (KIT_A2G_TC375_LITE)
- **MIKROE-3191 Flash 4 Click** module
- **USB Cable** (Micro-USB for programming and debugging)

### Optional Components
- Multimeter (for voltage verification)
- Logic analyzer (for SPI signal debugging)
- Oscilloscope (for signal integrity verification)

## Hardware Connections

### MikroBUS Connector Overview
The TC375 Lite Kit features a MikroBUS™ socket that provides a standardized interface for Click board modules. The Flash 4 Click module is designed to be directly compatible with this socket.

### Physical Connection
```
┌─────────────────────────────┐
│   TC375 LITE KIT            │
│                             │
│   ┌─────────────────┐       │
│   │   MikroBUS      │       │
│   │   Socket        │       │
│   └────────┬────────┘       │
│            │                │
│            │ Insert         │
│            ↓                │
│   ┌─────────────────┐       │
│   │ Flash 4 Click   │       │
│   │   MIKROE-3191   │       │
│   └─────────────────┘       │
│                             │
└─────────────────────────────┘
```

## Pin Mapping Details

### MikroBUS Pinout to TC375 Mapping

| MikroBUS Pin # | MikroBUS Function | TC375 Pin | QSPI Function | Signal Description |
|----------------|-------------------|-----------|---------------|-------------------|
| 1 | AN | - | - | Not used |
| 2 | RST | P02.8 | GPIO | Reset (optional) |
| 3 | CS | P15.1 | QSPI2_SLSO8 | Chip Select |
| 4 | SCK | P15.8 | QSPI2_SCLK | SPI Clock |
| 5 | MISO | P15.7 | QSPI2_MRST | Master In Slave Out |
| 6 | MOSI | P15.6 | QSPI2_MTSR | Master Out Slave In |
| 7 | +3.3V | VCC | Power | Power Supply |
| 8 | GND | GND | Ground | Ground |
| 9 | PWM | - | GPIO | IO2 (not used) |
| 10 | INT | - | GPIO | IO3 (not used) |

### Detailed Pin Description

#### Power Pins
- **Pin 7 (3.3V)**: Main power supply for the Flash 4 Click module
  - Voltage: 3.3V ±5%
  - Maximum current: ~30mA during active operations
  - Typical current: ~15mA during read/write
  - Standby current: ~10µA

- **Pin 8 (GND)**: Ground reference
  - Must have solid connection for proper operation
  - Shared with TC375 ground plane

#### SPI Communication Pins
- **Pin 3 (CS/QSPI2_SLSO8)**: Chip Select
  - Active LOW
  - TC375 Pin: P15.1
  - Controlled by QSPI2 peripheral
  - Pull-up to 3.3V on Flash 4 Click module

- **Pin 4 (SCK/QSPI2_SCLK)**: SPI Clock
  - TC375 Pin: P15.8
  - Output from TC375 to Flash module
  - Frequency: Configurable (default 1 MHz, max 50 MHz)
  - Idle state: LOW (SPI Mode 0)

- **Pin 5 (MISO/QSPI2_MRST)**: Master In Slave Out
  - TC375 Pin: P15.7
  - Input to TC375 from Flash module
  - Data sampled on rising edge of SCK
  - Internal pull-down resistor enabled

- **Pin 6 (MOSI/QSPI2_MTSR)**: Master Out Slave In
  - TC375 Pin: P15.6
  - Output from TC375 to Flash module
  - Data changes on falling edge of SCK

#### Optional GPIO Pins
- **Pin 2 (RST)**: Hardware Reset
  - Not currently used in software
  - Can be used for hardware reset functionality
  - Active LOW
  - Internal pull-up on Flash 4 Click

- **Pin 9 (PWM/IO2)**: Additional I/O
  - Connected to WP# (Write Protect) on flash chip
  - Pulled HIGH on module

- **Pin 10 (INT/IO3)**: Additional I/O
  - Connected to HOLD#/IO3 on flash chip
  - Pulled HIGH on module

## Power Requirements

### Voltage Specifications
- **Operating Voltage**: 3.3V ±5% (3.135V to 3.465V)
- **Logic High (VIH)**: 0.7 × VCC = 2.31V minimum
- **Logic Low (VIL)**: 0.3 × VCC = 0.99V maximum

### Current Consumption
| Operation Mode | Typical | Maximum |
|----------------|---------|---------|
| Standby | 10 µA | 20 µA |
| Read | 15 mA | 25 mA |
| Program | 20 mA | 30 mA |
| Erase | 25 mA | 35 mA |

### Power-Up Sequence
1. Apply 3.3V power
2. Wait for VCC to stabilize (minimum 100µs)
3. Release reset (if used)
4. Wait for internal oscillator to stabilize (~1ms)
5. Device ready for commands

## Assembly Instructions

### Step-by-Step Installation

#### Step 1: Power Down
1. Disconnect USB cable from TC375 Lite Kit
2. Ensure all power sources are disconnected
3. Wait 10 seconds for capacitors to discharge

#### Step 2: Visual Inspection
1. Inspect the MikroBUS socket on TC375 Lite Kit
   - Check for bent pins
   - Look for any debris or contamination
   
2. Inspect the Flash 4 Click module
   - Verify all pins are straight
   - Check for any physical damage
   - Ensure the flash chip is properly soldered

#### Step 3: Module Installation
1. Orient the Flash 4 Click module correctly:
   - The notch on the module should match the notch on the socket
   - Pin 1 indicator should align
   - Component side should face up

2. Carefully align the module pins with the socket

3. Gently but firmly press the module into the socket
   - Apply even pressure
   - Ensure all pins are fully seated
   - The module should sit flush with the socket

#### Step 4: Visual Verification
1. Inspect the installation:
   - All pins should be inserted fully
   - No pins should be bent or misaligned
   - Module should be level (not tilted)

2. Check for short circuits (with multimeter if available):
   - Measure resistance between VCC and GND (should be > 1kΩ)
   - If short circuit detected, remove module and inspect

#### Step 5: Power-Up Test
1. Reconnect USB cable to TC375 Lite Kit
2. Power on the board
3. Check LED indicators on TC375 Lite Kit (power LED should be on)
4. No smoke or unusual smells should be present

## Verification Steps

### Hardware Verification Checklist

#### Visual Inspection
- [ ] Flash 4 Click module is properly seated in MikroBUS socket
- [ ] All pins are fully inserted
- [ ] No bent or damaged pins
- [ ] Module is level and secure
- [ ] No visible short circuits

#### Electrical Verification (with Multimeter)
- [ ] VCC pin measures 3.3V ±5%
- [ ] GND pin measures 0V
- [ ] No short circuit between VCC and GND
- [ ] SPI pins show valid logic levels (not floating)

#### Software Verification
Run the included test program to verify:
- [ ] Device ID reads correctly (Manufacturer ID = 0x01)
- [ ] Write operation completes without errors
- [ ] Read operation returns written data
- [ ] LED1 turns ON (indicating successful test)

### Using Debugger for Verification
1. Open AURIX Development Studio
2. Build and flash the test program
3. Set breakpoint in `testFlash4()` function
4. Run in debug mode
5. Check variable values:
   - `g_deviceId[0]` should be `0x01`
   - `g_deviceId[1]` should be `0x19` (for S25FL512S)
   - `g_testPassed` should be `TRUE`

## Common Issues

### Issue 1: Device ID Read Fails
**Symptoms**: `g_deviceId[0]` reads as `0x00` or `0xFF`

**Possible Causes**:
1. Module not properly seated
2. Power supply issue
3. SPI clock too fast
4. Pin configuration mismatch

**Solutions**:
- Remove and re-insert module
- Check 3.3V power supply
- Reduce SPI clock to 100kHz for testing
- Verify pin assignments in `Flash4_Driver.c`

### Issue 2: LED1 Does Not Turn On
**Symptoms**: Test completes but LED remains off

**Possible Causes**:
1. Write/erase operation failed
2. Data verification failed
3. Timeout during operations

**Solutions**:
- Use debugger to check `g_testPassed` variable
- Increase timeout values in code
- Check `g_writeBuffer` and `g_readBuffer` contents
- Verify flash is properly erased before writing

### Issue 3: Communication Timeouts
**Symptoms**: Operations time out, `Flash4_WaitReady()` returns timeout error

**Possible Causes**:
1. Flash chip not responding
2. WIP (Write In Progress) bit stuck
3. Insufficient timeout duration

**Solutions**:
- Perform software reset: `Flash4_Reset()`
- Increase timeout values
- Check power supply stability
- Verify SPI signals with logic analyzer

### Issue 4: SPI Communication Errors
**Symptoms**: Garbled data, incorrect read values

**Possible Causes**:
1. SPI clock too fast for board layout
2. Signal integrity issues
3. Ground connection problems
4. Electromagnetic interference

**Solutions**:
- Reduce SPI clock frequency
- Check ground connections
- Add small capacitor (0.1µF) near module power pins
- Keep wires short and avoid running near noisy signals

### Issue 5: Inconsistent Behavior
**Symptoms**: Sometimes works, sometimes fails

**Possible Causes**:
1. Loose connection
2. Power supply noise
3. Temperature issues
4. Bad solder joints on module

**Solutions**:
- Re-seat module firmly
- Add decoupling capacitors
- Check ambient temperature (0°C to +70°C operating range)
- Inspect module for physical damage

## Testing with Logic Analyzer

### Recommended Setup
- **Probe Points**: CS, SCK, MOSI, MISO
- **Sample Rate**: Minimum 10 MSamples/s
- **Trigger**: Falling edge of CS
- **Protocol Decoder**: SPI Mode 0

### Expected Waveforms
1. **Read ID Command (0x90)**:
   ```
   CS:   ‾\_______________________/‾
   SCK:  __/‾\_/‾\_/‾\_/‾\_/‾\_/‾\___
   MOSI: --<0x90><0x00><0x00><0x00>
   MISO: ------<0x01><0x19>---------
   ```

2. **Write Enable Command (0x06)**:
   ```
   CS:   ‾\___/‾
   SCK:  __/‾\___
   MOSI: --<0x06>
   ```

## Safety Precautions

### Electrical Safety
- Always disconnect power before inserting/removing modules
- Do not hot-swap modules
- Ensure proper ESD precautions
- Use appropriate voltage levels (3.3V only)

### Handling Precautions
- Use ESD wrist strap when handling boards
- Hold boards by edges only
- Avoid touching components or pins
- Store in anti-static bags when not in use

## Support Resources

### Documentation
- TC375 Lite Kit User Manual
- AURIX TC375 User Manual
- MIKROE-3191 Datasheet
- S25FL512S Flash Datasheet

### Online Resources
- Infineon Developer Community
- AURIX Support Forum
- MikroElektronika LibStock

### Contact Information
- Infineon Support: www.infineon.com/support
- MikroElektronika Support: www.mikroe.com/support

## Revision History
- **v1.0** (2024): Initial release

