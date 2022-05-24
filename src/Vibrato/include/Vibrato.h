#pragma once

#include <vector>
#include <memory>

#include "ErrorDef.h"
#include "Util.h"
#include "Lfo.h"

class Vibrato{

public:
	enum Param_t {
		delayInSec,
		widthInSec,
		freqInHz,

		numParams
	};

	Vibrato();
	~Vibrato();

	Error_t init(int numChannels, float sampleRate);
	Error_t reset();

	Error_t setParam(Param_t param, float value);
	float getParam(Param_t param) const;

	Error_t process(float** inputBuffer, float** outputBuffer, int numFrames);
private:
	bool isInParamRange(Param_t param, float value) const;

	float mParamValues[numParams]{};
	float mParamRanges[numParams][2]{};

	int mNumChannels = 2;
	float mSampleRate = 44100.0f;
	std::vector<std::unique_ptr<Lfo>> mLfo;
};
