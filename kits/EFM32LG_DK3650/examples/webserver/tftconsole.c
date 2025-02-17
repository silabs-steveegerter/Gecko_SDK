/***************************************************************************//**
 * @file
 * @brief Provide stdio retargeting to TFT address mapped mode
 * @version 4.4.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_ebi.h"
#include "em_gpio.h"
#include "displayfont8x8.h"
#include "dmd/ssd2119/dmd_ssd2119.h"
#include "bsp.h"

#include "tftconsole.h"

#define fontBits chars_8x8_bits


#define CHARS                      40       /**< # Characters */
#define LINES                      30       /**< # Lines */

/* see TFT_Print()  */
#define PRINT_BUF_SIZE             256
#define PRINT_BUF_OUTPUT_MARGIN    64

static uint8_t charBuffer[LINES][CHARS];    /**< Character buffer */
static uint8_t rgbColor[3];                 /**< Color for font */

/** Cursor position */
static int  xpos, ypos;                     /**< Cursor position */

static bool fullUpdate  = true;             /**< Display updated */
static bool bufferReset = true;             /**< Reset buffer */
static bool tftReset    = true;             /**< Reset TFT */
static bool LFtoCRLF    = 0;                /**< LF to CRLF conversion disabled */
static bool initialized = false;            /**< Intialize TFT serial output */


/**************************************************************************//**
 * @brief
 *   Scroll one line of characters up on the screen
 *****************************************************************************/
static void scrollUp(void)
{
  int y;
  int x;

  /* copy all lines one line up */
  for (y = 0; y < (LINES - 1); y++)
  {
    for (x = 0; x < CHARS; x++)
    {
      charBuffer[y][x] = charBuffer[y + 1][x];
    }
  }
  /* clear last line */
  for (x = 0; x < CHARS; x++)
  {
    charBuffer[LINES - 1][x] = 0;
  }
  xpos       = 0;
  ypos       = LINES - 1;
  fullUpdate = true;
}

/**************************************************************************//**
 * @brief
 *   Transmit/display a character
 *
 * @param[in] c
 *   ASCII character to output
 *
 * @return
 *   -1 on failure, or positive character integer on sucesss
 *****************************************************************************/
static void TFT_TX(int c)
{
  /* check for CR */
  if (c == '\r')
  {
    xpos = 0;
    return;
  }
  /* check for LF */
  if (c == '\n')
  {
    ypos = ypos + 1;
    xpos = 0;
    if (ypos >= LINES)
    {
      /* scroll characters one line up */
      scrollUp();
      ypos = (LINES - 1);
    }
    return;
  }
  /* check for bell character, changes color to red */
  if (c == '\b')
  {
    if (rgbColor[1] == 0xff)
    {
      rgbColor[1] = 0x00;
      rgbColor[2] = 0x00;
    }
    else
    {
      rgbColor[1] = 0xff;
      rgbColor[2] = 0xff;
    }
    return;
  }

  /* check for non-printable characters */
  if (c < ' ' || c > '~')
  {
    c = ' ';
  }
  xpos = xpos + 1;
  if (xpos >= CHARS)
  {
    xpos = 0;
    ypos = ypos + 1;
  }
  if (ypos >= LINES)
  {
    scrollUp();
    ypos = 29;
  }
  charBuffer[ypos][xpos] = c - ' ';
}


/**************************************************************************//**
 * @brief
 *   Display framebuffer
 *
 * @param[in] fullFrame
 *   If true, draw entire screen, if false, draw incremental update (faster)
 *****************************************************************************/
