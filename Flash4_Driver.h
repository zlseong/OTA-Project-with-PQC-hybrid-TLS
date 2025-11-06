/**********************************************************************************************************************
 * \file Flash4_Driver.h
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * MIKROE-3191 Flash 4 Click Driver for TC375 Lite Kit
 * This driver provides interface to external flash memory via QSPI
 *********************************************************************************************************************/

#ifndef FLASH4_DRIVER_H_
#define FLASH4_DRIVER_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxQspi_SpiMaster.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
/* FLASH4 Commands */
#define FLASH4_CMD_READ_ID                       0x90
#define FLASH4_CMD_READ_IDENTIFICATION           0x9F
#define FLASH4_CMD_READ_ELECTRONIC_SIGNATURE     0xAB
#define FLASH4_CMD_READ_SERIAL_FLASH_PARAMETERS  0x5A
#define FLASH4_CMD_READ_STATUS_REG_1             0x05
#define FLASH4_CMD_READ_STATUS_REG_2             0x07
#define FLASH4_CMD_READ_CONFIGURATION_REG        0x35
#define FLASH4_CMD_READ_BANK_REG                 0x16
#define FLASH4_CMD_WRITE_BANK_REG                0x17
#define FLASH4_CMD_ACCESS_BANK_REG               0xB9
#define FLASH4_CMD_WRITE_REGISTER_WRR            0x01
#define FLASH4_CMD_WRITE_ENABLE_WREN             0x06
#define FLASH4_CMD_WRITE_DISABLE_WRDI            0x04
#define FLASH4_CMD_CLEAR_STATUS_REG              0x30
#define FLASH4_CMD_READ_ECC_STATUS_REG           0x18
#define FLASH4_CMD_READ_AUTO_BOOT_REG            0x14
#define FLASH4_CMD_WRITE_AUTO_BOOT_REG           0x15
#define FLASH4_CMD_PROGRAM_NVDLR                 0x43
#define FLASH4_CMD_WRITE_VDLR                    0x4A
#define FLASH4_CMD_READ_DATA_LEARNING_PATTERN    0x41
#define FLASH4_CMD_READ_FLASH                    0x03
#define FLASH4_CMD_4READ_FLASH                   0x13
#define FLASH4_CMD_FAST_READ_FLASH               0x0B
#define FLASH4_CMD_FAST_4READ_FLASH              0x0C
#define FLASH4_CMD_FAST_READ_DDR                 0x0D
#define FLASH4_CMD_FAST_4READ_DDR                0x0E
#define FLASH4_CMD_PAGE_PROGRAM                  0x02
#define FLASH4_CMD_PAGE_4PROGRAM                 0x12
#define FLASH4_CMD_PROGRAM_SUSPEND               0x85
#define FLASH4_CMD_PROGRAM_RESUME                0x8A
#define FLASH4_CMD_SECTOR_ERASE                  0xD8
#define FLASH4_CMD_SECTOR_4ERASE                 0xDC
#define FLASH4_CMD_BULK_ERASE                    0xC7
#define FLASH4_CMD_ERASE_SUSPEND                 0x75
#define FLASH4_CMD_ERASE_RESUME                  0x7A
#define FLASH4_CMD_WRITE_OTP_PROGRAM             0x42
#define FLASH4_CMD_READ_OTP                      0x4B
#define FLASH4_CMD_READ_ASP                      0x2B
#define FLASH4_CMD_WRITE_ASP_PROGRAM             0x2F
#define FLASH4_CMD_READ_DYB                      0xE0
#define FLASH4_CMD_WRITE_DYB                     0xE1
#define FLASH4_CMD_READ_PPB                      0xE2
#define FLASH4_CMD_WRITE_PPB                     0xE3
#define FLASH4_CMD_ERASE_PPB                     0xE4
#define FLASH4_CMD_READ_PPB_LOCK_BIT             0xA7
#define FLASH4_CMD_WRITE_PPB_LOCK_BIT            0xA6
#define FLASH4_CMD_SOFTWARE_RESET                0xF0
#define FLASH4_CMD_MODE_BIT_RESET                0xFF

