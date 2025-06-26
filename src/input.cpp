#include "input.h"
#include <cstdio>

RAWINPUTDEVICE Input::_rawInputDevice[2];
BYTE Input::_buffer[40];
int Input::mouse_x, Input::mouse_y;
int Input::mouse_x_change_, Input::mouse_y_change_, Input::mouse_z_change_;
bool Input::is_left_mouse_button_pressed_;
bool Input::is_right_mouse_button_pressed_;
bool Input::is_mouse_wheel_up_;
bool Input::is_mouse_wheel_down_;
bool Input::is_four_mouse_button_pressed_;
bool Input::is_five_mouse_button_pressed_;
bool Input::is_keyboard_button_pressed_[166];

void Input::get_data(LPARAM lParam) {
  // Determine how big the buffer should be
  UINT buffer_size;
  GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &buffer_size, sizeof(RAWINPUTHEADER));

  // Create a buffer of the correct size - but see note below

  // Call the function again, this time with the buffer to get the data
  if (buffer_size <= 40)
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, _buffer, &buffer_size, sizeof(RAWINPUTHEADER));

  const auto raw = (RAWINPUT*)_buffer;

  // The mouse has not been tested extensively,
  // but I believe it works.
  mouse_x_change_ = raw->data.mouse.lLastX;
  mouse_y_change_ = raw->data.mouse.lLastY;
  mouse_z_change_ = static_cast<short>(raw->data.mouse.usButtonData);
  if (mouse_z_change_ / 120 == 1) {
    is_mouse_wheel_up_ = true;
  }
  else { is_mouse_wheel_up_ = false; }
  if (mouse_z_change_ / -120 == 1) {
    is_mouse_wheel_down_ = true;
  }
  else { is_mouse_wheel_down_ = false; }

  if (raw->header.dwType == RIM_TYPEMOUSE) {
    // Get values from the mouse member (of type RAWMOUSE)
    const bool bStateDown = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN;
    const bool bStateUp = raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP;
    if (bStateDown == true && bStateUp == false) {
      is_left_mouse_button_pressed_ = true;
      is_keyboard_button_pressed_[0x01] = true;
    }

    if (bStateUp == true) {
      is_left_mouse_button_pressed_ = false;
      is_keyboard_button_pressed_[0x01] = false;
    }

    const bool bStateDownTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN;
    const bool bStateUpTwo = raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP;
    if (bStateDownTwo == true && bStateUpTwo == false) {
      is_right_mouse_button_pressed_ = true;
      is_keyboard_button_pressed_[0x02] = true;
    }
    if (bStateUpTwo == true) {
      is_right_mouse_button_pressed_ = false;
      is_keyboard_button_pressed_[0x02] = false;
    }
    const bool bStateDownFour = raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN; //South button
    const bool bStateUpFour = raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP;
    if (bStateDownFour && !bStateUpFour) {
      is_four_mouse_button_pressed_ = true;
      is_keyboard_button_pressed_[0x04] = true;
    }
    if (bStateUpFour) {
      is_four_mouse_button_pressed_ = false;
      is_keyboard_button_pressed_[0x04] = false;
    }
    const bool bStateDownFive = raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN; //North button
    const bool bStateUpFive = raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP;
    if (bStateDownFive && !bStateUpFive) {
      is_five_mouse_button_pressed_ = true;
      is_keyboard_button_pressed_[0x05] = true;
    }
    if (bStateUpFive) {
      is_five_mouse_button_pressed_ = false;
      is_keyboard_button_pressed_[0x05] = false;
    }
  }
  if (raw->header.dwType == RIM_TYPEKEYBOARD) {
    // Get key value from the keyboard member (of type RAWKEYBOARD)
    const USHORT key_code = raw->data.keyboard.VKey;
    const bool key_up = raw->data.keyboard.Flags & RI_KEY_BREAK;

    // Ok because I put the most effort into this section of my code,
    // I will briefly explain.

    // We need a boolean pointer to start.
    bool* pb_to_key = nullptr;

    // The keycode can be one of many possibilities,
    // which is why a switch case is to exhaustive to
    // use and I believe for the most part switch case 
    // would not be efficient.

    // So instead of manually typing every possible
    // case value, we can just start by looping through
    // an expected range of keys the keycode might be.

    for (int i = 0; i < 165; ++i) {
      // We add the hex-code 0x41 because that is the
      // value MSDN lists for virtual keycode 'A'.
      if (key_code == i + 0x01) {
        // However our alphabet or array of booleans that
        // represent it, begins at 0 and is only 25 in length.
        // So i itself is the appropritate index.
        pb_to_key = &is_keyboard_button_pressed_[i + 0x01];

        // At this point we have assigned our boolean pointer variable
        // a new address which is whatever index i would be accessing.
        // We break because there is no need to go further since we found the
        // matching keycode!
        break;
      }
    }

    if (pb_to_key != nullptr) {
      *pb_to_key = check_key_press(*pb_to_key, key_up);
    }
  }
}

bool Input::is_left_mouse_button_down() { return is_left_mouse_button_pressed_; }

bool Input::is_right_mouse_button_down() { return is_right_mouse_button_pressed_; }

bool Input::is_four_mouse_button_down() { return is_four_mouse_button_pressed_; }

bool Input::is_five_mouse_button_down() { return is_five_mouse_button_pressed_; }

int Input::get_mouse_change_x() { return mouse_x_change_; }

int Input::get_mouse_change_y() { return mouse_y_change_; }

int Input::get_mouse_change_z() { return mouse_z_change_; }

bool Input::is_alphabetic_key_down(int letter) { return is_keyboard_button_pressed_[letter]; }

bool Input::check_key_press(const bool is_last_key_state, const bool is_this_key_state) {
  // The following may be overkill, but just know the value returned
  // is based upon the current state of the boolean in question, and the
  // new state we have processed.
  if (is_this_key_state == false) {
    if (is_last_key_state == true) {
      return true;
    }
    return true;
  }
  if (is_this_key_state == true) {
    if (is_last_key_state == false) {
      return false;
    }
    return false;
  }
}
