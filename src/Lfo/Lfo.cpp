#include "Lfo.h"

Lfo::Lfo(float sampleRate){

	mParamRanges[amplitude][0] = 0.0f;
	mParamRanges[amplitude][1] = 1.0f;
	mParamRanges[freqInHz][0] = 0.0f;
	mParamRanges[freqInHz][1] = 5.0f;

	for (int param = 0; param < numParams; param++) {
		mParamValues[param] = mParamRanges[param][0];
	}

	mSampleRate = sampleRate;

	for (int i = 0; i < mBuffer.getLength(); i++) {
		mBuffer.putPostInc(sinf(2.0f * M_PI / mBuffer.getLength() * i));
	}
}

Error_t Lfo::setParam(Param_t param, float value){
	if (!isInParamRange(param, value))
		return Error_t::kFunctionInvalidArgsError;

	mParamValues[param] = value;
	return Error_t::kNoError;
}

float Lfo::getParam(Param_t param) const{
	return mParamValues[param];
}

float Lfo::process(){
	float currentValue = mBuffer.get(mCurrentIndex);
	mCurrentIndex += mParamValues[freqInHz] / mSampleRate * mBuffer.getLength();
	return currentValue;
}

bool Lfo::isInParamRange(Param_t param, float value) const{
	return (mParamRanges[param][0] <= value && value <= mParamRanges[param][1]);
}
