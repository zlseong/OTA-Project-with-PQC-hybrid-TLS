#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ifx_Types.h"

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * \brief Initialize UART for debug output
 * \return None
 */
void UART_Init(void);

/**
 * \brief Send a string via UART
 * \param str Null-terminated string to send
 */
void UART_SendString(const char *str);

/**
 * \brief Send a single character via UART
 * \param c Character to send
 */
void UART_SendChar(char c);

/**
 * \brief Send a byte in hexadecimal format via UART
 * \param value Byte value to send
 */
void UART_SendHex(uint8 value);

/**
 * \brief Send a 32-bit value in hexadecimal format via UART
 * \param value 32-bit value to send
 */
void UART_SendHex32(uint32 value);

/**
 * \brief Send a newline via UART
 */
void UART_SendNewLine(void);

#endif /* UART_DEBUG_H_ */

