#include "BatteryManagementSystem.h"

int main()
{
    CBMS * BMS_obj = new CBMS(DEUTSCH, CELCIUS);
    
    assert(BMS_obj->IsBatteryOk(44,22,0.7) == true);
    assert(BMS_obj->IsBatteryOk(85,20,0.8) == false);
    assert(BMS_obj->IsBatteryOk(40,83,0.9) == false);
    assert(BMS_obj->IsBatteryOk(25,30,0.5) == true);
    assert(BMS_obj->IsBatteryOk(2,23,0.5) == true);

    return 0;
}