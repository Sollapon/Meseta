#pragma once

#include <afx.h>
#include <vector>


#include "framework.h"
#include "xinput.h"


class PAD_INFO
{
public:
	static const unsigned int buttonNum = 2;

	const WORD xinput_button[16] = {
		0,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_BACK,
		XINPUT_GAMEPAD_START,
		XINPUT_GAMEPAD_LEFT_THUMB,
		XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
		0,
	};


	PAD_INFO()
		: padBit(0)
	{
		clear();
	}
	void clear()
	{
		padBit = 0;
		for (size_t i = 0; i < buttonNum; i++)
		{
			checkBit[i] = 0;
		}
	}
	WORD padBit;
	WORD checkBit[buttonNum];
	void setBit(int idx, CString str)
	{
		str.Replace(L" ", L"");

		std::vector<CString> NUM;
		for (;;)
		{
			int pos = str.Find(L"+");
			if (pos < 0)
			{
				NUM.push_back(str);
				break;
			}
			NUM.push_back(str.Left(pos));
			str.Delete(0, pos + 1);
		}

		for (size_t i = 0; i < NUM.size(); i++)
		{
			int b = _ttoi(NUM[i]);

			if (b > 0 && b < 16)
				checkBit[idx] |= xinput_button[b];
		}
	}

	bool getTrigger(int idx, DWORD button)
	{
		bool chek1 = (button & checkBit[idx]) == checkBit[idx];// ¡‰ñ‰Ÿ‚³‚ê‚Ä‚¢‚é
		bool check2 = (padBit & checkBit[idx]) == checkBit[idx]; // ‘O‰ñ‚à‰Ÿ‚³‚ê‚Ä‚¢‚é

		return (chek1 && !check2);
	}
};
