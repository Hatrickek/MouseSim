#pragma once
#include <string>
struct FFBSIZE {
private:
	std::string _effectTypeStr;
	int _magnitudeVar;
	int _directionVar;
	int _offsetVar;
	int _periodVar;
	int _durationVar;
public:
	std::string get_effect_type() { return _effectTypeStr; }
	int get_magnitude() const { return _magnitudeVar; }
	int get_direction() const { return _directionVar; }
	int get_offset() const { return _offsetVar; }
	int get_period() const { return _periodVar; }
	int get_duration() const { return _durationVar; }
	void set_effect_type(const std::string& type) { if (type != "NULL") { _effectTypeStr = type; } }
	void set_magnitude(int size) { if (size != NULL) { _magnitudeVar = size; } }
	void set_direction(int size) { if (size != NULL) { _directionVar = size; } }
	void set_offset(int size) { if (size != NULL) { _offsetVar = size; } }
	void set_period(int size) { if (size != NULL) { _periodVar = size; } }
	void set_duration(int size) { if (size != NULL) { _durationVar = size; } }
};