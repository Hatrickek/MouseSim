#include "settings.h"
#include <fstream>
#include <iostream>
float sensitivity;
float increase_time_throttle;
float release_time_throttle;
float increase_time_break;
float release_time_break;
float increase_time_clutch;
float release_time_clutch;
int throttle_key;
int brake_key;
int clutch_key;
int gear_shift_up_key;
int gear_shift_down_key;
int hand_brake_key;
int mouse_lock_key;
int mouse_center_key;
int extra_key1;
int extra_key2;
int extra_key3;
bool use_mouse;
int use_center_reduction;
bool use_force_feedback;
float acceleration_throttle;
float acceleration_break;
float acceleration_clutch;
float center_multiplier;
float throttle_limit;
float throttle_low_limit;
float brake_limit;
float brake_limit1;
float brake_limit2;
float brake_limit3;
float brake_limit4;
float brake_limit5;
bool use_wheel;

namespace sjson {
  static std::string config_name;
  bool check_json_file();
  void create_json_file();
  void load_json_file();
  json j2 = {
    {"Sensitivity", 2},
    {"IncreaseTimeThrottle", 100},
    {"ReleaseTimeThrottle", 100},
    {"IncreaseTimeBreak", 100},
    {"ReleaseTimeBreak", 100},
    {"IncreaseTimeClutch", 200},
    {"ReleaseTimeClutch", 200},
    {"ThrottleKey", 87},
    {"BreakKey", 83},
    {"ClutchKey", 67},
    {"GearShiftUpKey", 69},
    {"GearShiftDownKey", 81},
    {"HandBrakeKey", 32},
    {"MouseLockKey", 35},
    {"MouseCenterKey", 36},
    {"UseMouse", true},
    {"UseCenterReduction", 1},
    {"UseForceFeedback", true},
    {"AccelerationThrottle", 1.01},
    {"AccelerationBreak", 1.01},
    {"AccelerationClutch", 1.01},
    {"CenterMultiplier", 2},
    {"ThrottleLimit", 0.6},
    {"ThrottleLowLimit", 0.3},
    {"BrakeLimit", 0.7},
    {"BrakeLimit1", 0.6},
    {"BrakeLimit2", 0.7},
    {"BrakeLimit3", 0.8},
    {"BrakeLimit4", 0.85},
    {"BrakeLimit5", 0.95},
    {"UseWheel", true},
  };

  void initalize() {
    if (!check_json_file())
      create_json_file();
    load_json_file();
  }

  void load_json_file() {
    std::ifstream file(config_name);
    j2 = json::parse(file);
    //parse then set the variables.
    sensitivity = j2["Sensitivity"];
    increase_time_throttle = j2["IncreaseTimeThrottle"];
    release_time_throttle = j2["ReleaseTimeThrottle"];
    increase_time_break = j2["IncreaseTimeBreak"];
    release_time_break = j2["ReleaseTimeBreak"];
    increase_time_clutch = j2["IncreaseTimeClutch"];
    release_time_clutch = j2["ReleaseTimeClutch"];
    throttle_key = j2["ThrottleKey"];
    brake_key = j2["BreakKey"];
    clutch_key = j2["ClutchKey"];
    gear_shift_up_key = j2["GearShiftUpKey"];
    gear_shift_down_key = j2["GearShiftDownKey"];
    hand_brake_key = j2["HandBrakeKey"];
    mouse_lock_key = j2["MouseLockKey"];
    mouse_center_key = j2["MouseCenterKey"];
    use_mouse = j2["UseMouse"];
    use_center_reduction = j2["UseCenterReduction"];
    use_force_feedback = j2["UseForceFeedback"];
    acceleration_throttle = j2["AccelerationThrottle"];
    acceleration_break = j2["AccelerationBreak"];
    acceleration_clutch = j2["AccelerationClutch"];
    center_multiplier = j2["CenterMultiplier"];
    brake_limit = j2["BrakeLimit"];
    brake_limit1 = j2["BrakeLimit1"];
    brake_limit2 = j2["BrakeLimit2"];
    brake_limit3 = j2["BrakeLimit3"];
    brake_limit4 = j2["BrakeLimit4"];
    brake_limit5 = j2["BrakeLimit5"];
    throttle_limit = j2["ThrottleLimit"];
    throttle_low_limit = j2["ThrottleLowLimit"];
    use_wheel = j2["UseWheel"];
    file.close();
  }

  void create_json_file() {
    std::ofstream file;
    file.open("config_racing.json");
    file << j2.dump(1);
    file.close();
  }

  bool check_json_file() {
    static int selectedI;

    std::cout << "Select a config: 1-Racing, 2-Truck, 3-Formula" << std::endl;
    std::cin >> selectedI;
    switch (selectedI) {
      case 1: config_name = "config_racing.json";
        break;
      case 2: config_name = "config_truck.json";
        break;
      case 3: config_name = "config_formula.json";
        break;
      default: config_name = "config_racing.json";
    }
    std::ifstream file;
    file.open(config_name);
    if (file) {
      file.close();
      return true;
    }
    file.close();
    return false;
  }
}
