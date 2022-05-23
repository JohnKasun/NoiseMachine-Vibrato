#pragma once

#include "ErrorDef.h"
#include "Util.h"
#include "Synthesis.h"
#include "RingBuffer.h"

class Lfo
{
public:
	enum Param_t {
		amplitude,
		freqInHz,

		numParams
	};

	Lfo(float sampleRate);
	~Lfo() = default;

	Error_t setParam(Param_t param, float value);
	float getParam(Param_t param) const;

	float process();
private:
	CRingBuffer<float> mBuffer{ 1 << 9 };

	bool isInParamRange(Param_t param, float value) const;
	float mParamValues[numParams]{};
	float mParamRanges[numParams][2]{};

	float mSampleRate = 44100.0f;
	float mCurrentIndex = 0.0f;
};