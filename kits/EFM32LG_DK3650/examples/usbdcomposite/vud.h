/***************************************************************************//**
 * @file  vud.h
 * @brief USB Vendor Uniqueue Device.
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

#ifndef __VUD_H
#define __VUD_H

#ifdef __cplusplus
extern "C" {
#endif

void VUD_Init( void );
int  VUD_SetupCmd(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif /* __VUD_H */
