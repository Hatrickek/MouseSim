#pragma once
#include "input.h"
#include "vjoy.h"

class MouseToVjoy {
public:
  static void input_logic(INT& axisX,
                          INT& axisY,
                          INT& axisZ,
                          INT& axisRX,
                          INT& axisRY,
                          BOOL& is_button1_clicked,
                          BOOL& is_button2_clicked,
                          BOOL& is_button3_clicked,
                          BOOL& is_button4_clicked,
                          BOOL& is_button5_clicked,
                          DOUBLE deltaTime);
  static void mouse_logic(INT& axisX, DOUBLE sensitivity, DOUBLE sensitivity_center_reduction, INT use_center_reduction);

private:
  static bool is_cursor_locked;
  static DOUBLE center_multiplier;
};
