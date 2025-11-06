/**********************************************************************************************************************
 * \file Flash4_Config.h
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Configuration file for Flash4 driver
 * Adjust these parameters according to your hardware setup and requirements
 *********************************************************************************************************************/

#ifndef FLASH4_CONFIG_H_
#define FLASH4_CONFIG_H_

/*********************************************************************************************************************/
/*----------------------------------Hardware Configuration-----------------------------------------------------------*/
/*********************************************************************************************************************/

/* QSPI Module Selection */
#define FLASH4_QSPI_MODULE              &MODULE_QSPI2        /* QSPI module to use */

/* Baudrate Configuration */
#define FLASH4_QSPI_MAX_BAUDRATE        50000000UL  /* Max baudrate for QSPI module */
#define FLASH4_QSPI_BAUDRATE            1000000.0f  /* Channel baudrate: 1 MHz for compatibility */

/* Interrupt Priorities (0-255, lower number = higher priority) */
#define ISR_PRIORITY_FLASH4_TX          60          /* Transmit interrupt priority */
#define ISR_PRIORITY_FLASH4_RX          61          /* Receive interrupt priority */
#define ISR_PRIORITY_FLASH4_ER          62          /* Error interrupt priority */

#endif /* FLASH4_CONFIG_H_ */

