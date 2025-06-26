#include "mousetovjoy.h"
#include "settings.h"
#include <cmath>

#define STEERING_MAX 16384
#define STEERING_MIN (-16384)

constexpr auto AXIS_MAX = 32767;

//Function responsible for getting and modifying vars for throttle, break, clutch.
float th_limit = 1;
float th_low_limit = 1;
float br_limit = 1;

bool MouseToVjoy::is_cursor_locked;
DOUBLE MouseToVjoy::center_multiplier;

void limit_brakes() {
  if (Input::is_alphabetic_key_down(49)) {
    brake_limit = 0.60f;
  }
  if (Input::is_alphabetic_key_down(50)) {
    brake_limit = 0.70f;
  }
  if (Input::is_alphabetic_key_down(51)) {
    brake_limit = 0.80f;
  }
  if (Input::is_alphabetic_key_down(52)) {
    brake_limit = 0.85f;
  }
  if (Input::is_alphabetic_key_down(53)) {
    brake_limit = 0.90f;
  }
}

void MouseToVjoy::input_logic(INT& axisX,
                              INT& axisY,
                              INT& axisZ,
                              INT& axisRX,
                              INT& axisRY,
                              BOOL& is_button1_clicked,
                              BOOL& is_button2_clicked,
                              BOOL& is_button3_clicked,
                              BOOL& is_button4_clicked,
                              BOOL& is_button5_clicked,
                              DOUBLE deltaTime) {
  if (Input::is_left_mouse_button_down() && axisY < AXIS_MAX * th_limit && use_wheel) {
    axisY = (axisY + (release_time_throttle * deltaTime)) * acceleration_throttle;
  }
  if (Input::is_four_mouse_button_down() && Input::is_left_mouse_button_down()) //set the throttle limit
  {
    if (axisY > AXIS_MAX * th_limit)
      axisY = AXIS_MAX * th_limit;
    th_limit = throttle_limit;
  }
  else { th_limit = 1; }

  if (Input::is_left_mouse_button_down() && Input::is_alphabetic_key_down(17)) {
    //set the low throttle limit
    if (axisY > AXIS_MAX * th_low_limit)
      axisY += AXIS_MAX * th_low_limit;
    th_low_limit = throttle_low_limit;
  }
  else { th_low_limit = 1; }

  if (!Input::is_left_mouse_button_down() && axisY > 1) {
    axisY = (axisY - (release_time_throttle * deltaTime)) / acceleration_throttle;
  }
  if (Input::is_right_mouse_button_down() && axisZ < AXIS_MAX * br_limit) {
    axisZ = (axisZ + (increase_time_break * deltaTime)) * acceleration_break;
  }
  if (Input::is_five_mouse_button_down() && Input::is_right_mouse_button_down()) //set the brake limit
  {
    if (axisZ > AXIS_MAX * br_limit)
      axisZ = AXIS_MAX * br_limit;
    br_limit = brake_limit;
  }
  else { br_limit = 1; }
  if (!Input::is_right_mouse_button_down() && axisZ > 1) {
    axisZ = (axisZ - (release_time_break * deltaTime)) / acceleration_break;
  }

  if (Input::is_alphabetic_key_down(clutch_key) && axisRX < AXIS_MAX) {
    axisRX = (axisRX + (increase_time_clutch * deltaTime)) * acceleration_clutch;
  }
  if (!Input::is_alphabetic_key_down(clutch_key) && axisRX > 1) {
    axisRX = (axisRX - (release_time_clutch * deltaTime)) / acceleration_clutch;
  }
  if (Input::is_alphabetic_key_down(hand_brake_key) && axisRY < AXIS_MAX) {
    constexpr auto release_time_handbrake = 300;
    constexpr auto acceleration_handbrake = 1.01;
    axisRY = (axisRY - (release_time_handbrake * deltaTime)) / acceleration_handbrake;
  }
  if (Input::is_alphabetic_key_down(mouse_lock_key)) {
    SleepEx(250, !(Input::is_alphabetic_key_down(mouse_lock_key)));
    if (is_cursor_locked == false) {
      is_cursor_locked = true;
    }
    else {
      is_cursor_locked = false;
    }
  }
  if (Input::is_alphabetic_key_down(mouse_center_key)) {
    SleepEx(15, !(Input::is_alphabetic_key_down(mouse_center_key)));
    axisX = (32766 / 2);
  }
  if (Input::is_alphabetic_key_down(gear_shift_up_key)) {
    is_button1_clicked = true;
  }
  else {
    is_button1_clicked = false;
  }
  if (Input::is_alphabetic_key_down(gear_shift_down_key)) {
    is_button2_clicked = true;
  }
  else {
    is_button2_clicked = false;
  }

  if (Input::is_alphabetic_key_down(extra_key1)) {
    is_button3_clicked = true;
  }
  else {
    is_button3_clicked = false;
  }

  if (Input::is_alphabetic_key_down(extra_key2)) {
    is_button4_clicked = true;
  }
  else {
    is_button4_clicked = false;
  }

  if (Input::is_alphabetic_key_down(extra_key2)) {
    is_button5_clicked = true;
  }
  else {
    is_button5_clicked = false;
  }

  if (is_cursor_locked) {
    SetCursorPos(0, 5000);
  }
  limit_brakes();
}

void MouseToVjoy::mouse_logic(INT& X, DOUBLE sensitivity, DOUBLE sensitivity_center_reduction, INT use_center_reduction) {
  //vjoy max value is 0-32767 to make it easier to scale linear reduction/acceleration I subtract half of it so 16384 to make it -16384 to 16384.
  if (!use_mouse)
    return;
  X = X - 16384;
  if (X > 0) {
    center_multiplier = pow(sensitivity_center_reduction, (1 - (double)X / (double)STEERING_MAX));
  }
  else if (X < 0) {
    center_multiplier = pow(sensitivity_center_reduction, (1 - (double)X / (double)STEERING_MIN));
  }
  if (use_center_reduction == 1) {
    X = X + ((Input::get_mouse_change_x() * sensitivity) / center_multiplier);
  }
  else {
    X = X + (Input::get_mouse_change_x() * sensitivity);
  }
  if (X > 16384) {
    X = 16384;
  }
  else if (X < -16384) {
    X = -16384;
  }
  X += 16384;
};
