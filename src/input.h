#pragma once
#include <Windows.h>

/* http://msdn.microsoft.com/en-us/library/ms645540%28VS.85%29.aspx */

class Input {
public:
	// The constructor is important. I suppose I could of made a separate function for initalization
	// but I am trying to keep a consistent attitude with things.


	// Only call this once when your Application begins.
	// I may be wrong, but I saw nothing from what I read that indicated
	// devices need to be unregistered.


	// Everytime your MsgProc's uMsg is WM_INPUT, you will want to call the GetData function.
	// I believe this is smarter than calling it every frame in a game loop.

	static void get_data(LPARAM lParam);
	// This should be self-explanatory. Depending on which function
	// you call, you will get true or false about the state of the mouse button.
	// The code in the source file may be a bit difficult to follow,
	// but I tried my best to make it match what was described as the best way
	// to be sure the 

	static bool is_left_mouse_button_down();
	static bool is_right_mouse_button_down();
	static bool is_four_mouse_button_down();
	static bool is_five_mouse_button_down();

	// These functions return values that are relative to the change in position
	// of your mouse.

	static int get_mouse_change_x();
	static int get_mouse_change_y();
	static int get_mouse_change_z();
	// OPTIONALLY:
	// To obtain exact mouse coords check the uMsg in your Application's MsgProc
	// for WM_MOUSEMOVE, and use HIWORD() LOWORD() functions to extract the mouse X,Y
	// from lParam. Store them in the below variables.

	static int mouse_x, mouse_y;

	// Alphabetic as in any letter from the Alphabet. So IsAlphabeticKeyDown returns
	// a value of true or false from an array of 25 booleans. Each index is associated
	// with a position in the english alphabet. Use one of the enumerated values
	// such as VKey_A and subtract 0x41 or make a different enumeration list
	// for all 25 letters and set the first value to 0.

	static bool is_alphabetic_key_down(int letter);

private:
	// Two input devices are covered by this class. Mouse and Keyboard.
	// A good reference for them is: http://www.toymaker.info/Games/html/raw_input.html
	static RAWINPUTDEVICE _rawInputDevice[2];

	static BYTE _buffer[40];
	
	static int mouse_x_change_, mouse_y_change_, mouse_z_change_;

	static bool is_left_mouse_button_pressed_;
	static bool is_right_mouse_button_pressed_;
	static bool is_mouse_wheel_up_;
	static bool is_mouse_wheel_down_;
	static bool is_four_mouse_button_pressed_;
	static bool is_five_mouse_button_pressed_;

	static bool is_keyboard_button_pressed_[166];

	// The CheckKeyPress function is because of these issues:

	// 1. The Mouse
	// The flag is only set once when the button is pressed and so to determine if the mouse button is being held down you need to record it as down
	// until you get a RI_MOUSE_LEFT_BUTTON_UP flag.

	// 2. The Keyboard
	// When a key is held down the raw->data.heyboard.Flags has the RI_KEY_MAKE  bit set (actually none as the value is 0) and when the key is released 
	// the  RI_KEY_BREAK  bit is set (value is 1). 

	// 1 and 2 is what I got directly from ToyMaker's website. I believe for the most part CheckKeyPress takes what was said into account.
	static bool check_key_press(bool is_last_key_state, bool is_this_key_state);

};
