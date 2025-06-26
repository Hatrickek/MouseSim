#pragma once
#include <Windows.h>
#include "public.h"
#include "vjoyinterface.h"
/* Class that does everything with the vjoy, testing, accuiring, feeding.*/


class VJoy {
public:
  //Tests if the driver version is equal to dll version.
  static int test_driver();
  //Tests if UINT iInterface is existing.
  static int test_virtual_devices(UINT iInterface);
  //If UINT iInterface is existing, tries to accuire it.
  static int acquire_device(UINT iInterface);
  //If UINT iInterface exist, enable FFB to device.
  static int enable_ffb(UINT iInterface);
  //When UINT iInterface is accuired, feeds vars X Y Z RX to Axises X Y Z RX.
  static void feed_device(UINT iInterface,
                          INT X,
                          INT Y,
                          INT Z,
                          INT RX,
                          INT RY,
                          BOOL BUTTON1,
                          BOOL BUTTON2,
                          BOOL BUTTON3,
                          BOOL BUTTON4,
                          BOOL BUTTON5);

private:
  //Gets status from vjoy device using vjoyinterface.dll.
  static VjdStat _status;
  //Creates new joystick position array named iReport, used to feed all the data at the same time.
  static JOYSTICK_POSITION_V2 _iReport;
};
