/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "UART_Debug.h"
#include "IfxAsclin_Asc.h"
#include "IfxAsclin_PinMap.h"
#include "Ifx_Types.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define UART_BAUDRATE       115200
#define UART_TX_BUFFER_SIZE 128
#define UART_RX_BUFFER_SIZE 32

#define ISR_PRIORITY_ASCLIN_TX  40
#define ISR_PRIORITY_ASCLIN_RX  41
#define ISR_PRIORITY_ASCLIN_ER  42

/*********************************************************************************************************************/
/*-------------------------------------------------Global Variables--------------------------------------------------*/
/*********************************************************************************************************************/
static IfxAsclin_Asc g_ascHandle;
static uint8 g_uartTxBuffer[UART_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 g_uartRxBuffer[UART_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/*********************************************************************************************************************/
/*----------------------------------------------Interrupt Handlers---------------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(asclinTxISR, 0, ISR_PRIORITY_ASCLIN_TX);
void asclinTxISR(void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascHandle);
}

IFX_INTERRUPT(asclinRxISR, 0, ISR_PRIORITY_ASCLIN_RX);
void asclinRxISR(void)
{
    IfxAsclin_Asc_isrReceive(&g_ascHandle);
}

IFX_INTERRUPT(asclinErISR, 0, ISR_PRIORITY_ASCLIN_ER);
void asclinErISR(void)
{
    IfxAsclin_Asc_isrError(&g_ascHandle);
}

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

void UART_Init(void)
{
    IfxAsclin_Asc_Config ascConfig;
    
    /* Initialize configuration with default values */
    /* TC375 Lite Kit uses ASCLIN0 for on-board USB-Serial (COM port) */
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);
    
    /* Set baudrate */
    ascConfig.baudrate.baudrate = UART_BAUDRATE;
    ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;
    
    /* Set frame format */
    ascConfig.frame.dataLength = IfxAsclin_DataLength_8;
    ascConfig.frame.stopBit = IfxAsclin_StopBit_1;
    ascConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
    ascConfig.frame.parityBit = FALSE;
    
    /* Set TX/RX buffers */
    ascConfig.txBuffer = &g_uartTxBuffer;
    ascConfig.txBufferSize = UART_TX_BUFFER_SIZE;
    ascConfig.rxBuffer = &g_uartRxBuffer;
    ascConfig.rxBufferSize = UART_RX_BUFFER_SIZE;
    
    /* Set interrupt priorities */
    ascConfig.interrupt.txPriority = ISR_PRIORITY_ASCLIN_TX;
    ascConfig.interrupt.rxPriority = ISR_PRIORITY_ASCLIN_RX;
    ascConfig.interrupt.erPriority = ISR_PRIORITY_ASCLIN_ER;
    ascConfig.interrupt.typeOfService = IfxSrc_Tos_cpu0;
    
    /* Configure pins for TC375 Lite Kit on-board USB-Serial */
    /* P14.0 = TX, P14.1 = RX (connected to on-board FT232 USB-Serial) */
    const IfxAsclin_Asc_Pins pins = {
        .cts = NULL_PTR,
        .ctsMode = IfxPort_InputMode_noPullDevice,
        .rts = NULL_PTR,
        .rtsMode = IfxPort_OutputMode_pushPull,
        .rx = &IfxAsclin0_RXA_P14_1_IN,
        .rxMode = IfxPort_InputMode_pullUp,
        .tx = &IfxAsclin0_TX_P14_0_OUT,
        .txMode = IfxPort_OutputMode_pushPull,
        .pinDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;
    
    /* Initialize ASCLIN module */
    IfxAsclin_Asc_initModule(&g_ascHandle, &ascConfig);
}

void UART_SendChar(char c)
{
    /* Write to FIFO (blocking) */
    IfxAsclin_Asc_blockingWrite(&g_ascHandle, (uint8)c);
}

void UART_SendString(pchar str)
{
    while(*str != '\0')
    {
        UART_SendChar(*str);
        str++;
    }
}

void UART_SendHex(uint8 byte)
{
    uint8 nibble;
    
    /* High nibble */
    nibble = (byte >> 4) & 0x0F;
    UART_SendChar((nibble > 9) ? (nibble - 10 + 'A') : (nibble + '0'));
    
    /* Low nibble */
    nibble = byte & 0x0F;
    UART_SendChar((nibble > 9) ? (nibble - 10 + 'A') : (nibble + '0'));
}

void UART_SendHex32(uint32 value)
{
    uint8 i;
    for(i = 0; i < 4; i++)
    {
        UART_SendHex((uint8)((value >> (24 - i * 8)) & 0xFF));
    }
}
