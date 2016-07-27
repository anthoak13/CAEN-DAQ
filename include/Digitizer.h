///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                            Copyright (C) 2016                             //
//                    Adam Anthoony : All rights reserved                    //
//                                                                           //
//     This source code is licensed under the GNU GPL v3.0.You have the      //
//     right to modify and/or redistribute this source code under the terms  //
//     specified in the license, which may be found online at                //
//     http://www.gnu.org/licenses.                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

//Test for digitizer
#include "CAENDigitizer.h"
#include "CAENDigitizerType.h"
#include "WavedumpConfig.h"


CAEN_DGTZ_ErrorCode getDigitizerInfo(CAEN_DGTZ_BoardInfo_t *info, const WavedumpConfig &config);

TString getCAENError(const CAEN_DGTZ_ErrorCode error);

