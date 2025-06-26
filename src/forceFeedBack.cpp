#include <Windows.h>
#include <string>
#include "forceFeedBack.h"

#include <tchar.h>

// Convert Packet type to String
BOOL ForceFeedBack::packet_type2_str(FFBPType type, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = LPTSTR("");

	switch (type)
	{
	case PT_EFFREP:
		Str = LPTSTR("Effect Report");
		break;
	case PT_ENVREP:
		Str = LPTSTR("Envelope Report");
		break;
	case PT_CONDREP:
		Str = LPTSTR("Condition Report");
		break;
	case PT_PRIDREP:
		Str = LPTSTR("Periodic Report");
		break;
	case PT_CONSTREP:
		Str = LPTSTR("Constant Force Report");
		break;
	case PT_RAMPREP:
		Str = LPTSTR("Ramp Force Report");
		break;
	case PT_CSTMREP:
		Str = LPTSTR("Custom Force Data Report");
		break;
	case PT_SMPLREP:
		Str = LPTSTR("Download Force Sample");
		break;
	case PT_EFOPREP:
		Str = LPTSTR("Effect Operation Report");
		break;
	case PT_BLKFRREP:
		Str = LPTSTR("PID Block Free Report");
		break;
	case PT_CTRLREP:
		Str = LPTSTR("PID Device Control");
		break;
	case PT_GAINREP:
		Str = LPTSTR("Device Gain Report");
		break;
	case PT_SETCREP:
		Str = LPTSTR("Set Custom Force Report");
		break;
	case PT_NEWEFREP:
		Str = LPTSTR("Create New Effect Report");
		break;
	case PT_BLKLDREP:
		Str = LPTSTR("Block Load Report");
		break;
	case PT_POOLREP:
		Str = LPTSTR("PID Pool Report");
		break;
	default:
		stat = FALSE;
		break;
	}

	if (stat)
		strcpy_s(outStr, 100, Str);

	return stat;
}

