#pragma once
#include "vjoy.h"
#include "ffbsize.h"
#include <Windows.h>

class ForceFeedBack{
public:
	// Convert Packet type to String
	BOOL packet_type2_str(FFBPType Type, LPTSTR OutStr);
	// Convert Effect type to String
	BOOL effect_type2_str(FFBEType Type, LPTSTR OutStr);
	// Convert PID Device Control to String
	BOOL dev_ctrl2_str(FFB_CTRL Ctrl, LPTSTR OutStr);
	// Convert Effect operation to string
	BOOL effect_op_str(FFBOP Op, LPTSTR OutStr);
	// Polar values (0x00-0xFF) to Degrees (0-360)
	int polar_2deg(BYTE Polar);
	// Convert range 0x00-0xFF to 0%-100%
	int byte2_percent(BYTE InByte);
	// Convert One-Byte 2's complement input to integer
	int twos_comp_byte2_int(BYTE in);
	// Convert One-Byte 2's complement input to integer
	int twos_comp_word2_int(WORD in);
	FFBSIZE get_ffb_size() { return m_ffbSize; };
	void CALLBACK ffb_to_v_joy(PVOID data, PVOID userData);
private:
	LPCTSTR m_ffbEffectName[13] = { "NONE", "Constant Force", "Ramp", "Square", "Sine", "Triangle", "Sawtooth Up",
		"Sawtooth Down", "Spring", "Damper", "Inertia", "Friction", "Custom Force" };
	FFBSIZE m_ffbSize;
};