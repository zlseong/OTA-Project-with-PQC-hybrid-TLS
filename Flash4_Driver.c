#include "Flash4_Driver.h"
#include "Flash4_Config.h"
#include "IfxQspi_SpiMaster.h"
#include "IfxQspi_PinMap.h"
#include "IfxPort.h"
#include "IfxStm.h"
#include "IfxCpu_Irq.h"

// Global QSPI Master handle and channel
IfxQspi_SpiMaster g_qspiFlash4;
IfxQspi_SpiMaster_Channel g_qspiFlash4Channel;

// ISRs for QSPI Master
IFX_INTERRUPT(qspiFlash4TxISR, 0, ISR_PRIORITY_FLASH4_TX);
IFX_INTERRUPT(qspiFlash4RxISR, 0, ISR_PRIORITY_FLASH4_RX);
IFX_INTERRUPT(qspiFlash4ErISR, 0, ISR_PRIORITY_FLASH4_ER);

void qspiFlash4TxISR(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrTransmit(&g_qspiFlash4);
}

void qspiFlash4RxISR(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrReceive(&g_qspiFlash4);
}

void qspiFlash4ErISR(void)
{
    IfxCpu_enableInterrupts();
    IfxQspi_SpiMaster_isrError(&g_qspiFlash4);
}

void Flash4_Init(void)
{
    IfxQspi_SpiMaster_Config spiMasterConfig;
    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;

    // Initialize QSPI module configuration
    IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, FLASH4_QSPI_MODULE);
    
    spiMasterConfig.mode = IfxQspi_Mode_master;
    spiMasterConfig.maximumBaudrate = FLASH4_QSPI_MAX_BAUDRATE;
    
    // Configure ISR priorities
    spiMasterConfig.txPriority = ISR_PRIORITY_FLASH4_TX;
    spiMasterConfig.rxPriority = ISR_PRIORITY_FLASH4_RX;
    spiMasterConfig.erPriority = ISR_PRIORITY_FLASH4_ER;
    spiMasterConfig.isrProvider = IfxSrc_Tos_cpu0;
    
    // Configure pins
    const IfxQspi_SpiMaster_Pins pins = {
        &IfxQspi2_SCLK_P15_8_OUT, IfxPort_OutputMode_pushPull,      // SCLK
        &IfxQspi2_MTSR_P15_6_OUT, IfxPort_OutputMode_pushPull,      // MOSI
        &IfxQspi2_MRSTB_P15_7_IN, IfxPort_InputMode_pullDown,       // MISO (use MRSTB variant)
        IfxPort_PadDriver_cmosAutomotiveSpeed3
    };
    spiMasterConfig.pins = &pins;
    
    // Initialize QSPI module
    IfxQspi_SpiMaster_initModule(&g_qspiFlash4, &spiMasterConfig);

    // Initialize QSPI channel configuration
    IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig, &g_qspiFlash4);
    
    spiMasterChannelConfig.ch.baudrate = FLASH4_QSPI_BAUDRATE;
    
    const IfxQspi_SpiMaster_Output slsOutput = {
        &IfxQspi2_SLSO5_P15_1_OUT, IfxPort_OutputMode_pushPull,      // SLSO5 for P15.1
        IfxPort_PadDriver_cmosAutomotiveSpeed3
    };
    spiMasterChannelConfig.sls.output = slsOutput;
    
    // Initialize QSPI channel
    IfxQspi_SpiMaster_initChannel(&g_qspiFlash4Channel, &spiMasterChannelConfig);
}

void Flash4_WriteCommand(uint8 cmd)
{
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, &cmd, NULL_PTR, 1);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
}

void Flash4_ReadManufacturerId(uint8 *deviceId)
{
    uint8 txData[4] = {FLASH4_CMD_READ_ID, 0x00, 0x00, 0x00};
    uint8 rxData[4];
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, 4);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    // Extract manufacturer and device ID (skip first 2 dummy bytes)
    deviceId[0] = rxData[2];  // Manufacturer ID
    deviceId[1] = rxData[3];  // Device ID
}

void Flash4_ReadIdentification(uint8 *outData, uint8 nData)
{
    uint8 txData[1 + 256];  // Command + max data
    uint8 rxData[1 + 256];
    uint16 totalBytes = 1 + nData;
    uint8 i;
    
    txData[0] = FLASH4_CMD_READ_IDENTIFICATION;
    
    // Fill rest with dummy data
    for (i = 1; i < totalBytes; i++)
    {
        txData[i] = 0xFF;
    }
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, totalBytes);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    // The first byte received is dummy/echo, actual data starts from rxData[1]
    for (i = 0; i < nData; i++)
    {
        outData[i] = rxData[i + 1];
    }
}

