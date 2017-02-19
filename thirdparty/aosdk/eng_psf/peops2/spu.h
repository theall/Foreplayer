/***************************************************************************
                            spu.h  -  description
                             -------------------
    begin                : Wed May 15 2002
    copyright            : (C) 2002 by Pete Bernert
    email                : BlackDove@addcom.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version. See also the license.txt file for *
 *   additional informations.                                              *
 *                                                                         *
 ***************************************************************************/

//*************************************************************************//
// History of changes:
//
// 2004/04/04 - Pete
// - changed plugin to emulate PS2 spu
//
// 2002/05/15 - Pete
// - generic cleanup for the Peops release
//
//*************************************************************************//

#ifndef _SPU2_H
#define _SPU2_H

#include "stdafx.h"
static void SetupTimer(void);
static void RemoveTimer(void);
EXPORT_GCC void CALLBACK SPU2playADPCMchannel(xa_decode_t *xap);

EXPORT_GCC long CALLBACK SPU2init(void);
EXPORT_GCC long CALLBACK SPU2open(void *pDsp);
EXPORT_GCC int CALLBACK SPU2sample(stereo_sample_t *sample);
EXPORT_GCC void CALLBACK SPU2close(void);

#endif

