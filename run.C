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



void run()
{
    gInterpreter->AddIncludePath("include");
    gInterpreter->AddIncludePath("include/gui");
    gSystem->Load("lib/libCAENDigitizer.so");
    gROOT->ProcessLine(".x gui.C");
}
