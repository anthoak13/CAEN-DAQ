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
