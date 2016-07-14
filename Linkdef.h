/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                             Copyright (C) 2016                              //
//                     Adam Anthoony : All rights reserved                     //
//                                                                             //
//      This source code is licensed under the GNU GPL v3.0.You have the       //
//      right to modify and/or redistribute this source code under the terms   //
//      specified in the license, which may be found online at                 //
//      http://www.gnu.org/licenses.                                           //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

//Header for ROOT dictionary generation

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Wavedump and processing class
#pragma link C++ class DataProcessor+;
#pragma link C++ class SignalProcessor+;
#pragma link C++ class ChannelConfig+;
#pragma link C++ class WavedumpConfig+;
#pragma link C++ enum ChannelTrigger;
#pragma link C++ class MainFrame;
#pragma link C++ class ConfigPopout;
#pragma link C++ class WaveConfigPopout;
#pragma link C++ class DAQPopout;

#endif
