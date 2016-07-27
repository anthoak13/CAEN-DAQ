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

#include "Digitizer.h"
#include "WavedumpConfig.h"
#include "CAENDigitizer.h"
#include <iostream>
#include "TString.h"


CAEN_DGTZ_ErrorCode getDigitizerInfo(CAEN_DGTZ_BoardInfo_t *info, const WavedumpConfig &config) 
{
    CAEN_DGTZ_ErrorCode ret;
    int handle;

    ret = CAEN_DGTZ_OpenDigitizer(config.getUSB() ? CAEN_DGTZ_USB : CAEN_DGTZ_OpticalLink,
				  config.getDigitizerLoc(0),
				  config.getUSB() ? 0 : config.getDigitizerLoc(1),
				  config.getUSB() ? config.getDigitizerLoc(1) : config.getDigitizerLoc(2),
				  &handle);
    if(ret < 0) return ret;

    ret = CAEN_DGTZ_GetInfo(handle, info);
    if(ret < 0) return ret;
    
    ret = CAEN_DGTZ_CloseDigitizer(handle);
    return ret;
}

TString getCAENError(const CAEN_DGTZ_ErrorCode error) 
{
    switch (error) {

    case CAEN_DGTZ_Success:                                return "Operation completed successfully";
    case CAEN_DGTZ_CommError:                              return "Communication error";
    case CAEN_DGTZ_GenericError:                           return "Unspecified error";
    case CAEN_DGTZ_InvalidParam:                           return "Invalid parameter";
    case CAEN_DGTZ_InvalidLinkType:                        return "Invalid Link Type";
    case CAEN_DGTZ_InvalidHandle:                          return "Invalid device handle";
    case CAEN_DGTZ_MaxDevicesError:                        return "Maximum number of devices exceeded";
    case CAEN_DGTZ_BadBoardType:                           return "The operation is not allowed on this type of board";
    case CAEN_DGTZ_BadInterruptLev:                        return "The interrupt level is not allowed";
    case CAEN_DGTZ_BadEventNumber:                         return "The event number is bad";
    case CAEN_DGTZ_ReadDeviceRegisterFail:                 return "Unable to read the registry";
    case CAEN_DGTZ_WriteDeviceRegisterFail:                return "Unable to write into the registry";
    case CAEN_DGTZ_InvalidChannelNumber:                   return "The channel number is invalid";
    case CAEN_DGTZ_ChannelBusy:                            return "The Channel is busy";
    case CAEN_DGTZ_FPIOModeInvalid                         :return "Invalid FPIO Mode";
    case CAEN_DGTZ_WrongAcqMode                            :return "Wrong acquisition mode";
    case CAEN_DGTZ_FunctionNotAllowed                      :return "This function is not allowed for this module";
    case CAEN_DGTZ_Timeout                                 :return "Communication Timeout";
    case CAEN_DGTZ_InvalidBuffer                           :return "The buffer is invalid";
    case CAEN_DGTZ_EventNotFound                           :return "The event is not found";
    case CAEN_DGTZ_InvalidEvent                            :return "The vent is invalid";
    case CAEN_DGTZ_OutOfMemory                             :return "Out of memory";
    case CAEN_DGTZ_CalibrationError                        :return "Unable to calibrate the board";
    case CAEN_DGTZ_DigitizerNotFound                       :return "Unable to open the digitizer";
    case CAEN_DGTZ_DigitizerAlreadyOpen                    :return "The Digitizer is already open";
    case CAEN_DGTZ_DigitizerNotReady                       :return "The Digitizer is not ready to operate";
    case CAEN_DGTZ_InterruptNotConfigured                  :return "The Digitizer has not the IRQ configured";
    case CAEN_DGTZ_DigitizerMemoryCorrupted                :return "The digitizer flash memory is corrupted";
    case CAEN_DGTZ_DPPFirmwareNotSupported                 :return "The digitizer dpp firmware is not supported in this lib version";
    case CAEN_DGTZ_InvalidLicense                          :return "Invalid Firmware License";
    case CAEN_DGTZ_InvalidDigitizerStatus                  :return "The digitizer is found in a corrupted status";
    case CAEN_DGTZ_UnsupportedTrace                        :return "The given trace is not supported by the digitizer";
    case CAEN_DGTZ_InvalidProbe                            :return "The given probe is not supported for the given digitizer's trace";
    case CAEN_DGTZ_NotYetImplemented                       :return "The function is not yet implemented";
    }
}
