/**************************************************************************//**
 * @file ezr32hg_prs_signals.h
 * @brief EZR32HG_PRS_SIGNALS register and bit field definitions
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 ******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.@n
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.@n
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Laboratories, Inc.
 * has no obligation to support this Software. Silicon Laboratories, Inc. is
 * providing the Software "AS IS", with no express or implied warranties of any
 * kind, including, but not limited to, any implied warranties of
 * merchantability or fitness for any particular purpose or warranties against
 * infringement of any proprietary rights of a third party.
 *
 * Silicon Laboratories, Inc. will not be liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this Software.
 *
 *****************************************************************************/
/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @addtogroup EZR32HG_PRS_Signals
 * @{
 * @brief PRS Signal names
 *****************************************************************************/
#define PRS_VCMP_OUT            ((1 << 16) + 0)  /**< PRS Voltage comparator output */
#define PRS_ADC0_SINGLE         ((8 << 16) + 0)  /**< PRS ADC single conversion done */
#define PRS_ADC0_SCAN           ((8 << 16) + 1)  /**< PRS ADC scan conversion done */
#define PRS_USART0_IRTX         ((16 << 16) + 0) /**< PRS USART 0 IRDA out */
#define PRS_USART0_TXC          ((16 << 16) + 1) /**< PRS USART 0 TX complete */
#define PRS_USART0_RXDATAV      ((16 << 16) + 2) /**< PRS USART 0 RX Data Valid */
#define PRS_USARTRF1_IRTX       ((17 << 16) + 0) /**< PRS USART 1 IRDA out */
#define PRS_USARTRF1_TXC        ((17 << 16) + 1) /**< PRS USART 1 TX complete */
#define PRS_USARTRF1_RXDATAV    ((17 << 16) + 2) /**< PRS USART 1 RX Data Valid */
#define PRS_TIMER0_UF           ((28 << 16) + 0) /**< PRS Timer 0 Underflow */
#define PRS_TIMER0_OF           ((28 << 16) + 1) /**< PRS Timer 0 Overflow */
#define PRS_TIMER0_CC0          ((28 << 16) + 2) /**< PRS Timer 0 Compare/Capture 0 */
#define PRS_TIMER0_CC1          ((28 << 16) + 3) /**< PRS Timer 0 Compare/Capture 1 */
#define PRS_TIMER0_CC2          ((28 << 16) + 4) /**< PRS Timer 0 Compare/Capture 2 */
#define PRS_TIMER1_UF           ((29 << 16) + 0) /**< PRS Timer 1 Underflow */
#define PRS_TIMER1_OF           ((29 << 16) + 1) /**< PRS Timer 1 Overflow */
#define PRS_TIMER1_CC0          ((29 << 16) + 2) /**< PRS Timer 1 Compare/Capture 0 */
#define PRS_TIMER1_CC1          ((29 << 16) + 3) /**< PRS Timer 1 Compare/Capture 1 */
#define PRS_TIMER1_CC2          ((29 << 16) + 4) /**< PRS Timer 1 Compare/Capture 2 */
#define PRS_TIMER2_UF           ((30 << 16) + 0) /**< PRS Timer 2 Underflow */
#define PRS_TIMER2_OF           ((30 << 16) + 1) /**< PRS Timer 2 Overflow */
#define PRS_TIMER2_CC0          ((30 << 16) + 2) /**< PRS Timer 2 Compare/Capture 0 */
#define PRS_TIMER2_CC1          ((30 << 16) + 3) /**< PRS Timer 2 Compare/Capture 1 */
#define PRS_TIMER2_CC2          ((30 << 16) + 4) /**< PRS Timer 2 Compare/Capture 2 */
#define PRS_USB_SOF             ((36 << 16) + 0) /**< PRS USB Start of Frame */
#define PRS_USB_SOFSR           ((36 << 16) + 1) /**< PRS USB Start of Frame Sent/Received */
#define PRS_RTC_OF              ((40 << 16) + 0) /**< PRS RTC Overflow */
#define PRS_RTC_COMP0           ((40 << 16) + 1) /**< PRS RTC Compare 0 */
#define PRS_RTC_COMP1           ((40 << 16) + 2) /**< PRS RTC Compare 1 */
#define PRS_GPIO_PIN0           ((48 << 16) + 0) /**< PRS GPIO pin 0 */
#define PRS_GPIO_PIN1           ((48 << 16) + 1) /**< PRS GPIO pin 1 */
#define PRS_GPIO_PIN2           ((48 << 16) + 2) /**< PRS GPIO pin 2 */
#define PRS_GPIO_PIN3           ((48 << 16) + 3) /**< PRS GPIO pin 3 */
#define PRS_GPIO_PIN4           ((48 << 16) + 4) /**< PRS GPIO pin 4 */
#define PRS_GPIO_PIN5           ((48 << 16) + 5) /**< PRS GPIO pin 5 */
#define PRS_GPIO_PIN6           ((48 << 16) + 6) /**< PRS GPIO pin 6 */
#define PRS_GPIO_PIN7           ((48 << 16) + 7) /**< PRS GPIO pin 7 */
#define PRS_GPIO_PIN8           ((49 << 16) + 0) /**< PRS GPIO pin 8 */
#define PRS_GPIO_PIN9           ((49 << 16) + 1) /**< PRS GPIO pin 9 */
#define PRS_GPIO_PIN10          ((49 << 16) + 2) /**< PRS GPIO pin 10 */
#define PRS_GPIO_PIN11          ((49 << 16) + 3) /**< PRS GPIO pin 11 */
#define PRS_GPIO_PIN12          ((49 << 16) + 4) /**< PRS GPIO pin 12 */
#define PRS_GPIO_PIN13          ((49 << 16) + 5) /**< PRS GPIO pin 13 */
#define PRS_GPIO_PIN14          ((49 << 16) + 6) /**< PRS GPIO pin 14 */
#define PRS_GPIO_PIN15          ((49 << 16) + 7) /**< PRS GPIO pin 15 */
#define PRS_PCNT0_TCC           ((54 << 16) + 0) /**< PRS Triggered compare match */

/** @} End of group EZR32HG_PRS */
/** @} End of group Parts */

