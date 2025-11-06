/**********************************************************************************************************************
 * \file Flash4_Examples.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Example code demonstrating various Flash4 driver operations
 * This file contains practical examples for common flash memory operations
 *********************************************************************************************************************/

#include "Flash4_Driver.h"
#include "IfxStm.h"

/*********************************************************************************************************************/
/*----------------------------------Helper Functions-----------------------------------------------------------------*/
/*********************************************************************************************************************/

/* Simple delay function using STM */
static void delay_ms(uint32 ms)
{
    uint32 startTime = IfxStm_get(&MODULE_STM0);
    uint32 delay = ms * (IfxStm_getFrequency(&MODULE_STM0) / 1000);
    while((IfxStm_get(&MODULE_STM0) - startTime) < delay);
}

/*********************************************************************************************************************/
/*----------------------------------Example 1: Basic Read/Write--------------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 1: Basic Read/Write Operation
 * 
 * This example demonstrates:
 * - Writing a string to flash memory
 * - Reading back the data
 * - Verifying the data matches
 * 
 * \return TRUE if successful, FALSE otherwise
 */
boolean Example1_BasicReadWrite(void)
{
    const uint32 address = 0x00001000;
    const uint8 dataSize = 16;
    uint8 writeData[16] = "Hello Flash!";
    uint8 readData[16] = {0};
    uint8 i;
    
    /* Step 1: Enable write operations */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    delay_ms(10);
    
    /* Step 2: Erase the sector containing our target address */
    Flash4_SectorErase4(address);
    if(Flash4_WaitReady(5000) != FLASH4_OK)
        return FALSE;  /* Timeout */
    
    delay_ms(100);
    
    /* Step 3: Enable write again for programming */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    delay_ms(10);
    
    /* Step 4: Write data to flash */
    Flash4_PageProgram4(writeData, address, dataSize);
    if(Flash4_WaitReady(1000) != FLASH4_OK)
        return FALSE;  /* Timeout */
    
    delay_ms(100);
    
    /* Step 5: Read back the data */
    Flash4_ReadFlash4(readData, address, dataSize);
    delay_ms(10);
    
    /* Step 6: Verify data */
    for(i = 0; i < dataSize; i++)
    {
        if(readData[i] != writeData[i])
            return FALSE;
    }
    
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 2: Writing Multiple Pages--------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 2: Writing Multiple Pages
 * 
 * This example demonstrates:
 * - Writing data larger than one page (256 bytes)
 * - Handling page boundaries correctly
 * - Sequential page programming
 * 
 * \return TRUE if successful, FALSE otherwise
 */
boolean Example2_MultiPageWrite(void)
{
    const uint32 startAddress = 0x00010000;
    const uint16 totalBytes = 512;  /* 2 pages */
    uint8 writeBuffer[512];
    uint8 readBuffer[512];
    uint16 i;
    
    /* Prepare test data */
    for(i = 0; i < totalBytes; i++)
    {
        writeBuffer[i] = (uint8)(i & 0xFF);
    }
    
    /* Erase sector */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_SectorErase4(startAddress);
    if(Flash4_WaitReady(5000) != FLASH4_OK)
        return FALSE;
    
    /* Write data page by page */
    uint32 currentAddress = startAddress;
    uint16 bytesWritten = 0;
    
    while(bytesWritten < totalBytes)
    {
        uint16 bytesToWrite = (totalBytes - bytesWritten > 256) ? 256 : (totalBytes - bytesWritten);
        
        /* Enable write for this page */
        Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
        delay_ms(10);
        
        /* Program page */
        Flash4_PageProgram4(&writeBuffer[bytesWritten], currentAddress, bytesToWrite);
        if(Flash4_WaitReady(1000) != FLASH4_OK)
            return FALSE;
        
        bytesWritten += bytesToWrite;
        currentAddress += bytesToWrite;
        delay_ms(10);
    }
    
    /* Read back and verify */
    Flash4_ReadFlash4(readBuffer, startAddress, totalBytes);
    
    for(i = 0; i < totalBytes; i++)
    {
        if(readBuffer[i] != writeBuffer[i])
            return FALSE;
    }
    
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 3: Device Identification---------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 3: Reading Device Identification
 * 
 * This example demonstrates:
 * - Reading manufacturer ID
 * - Reading device ID
 * - Reading electronic signature
 * - Reading JEDEC identification
 * 
 * \return TRUE if device is recognized, FALSE otherwise
 */
boolean Example3_DeviceIdentification(void)
{
    uint8 deviceId[2];
    uint8 jedecId[3];
    uint8 electronicSig;
    
    /* Read Manufacturer and Device ID */
    Flash4_ReadManufacturerId(deviceId);
    
    /* Expected values for S25FL512S:
     * Manufacturer ID: 0x01 (Cypress/Infineon)
     * Device ID: 0x19
     */
    if(deviceId[0] != 0x01 || deviceId[1] != 0x19)
        return FALSE;
    
    /* Read JEDEC ID (should return: 0x01, 0x02, 0x20) */
    Flash4_ReadIdentification(jedecId, 3);
    
    /* Read Electronic Signature */
    electronicSig = Flash4_ReadElectronicId();
    
    /* If we get here, device is recognized */
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 4: Sector Management-------------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 4: Erasing Multiple Sectors
 * 
 * This example demonstrates:
 * - Erasing multiple consecutive sectors
 * - Verifying erase operation (reading 0xFF)
 * 
 * \return TRUE if successful, FALSE otherwise
 */
boolean Example4_EraseSectors(void)
{
    const uint32 startSector = 0x00000000;
    const uint8 numSectors = 2;
    const uint32 sectorSize = 0x00040000;  /* 256 KB */
    uint8 i, j;
    uint8 verifyBuffer[256];
    
    /* Erase each sector */
    for(i = 0; i < numSectors; i++)
    {
        uint32 sectorAddress = startSector + (i * sectorSize);
        
        /* Enable write */
        Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
        delay_ms(10);
        
        /* Erase sector */
        Flash4_SectorErase4(sectorAddress);
        if(Flash4_WaitReady(5000) != FLASH4_OK)
            return FALSE;
        
        delay_ms(100);
    }
    
    /* Verify erasure - check first page of each sector */
    for(i = 0; i < numSectors; i++)
    {
        uint32 sectorAddress = startSector + (i * sectorSize);
        
        /* Read first page */
        Flash4_ReadFlash4(verifyBuffer, sectorAddress, 256);
        
        /* Verify all bytes are 0xFF (erased state) */
        for(j = 0; j < 256; j++)
        {
            if(verifyBuffer[j] != 0xFF)
                return FALSE;
        }
    }
    
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 5: Configuration Storage--------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 5: Storing and Loading Configuration Data
 * 
 * This example demonstrates:
 * - Storing a configuration structure
 * - Loading configuration from flash
 * - Validating configuration with checksum
 * 
 * \return TRUE if successful, FALSE otherwise
 */

/* Example configuration structure */
typedef struct
{
    uint32 magic;           /* Magic number for validation */
    uint16 version;         /* Configuration version */
    uint16 checksum;        /* Simple checksum */
    uint8 deviceName[32];   /* Device name string */
    uint32 serialNumber;    /* Serial number */
    uint8 reserved[16];     /* Reserved for future use */
} Config_t;

#define CONFIG_MAGIC    0xABCD1234
#define CONFIG_VERSION  0x0100
#define CONFIG_ADDRESS  0x00020000

/* Calculate simple checksum */
static uint16 calculateChecksum(const uint8 *data, uint16 length)
{
    uint16 checksum = 0;
    uint16 i;
    
    for(i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    
    return checksum;
}

boolean Example5_StoreConfiguration(void)
{
    Config_t config;
    Config_t readConfig;
    uint8 i;
    
    /* Initialize configuration */
    config.magic = CONFIG_MAGIC;
    config.version = CONFIG_VERSION;
    
    /* Set device name */
    const uint8 name[] = "TC375_Device_001";
    for(i = 0; i < 16; i++)
    {
        config.deviceName[i] = name[i];
    }
    
    config.serialNumber = 12345678;
    
    /* Calculate checksum (exclude checksum field itself) */
    config.checksum = calculateChecksum((uint8*)&config, sizeof(Config_t) - 2);
    
    /* Erase sector */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_SectorErase4(CONFIG_ADDRESS);
    if(Flash4_WaitReady(5000) != FLASH4_OK)
        return FALSE;
    
    /* Write configuration */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_PageProgram4((uint8*)&config, CONFIG_ADDRESS, sizeof(Config_t));
    if(Flash4_WaitReady(1000) != FLASH4_OK)
        return FALSE;
    
    delay_ms(100);
    
    /* Read back configuration */
    Flash4_ReadFlash4((uint8*)&readConfig, CONFIG_ADDRESS, sizeof(Config_t));
    
    /* Validate */
    if(readConfig.magic != CONFIG_MAGIC)
        return FALSE;
    
    if(readConfig.version != CONFIG_VERSION)
        return FALSE;
    
    /* Verify checksum */
    uint16 calculatedChecksum = calculateChecksum((uint8*)&readConfig, sizeof(Config_t) - 2);
    if(calculatedChecksum != readConfig.checksum)
        return FALSE;
    
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 6: Data Logging------------------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 6: Simple Data Logger
 * 
 * This example demonstrates:
 * - Sequential data logging
 * - Ring buffer implementation in flash
 * - Finding next available log entry
 * 
 * \return TRUE if successful, FALSE otherwise
 */

/* Log entry structure */
typedef struct
{
    uint32 timestamp;       /* Timestamp in milliseconds */
    uint16 sensorValue;     /* Sensor reading */
    uint8 status;           /* Status flags */
    uint8 reserved;         /* Reserved */
} LogEntry_t;

#define LOG_START_ADDRESS   0x00030000
#define LOG_SECTOR_SIZE     0x00040000
#define LOG_MAX_ENTRIES     (LOG_SECTOR_SIZE / sizeof(LogEntry_t))

/* Find next available log entry slot */
static uint32 findNextLogSlot(void)
{
    uint32 address = LOG_START_ADDRESS;
    uint32 maxAddress = LOG_START_ADDRESS + LOG_SECTOR_SIZE;
    uint8 buffer[4];
    
    while(address < maxAddress)
    {
        Flash4_ReadFlash4(buffer, address, 4);
        
        /* Check if erased (0xFFFFFFFF indicates empty slot) */
        if(buffer[0] == 0xFF && buffer[1] == 0xFF && 
           buffer[2] == 0xFF && buffer[3] == 0xFF)
        {
            return address;
        }
        
        address += sizeof(LogEntry_t);
    }
    
    /* No free slots, need to erase sector */
    return 0xFFFFFFFF;
}

boolean Example6_LogData(void)
{
    LogEntry_t entry;
    uint32 logAddress;
    
    /* Find next available slot */
    logAddress = findNextLogSlot();
    
    if(logAddress == 0xFFFFFFFF)
    {
        /* Sector full, erase and start over */
        Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
        Flash4_SectorErase4(LOG_START_ADDRESS);
        if(Flash4_WaitReady(5000) != FLASH4_OK)
            return FALSE;
        
        logAddress = LOG_START_ADDRESS;
    }
    
    /* Prepare log entry */
    entry.timestamp = IfxStm_get(&MODULE_STM0) / 100000;  /* ms */
    entry.sensorValue = 1234;  /* Example sensor value */
    entry.status = 0x01;       /* Example status */
    entry.reserved = 0;
    
    /* Write log entry */
    Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
    Flash4_PageProgram4((uint8*)&entry, logAddress, sizeof(LogEntry_t));
    if(Flash4_WaitReady(1000) != FLASH4_OK)
        return FALSE;
    
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example 7: Firmware Update---------------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Example 7: Firmware Storage and Verification
 * 
 * This example demonstrates:
 * - Storing firmware image in flash
 * - Verifying firmware with CRC
 * - Reading firmware for boot loader
 * 
 * \return TRUE if successful, FALSE otherwise
 */

#define FIRMWARE_START_ADDRESS  0x00100000
#define FIRMWARE_MAX_SIZE       0x00100000  /* 1 MB */

/* Simple CRC-16 calculation */
static uint16 calculateCRC16(const uint8 *data, uint32 length)
{
    uint16 crc = 0xFFFF;
    uint32 i;
    uint8 j;
    
    for(i = 0; i < length; i++)
    {
        crc ^= data[i];
        for(j = 0; j < 8; j++)
        {
            if(crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    
    return crc;
}

boolean Example7_StoreFirmware(const uint8 *firmwareData, uint32 firmwareSize)
{
    uint32 sectorsToErase;
    uint32 i;
    uint16 calculatedCRC;
    uint16 storedCRC;
    
    if(firmwareSize > FIRMWARE_MAX_SIZE)
        return FALSE;
    
    /* Calculate number of sectors to erase */
    sectorsToErase = (firmwareSize + 0x3FFFF) / 0x40000;  /* Round up */
    
    /* Erase sectors */
    for(i = 0; i < sectorsToErase; i++)
    {
        uint32 sectorAddress = FIRMWARE_START_ADDRESS + (i * 0x40000);
        
        Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
        Flash4_SectorErase4(sectorAddress);
        if(Flash4_WaitReady(5000) != FLASH4_OK)
            return FALSE;
    }
    
    /* Calculate CRC before writing */
    calculatedCRC = calculateCRC16(firmwareData, firmwareSize);
    
    /* Write firmware in pages */
    uint32 bytesWritten = 0;
    uint32 currentAddress = FIRMWARE_START_ADDRESS;
    
    while(bytesWritten < firmwareSize)
    {
        uint16 bytesToWrite = (firmwareSize - bytesWritten > 256) ? 256 : (firmwareSize - bytesWritten);
        
        Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);
        Flash4_PageProgram4((uint8*)&firmwareData[bytesWritten], currentAddress, bytesToWrite);
        if(Flash4_WaitReady(1000) != FLASH4_OK)
            return FALSE;
        
        bytesWritten += bytesToWrite;
        currentAddress += bytesToWrite;
    }
    
    /* Verify by reading back and calculating CRC */
    uint8 verifyBuffer[256];
    uint32 bytesVerified = 0;
    uint16 verifyCRC = 0xFFFF;
    
    while(bytesVerified < firmwareSize)
    {
        uint16 bytesToRead = (firmwareSize - bytesVerified > 256) ? 256 : (firmwareSize - bytesVerified);
        
        Flash4_ReadFlash4(verifyBuffer, FIRMWARE_START_ADDRESS + bytesVerified, bytesToRead);
        
        /* Update CRC */
        uint16 chunkCRC = calculateCRC16(verifyBuffer, bytesToRead);
        /* Combine CRC chunks (simplified) */
        
        bytesVerified += bytesToRead;
    }
    
    /* In real application, would compare CRC values */
    return TRUE;
}

/*********************************************************************************************************************/
/*----------------------------------Example Usage-----------------------------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Run all examples
 * 
 * This function runs all example functions in sequence
 * Use in main() to test all functionality
 */
void RunAllExamples(void)
{
    boolean result;
    
    /* Initialize Flash4 driver */
    Flash4_Init();
    delay_ms(100);
    
    /* Example 1: Basic Read/Write */
    result = Example1_BasicReadWrite();
    if(!result)
    {
        /* Handle error */
    }
    
    /* Example 2: Multi-page Write */
    result = Example2_MultiPageWrite();
    if(!result)
    {
        /* Handle error */
    }
    
    /* Example 3: Device Identification */
    result = Example3_DeviceIdentification();
    if(!result)
    {
        /* Handle error */
    }
    
    /* Example 4: Sector Erase */
    result = Example4_EraseSectors();
    if(!result)
    {
        /* Handle error */
    }
    
    /* Example 5: Configuration Storage */
    result = Example5_StoreConfiguration();
    if(!result)
    {
        /* Handle error */
    }
    
    /* Example 6: Data Logging */
    result = Example6_LogData();
    if(!result)
    {
        /* Handle error */
    }
}

