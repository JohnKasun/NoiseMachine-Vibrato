#include "Lfo.h"

Lfo::Lfo(float sampleRate){

	for (int param = 0; param < numParams; param++) {
		mParamValues[param] = 0.0f;
	}

	mSampleRate = sampleRate;

	for (int i = 0; i < mBuffer.getLength(); i++) {
		mBuffer.putPostInc(sinf(2.0f * M_PI / mBuffer.getLength() * i));
	}
}

void Lfo::setParam(Param_t param, float value){
	mParamValues[param] = value;
}

float Lfo::getParam(Param_t param) const{
	return mParamValues[param];
}

float Lfo::process(){
	float currentValue = mBuffer.get(mCurrentIndex);
	mCurrentIndex += mParamValues[freqInHz] / mSampleRate * mBuffer.getLength();
	return mParamValues[amplitude] * currentValue;
}
