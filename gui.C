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

//File for gui, macro should be interpreted
//Adam Anthony 

#include "MainFrame.h"

void gui()
{
    new MainFrame(gClient->GetRoot(), 400, 220);
}

//Main Program *****************
#ifdef stand
int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   gui();

   theApp.Run();
   /*
    WavedumpConfig config;
    ChannelConfig chan = config.getChan(5);
    std::cout << chan.getEnabled() << std::endl;
    ChannelConfig chan2(false, 0, 0, (ChannelTrigger)0);
    config.addChannel(5, chan2);
    chan = config.getChan(5);
    std::cout << chan.getEnabled() << std::endl;*/
   return 0;
}
#endif
