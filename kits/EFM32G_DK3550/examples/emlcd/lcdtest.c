/**************************************************************************//**
 * @file
 * @brief LCD Controller test and demo
 * @version 4.4.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <string.h>
#include <stddef.h>
/* EFM32 CMSIS */
#include "em_device.h"
#include "em_emu.h"
#include "em_lcd.h"
#include "em_gpio.h"
/* BSP */
#include "bsp.h"
/* Board drivers */
#include "segmentlcd.h"
#include "rtcdriver.h"
#include "lcdtest.h"

/** Demo scroll text */
static char *stext = "Silicon Labs        ";

/* Joystick press */
#define JOY_MODE_EM3       BC_UIF_JOYSTICK_UP
#define JOY_MODE_EM4       BC_UIF_JOYSTICK_DOWN
#define JOY_MODE_BUTTON    BC_UIF_JOYSTICK_CENTER
#define JOY_MODE_NONE      0x0000

static uint16_t emMode = JOY_MODE_NONE;

/** Timer used for bringing the system back to EM0. */
RTCDRV_TimerID_t xTimerForWakeUp;

/**************************************************************************//**
 * @brief GPIO Interrupt handler
 *****************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint16_t     joystick;

  /* Clear interrupt */
  BSP_InterruptFlagsClear(BC_INTEN_JOYSTICK);
  GPIO_IntClear(1 << 2);

  /* Read and store joystick activity */
  joystick = BSP_JoystickGet();

  /* One bit for each direction left/right/up/down + button */
  switch (joystick)
  {
  case JOY_MODE_BUTTON:
    emMode = JOY_MODE_NONE;
    BSP_LedsSet(0x0000);
    SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 0);
    SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 0);
    break;
  case JOY_MODE_EM3:
  case JOY_MODE_EM4:
    emMode = joystick;
    BSP_LedsSet(0xffff);
    SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 1);
    SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 1);
    break;
  default:
    break;
  }
}

/**************************************************************************//**
 * @brief Initialize GPIO interrupt on PC14
 *****************************************************************************/
