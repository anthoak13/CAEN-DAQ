#include "Digitizer.h"
#include "CAENDigitizer.h"
#include <iostream>
void testDigitizer()
{
    CAEN_DGTZ_ErrorCode ret;
    CAEN_DGTZ_BoardInfo_t BoardInfo;
    int handle;
    std::cout << "Usb? " << CAEN_DGTZ_USB << std::endl;
    ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, 0, &handle);

    std::cout << "Error: " << ret <<std::endl;
}