static void TFT_Update(bool fullFrame)
{
  int      x, y;
  uint32_t pixelX, pixelY;
  uint8_t  c, bitField;
  int      i;

  /* Draw a full screen */
  if (fullFrame)
  {
    for (y = 0; y < LINES; y++)
    {
      for (x = 0; x < CHARS; x++)
      {
        pixelX = x * 8;
        pixelY = y * 8;

        c = charBuffer[y][x];
        for (i = 0; i < 8; i++)
        {
          bitField = fontBits[c + 100 * i];
          if (bitField == 0)
          {
            DMD_writeData(pixelX, pixelY + i, (uint8_t *) "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 8);
            continue;
          }

          if (bitField & 0x01)
          {
            DMD_writeColor(pixelX + 0, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 0, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x02)
          {
            DMD_writeColor(pixelX + 1, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 1, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x04)
          {
            DMD_writeColor(pixelX + 2, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 2, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x08)
          {
            DMD_writeColor(pixelX + 3, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 3, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x10)
          {
            DMD_writeColor(pixelX + 4, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 4, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x20)
          {
            DMD_writeColor(pixelX + 5, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 5, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x40)
          {
            DMD_writeColor(pixelX + 6, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 6, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
          if (bitField & 0x80)
          {
            DMD_writeColor(pixelX + 7, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
          }
          else
          {
            DMD_writeColor(pixelX + 7, pixelY + i, 0x00, 0x00, 0x00, 1);
          }
        }
      }
    }
  }
  else
  {
    /* Draw xpos, ypos only */
    c      = charBuffer[ypos][xpos];
    pixelX = xpos * 8;
    pixelY = ypos * 8;
    for (i = 0; i < 8; i++)
    {
      bitField = fontBits[c + 100 * i];
      if (bitField == 0)
      {
        DMD_writeData(pixelX, pixelY + i, (uint8_t *) "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 8);
        continue;
      }

      if (bitField & 0x01)

      {
        DMD_writeColor(pixelX + 0, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 0, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x02)
      {
        DMD_writeColor(pixelX + 1, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 1, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x04)
      {
        DMD_writeColor(pixelX + 2, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 2, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x08)
      {
        DMD_writeColor(pixelX + 3, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 3, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x10)
      {
        DMD_writeColor(pixelX + 4, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 4, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x20)
      {
        DMD_writeColor(pixelX + 5, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 5, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x40)
      {
        DMD_writeColor(pixelX + 6, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 6, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
      if (bitField & 0x80)
      {
        DMD_writeColor(pixelX + 7, pixelY + i, rgbColor[0], rgbColor[1], rgbColor[2], 1);
      }
      else
      {
        DMD_writeColor(pixelX + 7, pixelY + i, 0x00, 0x00, 0x00, 1);
      }
    }
  }
}

/**************************************************************************//**
 * @brief
 *   Initialise TFT output
 *****************************************************************************/
void TFT_Init(void)
{
  int          x, y;
  volatile int i;
  EMSTATUS     status;

  /* Initialize color for font */
  /* Use \b for red text (bell/warning) */
  rgbColor[0] = 0xff;
  rgbColor[1] = 0xff;
  rgbColor[2] = 0xff;

  /* Character buffer */
  if (bufferReset)
  {
    /* Clear character buffer */
    for (y = 0; y < LINES; y++)
    {
      for (x = 0; x < CHARS; x++)
      {
        charBuffer[y][x] = 0;
      }
    }
    /* Set cursor position to upper left */
    xpos = 0;
    ypos = 0;
  }

  /* Display controller */
  if (tftReset)
  {
    /* Configure for EBI mode and reset display */
    BSP_DisplayControl(BSP_Display_EBI);
    BSP_DisplayControl(BSP_Display_ResetAssert);
    BSP_DisplayControl(BSP_Display_PowerDisable);
    /* Short delay */
    for (i = 0; i < 10000; i++)
      ;
    /* Configure display for Direct Drive + SPI mode */
    BSP_DisplayControl(BSP_Display_Mode8080);
    BSP_DisplayControl(BSP_Display_PowerEnable);
    BSP_DisplayControl(BSP_Display_ResetRelease);

    /* Initialize graphics - abort on failure */
    status = DMDIF_init(BC_SSD2119_BASE, BC_SSD2119_BASE + 2);
    if ((status != DMD_OK) && (status != DMD_ERROR_DRIVER_ALREADY_INITIALIZED))
      while (1);

    status = DMD_init(0);
    if ((status != DMD_OK) && (status != DMD_ERROR_DRIVER_ALREADY_INITIALIZED))
      while (1);

    /* Make sure display is configured with correct rotation */
    if ((status == DMD_OK))
      DMD_flipDisplay(1, 1);
  }
  initialized = true;
}

/**************************************************************************//**
 * @brief
 *   Transmit single byte to the TFT
 *
 * @param c
 *   Character to transmit
 *
 * @return
 *   Transmitted character
 *****************************************************************************/
int TFT_WriteChar(char c)
{
  if ((BSP_RegisterRead(&BC_REGISTER->UIF_AEM) == BC_UIF_AEM_EFM))
  {
    if ((BSP_RegisterRead(&BC_REGISTER->ARB_CTRL) != BC_ARB_CTRL_EBI) || (initialized == false))
    {
      if (initialized)
      {
        bufferReset = false;
        tftReset    = true;
        TFT_Init();
      }
      else
      {
        bufferReset = true;
        tftReset    = true;
        TFT_Init();
      }
      fullUpdate = true;
    }
  }

  /* Check for form feed - clear screen */
  if (c == '\f')
  {
    bufferReset = true;
    tftReset    = false;
    TFT_Init();
    fullUpdate = true;
    return c;
  }

  /* Add CR or LF to CRLF if enabled */
  if (LFtoCRLF && (c == '\n'))
  {
    TFT_TX('\r');
  }
  TFT_TX(c);

  if (LFtoCRLF && (c == '\r'))
  {
    TFT_TX('\n');
  }

  /* Update display */
  TFT_Update(fullUpdate);
  fullUpdate = false;
  return c;
}

/**************************************************************************//**
 * @brief
 *   puts() simulation
 *****************************************************************************/
void TFT_Puts(char *pchar)
{
  while (*pchar)
  {
    TFT_WriteChar(*pchar++);
  }
}

/**************************************************************************//**
 * @brief
 *   printf() simulation
 *****************************************************************************/
void TFT_Print(const char *format, ...)
{
  char    buf[PRINT_BUF_SIZE];
  va_list args;

  va_start(args, format);
  if (vsnprintf(buf, PRINT_BUF_SIZE, format, args))
  {
    TFT_Puts(buf);
  }
}


/**************************************************************************//**
 * @brief
 *   LF to CRLF conversion
 *
 * @param on
 *   If non-zero, automatic LF to CRLF conversion will be enabled
 *****************************************************************************/
void TFT_CrLf(int on)
{
  if (on)
  {
    LFtoCRLF = true;
  }
  else
  {
    LFtoCRLF = false;
  }
}

/**************************************************************************//**
 * @brief
 *   Get cursor position
 *
 * @param[out] pos
 *   current cursor position
 *****************************************************************************/
void TFT_PosGet(TFT_Pos * pos)
{
  pos->x = xpos;
  pos->y = ypos;
}

/**************************************************************************//**
 * @brief
 *   Set cursor position
 *
 * @param[in] pos
 *   new cursor position
 *****************************************************************************/
void TFT_PosSet(const TFT_Pos * pos)
{
  int x = pos->x;
  int y = pos->y;

  if ((x < CHARS) && (x >= 0))
  {
    xpos = x;
  }

  if ((y < LINES) && (y >= 0))
  {
    ypos = y;
  }
}