void GPIO_IRQInit(void)
{
  /* Configure PC14 as input pull, drive high */
  GPIO_PinModeSet(gpioPortE, 2, gpioModeInputPull, 1);

  /* Set and enable falling edge interrupt */
  GPIO_IntConfig(gpioPortE, 2, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

/**************************************************************************//**
 * @brief Callback function lighting the "Antenna symbol"
 *****************************************************************************/
void RtcTrigger( RTCDRV_TimerID_t id, void *user)
{
  ( void)id;
  ( void)user;
  /* Just a few No-OPerations to have a place to put a breakpoint */
  SegmentLCD_Symbol(LCD_SYMBOL_EFM32, 1);
}

/**************************************************************************//**
 * @brief Sleeps in EM1 in given time unless some other IRQ sources has been
 *        enabled
 * @param msec Time in milliseconds
 *****************************************************************************/
void EM1Sleep(uint32_t msec)
{
  /* Wake us up after msec (or joystick pressed) */
  NVIC_DisableIRQ(LCD_IRQn);
  /* Tell AEM we're in EM1 */
  BSP_EnergyModeSet(1);
  RTCDRV_StartTimer( xTimerForWakeUp, rtcdrvTimerTypeOneshot, msec, RtcTrigger, NULL);
  EMU_EnterEM1();
  BSP_EnergyModeSet(0);
  NVIC_EnableIRQ(LCD_IRQn);
}

/**************************************************************************//**
 * @brief Sleeps in EM2 in given time unless some other IRQ sources has been
 *        enabled
 * @param msec Time in milliseconds
 *****************************************************************************/
void EM2Sleep(uint32_t msec)
{
  /* Wake us up after msec (or joystick pressed) */
  NVIC_DisableIRQ(LCD_IRQn);
  /* Tell AEM we're in EM2 */
  BSP_EnergyModeSet(2);
  RTCDRV_StartTimer( xTimerForWakeUp, rtcdrvTimerTypeOneshot, msec, NULL, NULL);
  EMU_EnterEM2(true);
  BSP_EnergyModeSet(0);
  NVIC_EnableIRQ(LCD_IRQn);
}


/**************************************************************************//**
 * @brief Sleeps in EM3 until GPIO interrupt is triggered
 *****************************************************************************/
void EM3Sleep(void)
{
  /* Tell AEM we're in EM3  */
  BSP_EnergyModeSet(3);
  SegmentLCD_Disable();
  EMU_EnterEM3(true);
  BSP_EnergyModeSet(0);
  SegmentLCD_Init(false);
}

/**************************************************************************//**
 * @brief Sleeps in EM4 until reset
 *****************************************************************************/
void EM4Sleep(void)
{
  /* Tell AEM we're in EM4 */
  BSP_EnergyModeSet(4);
  EMU_EnterEM4();
  /* we will never wake up again here - reset required */
}

/**************************************************************************//**
 * @brief LCD scrolls a text over the display, sort of "polled printf"
 *****************************************************************************/
void ScrollText(char *scrolltext)
{
  int  i, len;
  char buffer[8];

  buffer[7] = 0x00;
  len       = strlen(scrolltext);
  if (len < 7) return;
  for (i = 0; i < (len - 7); i++)
  {
    memcpy(buffer, scrolltext + i, 7);
    SegmentLCD_Write(buffer);
    EM2Sleep(125);
  }
}

/**************************************************************************//**
 * @brief LCD Blink Test
 *****************************************************************************/
void BlinkTest(void)
{
  SegmentLCD_EnergyMode(0, 1);
  SegmentLCD_EnergyMode(1, 1);
  SegmentLCD_EnergyMode(2, 1);
  SegmentLCD_EnergyMode(3, 1);
  SegmentLCD_EnergyMode(4, 1);

  /* 2 minutes to midnight */
  SegmentLCD_Number(2358);
  SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);
  SegmentLCD_Symbol(LCD_SYMBOL_GECKO, 1);
  SegmentLCD_Symbol(LCD_SYMBOL_EFM32, 1);
  SegmentLCD_Write(" EFM32 ");
  LCD_BlinkEnable(true);
  LCD_SyncBusyDelay(0xFFFFFFFF) ;
  EM2Sleep(2000);

  SegmentLCD_EnergyMode(4, 0);
  EM2Sleep(62);
  SegmentLCD_EnergyMode(3, 0);
  EM2Sleep(62);
  SegmentLCD_EnergyMode(2, 0);
  EM2Sleep(62);
  SegmentLCD_EnergyMode(1, 0);
  EM2Sleep(62);
  SegmentLCD_EnergyMode(0, 0);
  LCD_BlinkEnable(false);
  LCD_SyncBusyDelay(0xFFFFFFFF);
}

/**************************************************************************//**
 * @brief LCD Animate
 *****************************************************************************/
void AnimateTest(void)
{
  /* Initialize this C99-style, to show what's going on */
  LCD_AnimInit_TypeDef anim = {
    .enable = true,
    .AReg = 0x00,
    .AShift = lcdAnimShiftNone,
    .BReg = 0x03,
    .BShift = lcdAnimShiftLeft,
    .animLogic = lcdAnimLogicOr,
  };
  LCD_FrameCountInit_TypeDef fc = {
    .enable = true,
    .top = 5,
    .prescale = lcdFCPrescDiv1
  };

  SegmentLCD_Write("ANIMATE");

  /* Initialize frame count updates */
  LCD_FrameCountInit(&fc);
  LCD_AnimInit(&anim);

  EM2Sleep(5000);

  anim.BReg = 0xAA;
  LCD_AnimInit(&anim);

  LCD_BlinkEnable(true);
  EM2Sleep(5000);

  LCD_BlinkEnable(false);
}

/**************************************************************************//**
 * @brief LCD Test Routine, shows various text and patterns
 *****************************************************************************/
void Test(void)
{
  int i, numberOfIterations = 0;

  /* Enable interrupts on pushbuttons */
  BSP_InterruptFlagsClear(0xffff);
  BSP_InterruptEnable(BC_INTEN_JOYSTICK);
  BSP_LedsSet(0x0000);

  /* Initialize GPIO */
  GPIO_IRQInit();

  /* Loop through funny pattern */
  while (1)
  {
    SegmentLCD_AllOff();
    if (emMode != JOY_MODE_NONE)
    {
      SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 1);
      SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 1);
    }
    else
    {
      for (i = 100; i > 0; i--)
      {
        SegmentLCD_Number(i);
        EM2Sleep(10);
      }
      SegmentLCD_NumberOff();

      SegmentLCD_Symbol(LCD_SYMBOL_GECKO, 1);
      SegmentLCD_Symbol(LCD_SYMBOL_EFM32, 1);
      SegmentLCD_Write(" Gecko ");
      EM2Sleep(1000);

      SegmentLCD_AllOn();
      EM2Sleep(1000);

      SegmentLCD_AllOff();
    }
    if (emMode != JOY_MODE_NONE)
    {
      SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 1);
      SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 1);
    }
    else
    {
      SegmentLCD_Write("OOOOOOO");
      EM2Sleep(62);
      SegmentLCD_Write("XXXXXXX");
      EM2Sleep(62);
      SegmentLCD_Write("+++++++");
      EM2Sleep(62);
      SegmentLCD_Write("@@@@@@@");
      EM2Sleep(62);
      SegmentLCD_Write("SILICON");
      EM2Sleep(250);
      SegmentLCD_Write("@@LICON");
      EM2Sleep(62);
      SegmentLCD_Write(" @@ICON");
      EM2Sleep(62);
      SegmentLCD_Write(" L@@CON");
      EM2Sleep(62);
      SegmentLCD_Write(" LA@@ON");
      EM2Sleep(62);
      SegmentLCD_Write(" LAB@@N");
      EM2Sleep(62);
      SegmentLCD_Write(" LABS@@");
      EM2Sleep(62);
      SegmentLCD_Write(" LABS @");
      EM2Sleep(250);
      SegmentLCD_Write("-EFM32-");
      EM2Sleep(250);

      /* Various eye candy */
      SegmentLCD_AllOff();
      if (emMode != JOY_MODE_NONE)
      {
        SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 1);
        SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 1);
      }
      for (i = 0; i < 8; i++)
      {
        SegmentLCD_Number(numberOfIterations + i);
        SegmentLCD_ARing(i, 1);
        EM2Sleep(20);
      }
      for (i = 0; i < 8; i++)
      {
        SegmentLCD_Number(numberOfIterations + i);
        SegmentLCD_ARing(i, 0);
        EM2Sleep(100);
      }

      for (i = 0; i < 5; i++)
      {
        SegmentLCD_Number(numberOfIterations + i);
        SegmentLCD_Battery(i);
        SegmentLCD_EnergyMode(i, 1);
        EM2Sleep(100);
        SegmentLCD_EnergyMode(i, 0);
        EM2Sleep(100);
      }
      SegmentLCD_Symbol(LCD_SYMBOL_ANT, 1);
      for (i = 0; i < 4; i++)
      {
        SegmentLCD_EnergyMode(i, 1);
        EM2Sleep(100);
      }
      SegmentLCD_Symbol(LCD_SYMBOL_ANT, 0);
    }
    /* Energy Modes */
    SegmentLCD_NumberOff();
    SegmentLCD_Symbol(LCD_SYMBOL_GECKO, 1);
    SegmentLCD_Symbol(LCD_SYMBOL_EFM32, 1);
    if ((emMode != JOY_MODE_EM3) && (emMode != JOY_MODE_EM4))
    {
      ScrollText("Energy Mode demo, joystick up or down for EM3 and EM4       ");
    }
    SegmentLCD_Write("  EM0  ");
    SegmentLCD_Number(0);
    SegmentLCD_EnergyMode(0, 1);
    SegmentLCD_EnergyMode(1, 1);
    SegmentLCD_EnergyMode(2, 1);
    SegmentLCD_EnergyMode(3, 1);
    SegmentLCD_EnergyMode(4, 1);
    RTCDRV_Delay(4000);
    SegmentLCD_Write("  EM1  ");
    SegmentLCD_Number(1111);
    SegmentLCD_EnergyMode(0, 0);
    EM1Sleep(4000);
    SegmentLCD_Write("  EM2  ");
    SegmentLCD_Number(2222);
    SegmentLCD_EnergyMode(1, 0);
    EM2Sleep(4000);

    /* Check if somebody's pressed joystick up */
    if (emMode == JOY_MODE_EM3)
    {
      ScrollText("Going down to EM3, press joystick to wake up    ");
      SegmentLCD_Write("  EM3  ");
      SegmentLCD_Number(3333);
      RTCDRV_Delay(1000);

      /* Wake up on GPIO interrupt */
      EM3Sleep();
      SegmentLCD_Number(0000);
      SegmentLCD_Write("--EM0--");
      RTCDRV_Delay(500);
      SegmentLCD_Symbol(LCD_SYMBOL_PAD0, 0);
      SegmentLCD_Symbol(LCD_SYMBOL_PAD1, 0);
      BSP_LedsSet(0x0000);
      emMode = JOY_MODE_NONE;
    }
    /* Check if somebody's joystick down */
    if (emMode == JOY_MODE_EM4)
    {
      ScrollText("Going down to EM4, press reset to restart    ");
      SegmentLCD_Write("  EM4  ");
      SegmentLCD_Number(4444);
      RTCDRV_Delay(1000);

      /* Wake up on reset */
      EM4Sleep();
    }
    SegmentLCD_EnergyMode(0, 0);
    SegmentLCD_EnergyMode(1, 0);
    SegmentLCD_EnergyMode(2, 0);
    SegmentLCD_EnergyMode(3, 0);
    SegmentLCD_EnergyMode(4, 0);

    /* Scrolltext */
    ScrollText(stext);

    /* Blink and animation featurs */
    BlinkTest();
    AnimateTest();

    numberOfIterations++;
  }
}