// Convert Effect type to String
BOOL ForceFeedBack::effect_type2_str(FFBEType type, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = LPTSTR("");

	switch (type)
	{
	case ET_NONE:
		stat = FALSE;
		break;
	case ET_CONST:
		Str = LPTSTR("Constant Force");
		break;
	case ET_RAMP:
		Str = LPTSTR("Ramp");
		break;
	case ET_SQR:
		Str = LPTSTR("Square");
		break;
	case ET_SINE:
		Str = LPTSTR("Sine");
		break;
	case ET_TRNGL:
		Str = LPTSTR("Triangle");
		break;
	case ET_STUP:
		Str = LPTSTR("Sawtooth Up");
		break;
	case ET_STDN:
		Str = LPTSTR("Sawtooth Down");
		break;
	case ET_SPRNG:
		Str = LPTSTR("Spring");
		break;
	case ET_DMPR:
		Str = LPTSTR("Damper");
		break;
	case ET_INRT:
		Str = LPTSTR("Inertia");
		break;
	case ET_FRCTN:
		Str = LPTSTR("Friction");
		break;
	case ET_CSTM:
		Str = LPTSTR("Custom Force");
		break;
	default:
		stat = FALSE;
		break;
	};

	if (stat)
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Convert PID Device Control to String
BOOL ForceFeedBack::dev_ctrl2_str(FFB_CTRL ctrl, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = LPTSTR("");

	switch (ctrl)
	{
	case CTRL_ENACT:
		Str = LPTSTR("Enable Actuators");
		break;
	case CTRL_DISACT:
		Str = LPTSTR("Disable Actuators");
		break;
	case CTRL_STOPALL:
		Str = LPTSTR("Stop All Effects");
		break;
	case CTRL_DEVRST:
		Str = LPTSTR("Device Reset");
		break;
	case CTRL_DEVPAUSE:
		Str = LPTSTR("Device Pause");
		break;
	case CTRL_DEVCONT:
		Str = LPTSTR("Device Continue");
		break;
	default:
		stat = FALSE;
		break;
	}
	if (stat)
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Convert Effect operation to string
BOOL ForceFeedBack::effect_op_str(FFBOP op, LPTSTR outStr)
{
	BOOL stat = TRUE;
	LPTSTR Str = LPTSTR("");

	switch (op)
	{
	case EFF_START:
		Str = LPTSTR("Effect Start");
		break;
	case EFF_SOLO:
		Str = LPTSTR("Effect Solo Start");
		break;
	case EFF_STOP:
		Str = LPTSTR("Effect Stop");
		break;
	default:
		stat = FALSE;
		break;
	}

	if (stat)
		_tcscpy_s(outStr, 100, Str);

	return stat;
}

// Polar values (0x00-0xFF) to Degrees (0-360)
int ForceFeedBack::polar_2deg(BYTE polar)
{
	return ((UINT)polar * 360) / 255;
}

// Convert range 0x00-0xFF to 0%-100%
int ForceFeedBack::byte2_percent(BYTE inByte)
{
	return ((UINT)inByte * 100) / 255;
}

// Convert One-Byte 2's complement input to integer
int ForceFeedBack::twos_comp_byte2_int(BYTE in)
{
	int tmp;
	BYTE inv = ~in;
	BOOL isNeg = in >> 7;
	if (isNeg)
	{
		tmp = (int)(inv);
		tmp = -1 * tmp;
		return tmp;
	}
	else
		return (int)in;
}

// Convert One-Byte 2's complement input to integer
int ForceFeedBack::twos_comp_word2_int(WORD in)
{
	int tmp;
	WORD inv = ~in;
	BOOL isNeg = in >> 15;
	if (isNeg)
	{
		tmp = (int)(inv);
		tmp = -1 * tmp;
		return tmp - 1;
	}
	else
		return (int)in;
}
// Convert Ffb Calls into FFBSIZE struct
void CALLBACK ForceFeedBack::ffb_to_v_joy(PVOID data, PVOID userData)
{
	/////// Packet Device ID, and Type Block Index (if exists)
#pragma region Packet Device ID, and Type Block Index
	int DeviceID, BlockIndex;
	FFBPType	Type;
	TCHAR	TypeStr[100];
#pragma endregion


	/////// Effect Report
#pragma region Effect Report
#pragma warning( push )
#pragma warning( disable : 4996 )
	FFB_EFF_CONST Effect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Const((FFB_DATA *)data, &Effect))
	{
		
	};
#pragma endregion
#pragma region PID Device Control
	FFB_CTRL	Control;
	TCHAR	CtrlStr[100];
	if (ERROR_SUCCESS == Ffb_h_DevCtrl((FFB_DATA *)data, &Control) && dev_ctrl2_str(Control, CtrlStr))
	{

	}
#pragma endregion
#pragma region Effect Operation
	FFB_EFF_OP	Operation;
	TCHAR	EffOpStr[100];
	if (ERROR_SUCCESS == Ffb_h_EffOp((FFB_DATA *)data, &Operation) && effect_op_str(Operation.EffectOp, EffOpStr))
	{

	};
#pragma endregion
#pragma region Global Device Gain
	BYTE Gain;
	if (ERROR_SUCCESS == Ffb_h_DevGain((FFB_DATA *)data, &Gain)) {

	}

#pragma endregion
#pragma region Condition
	FFB_EFF_COND Condition;
	if (ERROR_SUCCESS == Ffb_h_Eff_Cond((FFB_DATA *)data, &Condition))
	{

	}
#pragma endregion
#pragma region Envelope
	FFB_EFF_ENVLP Envelope;
	if (ERROR_SUCCESS == Ffb_h_Eff_Envlp((FFB_DATA *)data, &Envelope))
	{

	};

#pragma endregion
#pragma region Periodic
	FFB_EFF_PERIOD EffPrd;
	if (ERROR_SUCCESS == Ffb_h_Eff_Period((FFB_DATA *)data, &EffPrd))
	{
		m_ffbSize.set_effect_type("Period");
		m_ffbSize.set_magnitude(EffPrd.Magnitude);
		m_ffbSize.set_direction(Effect.DirX);
		m_ffbSize.set_offset(twos_comp_word2_int(static_cast<WORD>(EffPrd.Offset)));
		m_ffbSize.set_period(EffPrd.Period);
		m_ffbSize.set_duration(Effect.Duration);
	};
#pragma endregion

#pragma region Effect Type
	FFBEType EffectType;
	if (ERROR_SUCCESS == Ffb_h_EffNew((FFB_DATA *)data, &EffectType))
	{

	};

#pragma endregion

#pragma region Ramp Effect
	FFB_EFF_RAMP RampEffect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Ramp((FFB_DATA *)data, &RampEffect))
	{

	};

#pragma endregion

#pragma region Constant Effect
	FFB_EFF_CONSTANT ConstantEffect;
	if (ERROR_SUCCESS == Ffb_h_Eff_Constant((FFB_DATA *)data, &ConstantEffect))
	{

		m_ffbSize.set_effect_type("Constant");
		m_ffbSize.set_magnitude((INT16)ConstantEffect.Magnitude);
		m_ffbSize.set_direction(polar_2deg(Effect.Direction));
		m_ffbSize.set_offset(0);
		m_ffbSize.set_period(0);
		m_ffbSize.set_duration(Effect.Duration);
	};
#pragma endregion
}