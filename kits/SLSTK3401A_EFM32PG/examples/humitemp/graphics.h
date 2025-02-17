/**************************************************************************//**
 * @brief Draws the graphics on the display
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

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "em_i2c.h"

#define DEMO_VERSION "Demo v1.0"

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
void GRAPHICS_Init(void);
void GRAPHICS_ShowStatus(bool si7013_status);
void GRAPHICS_Draw(int32_t tempData, uint32_t rhData);

#ifdef __cplusplus
}
#endif


#endif /* __GRAHPHICS_H */
