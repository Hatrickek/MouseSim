#include <Windows.h>
#include <cstdio>
#include "vjoy.h"
#include "mousetovjoy.h"
#include "input.h"
#include "Stopwatch.h"
#include "forceFeedBack.h"
#include "settings.h"

HWND hwnd;
WNDCLASSEX wc;
MSG msgWindow;
ForceFeedBack fFB;
win32::Stopwatch sw;
INT axisX, axisY, axisZ, axisRX, axisRY, ffb_strength;
BOOL is_button1_clicked, is_button2_clicked, is_button3_clicked, is_button4_clicked, is_button5_clicked;
void limit_brakes();

void CALLBACK FFBCALLBACK(const PVOID data, const PVOID userData) {
  fFB.ffb_to_v_joy(data, userData);
}

void initialization_code() {
  UINT DEV_ID = 1;
  sw.start();
  VJoy::test_driver();
  VJoy::test_virtual_devices(DEV_ID);
  VJoy::acquire_device(DEV_ID);
  VJoy::enable_ffb(DEV_ID);
  FfbRegisterGenCB(FFBCALLBACK, &DEV_ID);

  sjson::initalize();

  printf("==================================\n");
  printf("Sensitivity = %.2f \n", sensitivity);
  printf("Throttle Attack Time = %.0f \n", increase_time_throttle);
  printf("Throttle Release Time = %.0f \n", release_time_throttle);
  printf("Break Attack Time = %.0f \n", increase_time_break);
  printf("Break Release Time = %.0f \n", release_time_break);
  printf("Clutch Attack Time = %.0f \n", increase_time_clutch);
  printf("Clutch Release Time = %.0f \n", release_time_clutch);
  printf("Throttle key = %d \n", throttle_key);
  printf("Break key = %d \n", brake_key);
  printf("Clutch key = %d \n", clutch_key);
  printf("Gear Shift Up key = %d \n", gear_shift_up_key);
  printf("Gear Shift Down key = %d \n", gear_shift_down_key);
  printf("Hand Brake Key = %d \n", hand_brake_key);
  printf("Mouse Lock key = %d \n", mouse_lock_key);
  printf("Mouse Center key = %d \n", mouse_center_key);
  printf("Use Mouse = %d \n", use_mouse);
  printf("Use Center Reduction = %d \n", use_center_reduction);
  printf("Use Force Feedback = %d \n", use_force_feedback);
  printf("Acceleration Throttle = %.2f \n", acceleration_throttle);
  printf("Acceleration Break = %.2f \n", acceleration_break);
  printf("Acceleration Clutch = %.2f \n", acceleration_clutch);
  printf("Center Multiplier = %.2f \n", center_multiplier);
  printf("Throttle limit = %.2f \n", throttle_limit);
  printf("Throttle low limit = %.2f \n", throttle_low_limit);
  printf("Brake limit = %.2f\n", brake_limit);
  printf("Use wheel = %d\n", use_wheel);
  printf("==================================\n");
  axisX = 32767 / 2; // so the wheel starts at the middle instead of far left.
}

void update_code() {
  Sleep(1);
  if (fFB.get_ffb_size().get_effect_type() == "Constant") {
    if (fFB.get_ffb_size().get_direction() > 100) {
      ffb_strength = (fFB.get_ffb_size().get_magnitude()) * (sw.elapsedMilliseconds() * 0.001);
    }
    else {
      ffb_strength = -(fFB.get_ffb_size().get_magnitude()) * (sw.elapsedMilliseconds() * 0.001);
    }
  }
  if (fFB.get_ffb_size().get_effect_type() == "Period") {
    ffb_strength = (fFB.get_ffb_size().get_offset() * 0.5) * (sw.elapsedMilliseconds() * 0.001);
  }
  if (use_force_feedback) {
    axisX = axisX + ffb_strength;
    ffb_strength = 0;
  }
  MouseToVjoy::input_logic(axisX,
                           axisY,
                           axisZ,
                           axisRX,
                           axisRY,
                           is_button1_clicked,
                           is_button2_clicked,
                           is_button3_clicked,
                           is_button4_clicked,
                           is_button5_clicked,
                           sw.elapsedMilliseconds());
  VJoy::feed_device(1,
                    axisX,
                    axisY,
                    axisZ,
                    axisRX,
                    axisRY,
                    is_button1_clicked,
                    is_button2_clicked,
                    is_button3_clicked,
                    is_button4_clicked,
                    is_button5_clicked);
  is_button1_clicked = false;
  is_button2_clicked = false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  switch (Msg) {
    case WM_CREATE: RAWINPUTDEVICE m_Rid[2];
    //Keyboard
      m_Rid[0].usUsagePage = 1;
      m_Rid[0].usUsage = 6;
      m_Rid[0].dwFlags = RIDEV_INPUTSINK;
      m_Rid[0].hwndTarget = hwnd;
    //Mouse
      m_Rid[1].usUsagePage = 1;
      m_Rid[1].usUsage = 2;
      m_Rid[1].dwFlags = RIDEV_INPUTSINK;
      m_Rid[1].hwndTarget = hwnd;
      RegisterRawInputDevices(m_Rid, 2, sizeof(RAWINPUTDEVICE));

      break;
    case WM_INPUT: Input::get_data(lParam);
      MouseToVjoy::mouse_logic(axisX, sensitivity, center_multiplier, use_center_reduction);
      break;
    case WM_CLOSE: PostQuitMessage(0);
    case WM_DESTROY: DestroyWindow(hwnd);
    default: return DefWindowProc(hwnd, Msg, wParam, lParam);
  }
  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  static const char* class_name = "DUMMY_CLASS";
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = class_name;
  if (RegisterClassEx(&wc)) {
    CreateWindowEx(0, class_name, "dummy_name", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
  }
  const std::string cmd_line = lpCmdLine;
  //No point of checking for this since we must 
  //specify a preset to use at start. 
  if (cmd_line != "-noconsole") {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::ios::sync_with_stdio();
  }
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  initialization_code();

  while (true) {
    sw.stop();
    sw.start();
    while (PeekMessage(&msgWindow, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msgWindow);
      DispatchMessage(&msgWindow);
    }
    update_code();
    if (msgWindow.message == WM_QUIT || msgWindow.message == WM_DESTROY) {
      break;
    }
  }
}