/* Flash device IDs */
#define FLASH4_MANUFACTURER_ID                   0x01
#define FLASH4_DEVICE_ID                         0x19

/* Configuration */
#define FLASH4_MAX_PAGE_SIZE                     256
#define FLASH4_QSPI_BAUDRATE                     1000000     /* 1 MHz SPI clock */

/* Interrupt Service Routine priorities */
#define ISR_PRIORITY_FLASH4_TX                   60
#define ISR_PRIORITY_FLASH4_RX                   61
#define ISR_PRIORITY_FLASH4_ER                   62

/* Return values */
#define FLASH4_OK                                0
#define FLASH4_ERROR                             1
#define FLASH4_BUSY                              2
#define FLASH4_TIMEOUT                           3
#define FLASH4_TIMEOUT_ERROR                     3  /* Alias for FLASH4_TIMEOUT */

/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    IfxQspi_SpiMaster         spiMaster;            /* QSPI Master handle            */
    IfxQspi_SpiMaster_Channel spiMasterChannel;     /* QSPI Master Channel handle    */
} Flash4_t;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Initialize Flash4 QSPI interface
 * \return None
 */
void Flash4_Init(void);

/**
 * \brief Get Flash4 handle
 * \return Pointer to Flash4 structure
 */
Flash4_t* Flash4_GetHandle(void);

/**
 * \brief Write a command to the flash
 * \param cmd Command byte
 */
void Flash4_WriteCommand(uint8 cmd);

/**
 * \brief Read manufacturer and device ID
 * \param deviceId Output buffer (2 bytes: Manufacturer ID and Device ID)
 */
void Flash4_ReadManufacturerId(uint8 *deviceId);

/**
 * \brief Read identification
 * \param outData Output buffer
 * \param nData Number of bytes to read
 */
void Flash4_ReadIdentification(uint8 *outData, uint8 nData);

/**
 * \brief Read electronic signature
 * \return Electronic signature byte
 */
uint8 Flash4_ReadElectronicId(void);

/**
 * \brief Read a single byte from register
 * \param reg Register address
 * \return Register value
 */
uint8 Flash4_ReadByte(uint8 reg);

/**
 * \brief Write a single byte to register
 * \param reg Register address
 * \param txData Data to write
 */
void Flash4_WriteByte(uint8 reg, uint8 txData);

/**
 * \brief Read flash memory with 4-byte address
 * \param outData Output buffer
 * \param addr Start address (32-bit)
 * \param nData Number of bytes to read
 */
void Flash4_ReadFlash4(uint8 *outData, uint32 addr, uint16 nData);

/**
 * \brief Write data to flash memory with 4-byte address (page program)
 * \param inData Input data buffer
 * \param addr Start address (32-bit)
 * \param nData Number of bytes to write (max 256)
 */
void Flash4_PageProgram4(uint8 *inData, uint32 addr, uint16 nData);

/**
 * \brief Erase a sector with 4-byte address
 * \param addr Sector address (32-bit)
 */
void Flash4_SectorErase4(uint32 addr);

/**
 * \brief Check if Write In Progress (WIP) bit is set
 * \return 1 if busy, 0 if ready
 */
uint8 Flash4_CheckWIP(void);

/**
 * \brief Check if Write Enable Latch (WEL) bit is set
 * \return 1 if enabled, 0 if disabled
 */
uint8 Flash4_CheckWEL(void);

/**
 * \brief Software reset of the flash device
 */
void Flash4_Reset(void);

/**
 * \brief Wait for flash operation to complete
 * \param timeoutMs Timeout in milliseconds
 * \return FLASH4_OK if ready, FLASH4_TIMEOUT if timeout
 */
uint8 Flash4_WaitReady(uint32 timeoutMs);

#endif /* FLASH4_DRIVER_H_ */