uint8 Flash4_ReadElectronicId(void)
{
    uint8 txData[5] = {FLASH4_CMD_READ_ELECTRONIC_SIGNATURE, 0x00, 0x00, 0x00, 0xFF};
    uint8 rxData[5];
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, 5);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    // Electronic signature is in the last byte
    return rxData[4];
}

uint8 Flash4_ReadByte(uint8 reg)
{
    uint8 txData[2] = {reg, 0xFF};
    uint8 rxData[2];
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, 2);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    return rxData[1];
}

void Flash4_WriteByte(uint8 reg, uint8 txData)
{
    uint8 data[2] = {reg, txData};
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, data, NULL_PTR, 2);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
}

void Flash4_ReadFlash4(uint8 *outData, uint32 addr, uint16 nData)
{
    uint8 txData[5 + 256]; // Max: 5 bytes command/address + 256 bytes data
    uint8 rxData[5 + 256];
    uint16 i;
    uint16 totalBytes = 5 + nData;
    
    txData[0] = FLASH4_CMD_4READ_FLASH;
    txData[1] = (uint8)((addr >> 24) & 0xFF);
    txData[2] = (uint8)((addr >> 16) & 0xFF);
    txData[3] = (uint8)((addr >> 8) & 0xFF);
    txData[4] = (uint8)(addr & 0xFF);
    
    // Fill rest with dummy data
    for (i = 5; i < totalBytes; i++)
    {
        txData[i] = 0xFF;
    }
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, totalBytes);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    // The first 5 bytes received are dummy/echo, actual data starts from rxData[5]
    for (i = 0; i < nData; i++)
    {
        outData[i] = rxData[i + 5];
    }
}

void Flash4_PageProgram4(uint8 *inData, uint32 addr, uint16 nData)
{
    uint8 txData[5 + 256]; // Max page size + 5 command/address bytes
    uint16 i;
    
    txData[0] = FLASH4_CMD_PAGE_4PROGRAM;
    txData[1] = (uint8)((addr >> 24) & 0xFF);
    txData[2] = (uint8)((addr >> 16) & 0xFF);
    txData[3] = (uint8)((addr >> 8) & 0xFF);
    txData[4] = (uint8)(addr & 0xFF);

    for (i = 0; i < nData; i++)
    {
        txData[i + 5] = inData[i];
    }
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, NULL_PTR, (Ifx_SizeT)(5u + nData));
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
}

void Flash4_SectorErase4(uint32 addr)
{
    uint8 txData[5];
    
    txData[0] = FLASH4_CMD_SECTOR_4ERASE;
    txData[1] = (uint8)((addr >> 24) & 0xFF);
    txData[2] = (uint8)((addr >> 16) & 0xFF);
    txData[3] = (uint8)((addr >> 8) & 0xFF);
    txData[4] = (uint8)(addr & 0xFF);

    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, NULL_PTR, 5);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
}

uint8 Flash4_CheckWIP(void)
{
    uint8 txData[2] = {FLASH4_CMD_READ_STATUS_REG_1, 0xFF};
    uint8 rxData[2];
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, 2);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    return (rxData[1] & 0x01); // WIP bit is bit 0 of status register
}

uint8 Flash4_CheckWEL(void)
{
    uint8 txData[2] = {FLASH4_CMD_READ_STATUS_REG_1, 0xFF};
    uint8 rxData[2];
    
    IfxQspi_SpiMaster_exchange(&g_qspiFlash4Channel, txData, rxData, 2);
    while (IfxQspi_SpiMaster_getStatus(&g_qspiFlash4Channel) == IfxQspi_Status_busy);
    
    return (rxData[1] & 0x02); // WEL bit is bit 1 of status register
}

void Flash4_Reset(void)
{
    // Send software reset command
    Flash4_WriteCommand(FLASH4_CMD_SOFTWARE_RESET);
}

uint8 Flash4_WaitReady(uint32 timeoutMs)
{
    uint32 startTime = IfxStm_get(&MODULE_STM0);
    uint32 ticksPerMs = (uint32)(IfxStm_getFrequency(&MODULE_STM0) / 1000u);
    uint32 timeoutTicks = timeoutMs * ticksPerMs;

    while (Flash4_CheckWIP())
    {
        if ((IfxStm_get(&MODULE_STM0) - startTime) > timeoutTicks)
        {
            return FLASH4_TIMEOUT_ERROR;
        }
    }
    return FLASH4_OK;
}
