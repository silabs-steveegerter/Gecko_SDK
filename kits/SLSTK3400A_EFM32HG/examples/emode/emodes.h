/**************************************************************************//**
 * @file emodes.h
 * @brief Happy Gecko energy mode setups (See Data Sheet Table 3.3).
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

#ifndef EMODES_H
#define EMODES_H

#include "em_cmu.h"

void em_EM0_Hfxo(void);

void em_EM0_Hfrco(CMU_HFRCOBand_TypeDef band);

void em_EM1_Hfxo(void);

void em_EM1_Hfrco(CMU_HFRCOBand_TypeDef band);

void em_EM2_LfrcoRTC(void);

void em_EM3_Ulfrco(void);

void em_EM4(void);

#endif // EMODES_H
