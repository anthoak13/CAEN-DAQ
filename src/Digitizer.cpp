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

#ifndef __CINT__
#include "Digitizer.h"
#include "CAENDigitizer.h"
#include <iostream>
#endif
void testDigitizer()
{
    CAEN_DGTZ_ErrorCode ret;
    CAEN_DGTZ_BoardInfo_t BoardInfo;
    int handle;
    char *buffer = NULL;

    ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, 0, &handle);
    std::cout << "Open: " << ret << std::endl;
    ret = CAEN_DGTZ_FreeReadoutBuffer(&buffer);
    std::cout << "Free: " << (int)ret << std::endl;
    ret = CAEN_DGTZ_CloseDigitizer(handle);
    std::cout << "Close: " << (int)ret << std::endl;

}
