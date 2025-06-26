#pragma once
#include "json.hpp"
using json = nlohmann::json;

extern float sensitivity;
extern float increase_time_throttle;
extern float release_time_throttle;
extern float increase_time_break;
extern float release_time_break;
extern float increase_time_clutch;
extern float release_time_clutch;
extern int throttle_key;
extern int brake_key;
extern int clutch_key;
extern int gear_shift_up_key;
extern int gear_shift_down_key;
extern int hand_brake_key;
extern int mouse_lock_key;
extern int mouse_center_key;
extern int extra_key1;
extern int extra_key2;
extern int extra_key3;
extern bool use_mouse;
extern int use_center_reduction;
extern bool use_force_feedback;
extern float acceleration_throttle;
extern float acceleration_break;
extern float acceleration_clutch;
extern float center_multiplier;
extern float throttle_limit;
extern float throttle_low_limit;
extern float brake_limit;
extern float brake_limit1;
extern float brake_limit2;
extern float brake_limit3;
extern float brake_limit4;
extern float brake_limit5;
extern bool use_wheel;
namespace sjson
{
	void initalize();
};