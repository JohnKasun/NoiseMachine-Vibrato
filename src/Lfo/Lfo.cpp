#include "Lfo.h"

Lfo::Lfo(int numChannels, float sampleRate){

	mParamRanges[amplitude][0] = 0.0f;
	mParamRanges[amplitude][1] = 1.0f;
	mParamRanges[freqInHz][0] = 0.0f;
	mParamRanges[freqInHz][1] = 5.0f;

	for (int param = 0; param < numParams; param++) {
		mParamValues[param] = mParamRanges[param][0];
	}

	mNumChannels = numChannels;
	mSampleRate = sampleRate;

	for (int i = 0; i < bufferSize; i++) {
		buffer.putPostInc(sinf(2.0f * M_PI / bufferSize * i));
	}
}

Lfo::~Lfo(){

}

Error_t Lfo::setParam(Param_t param, float value){
	if (!isInParamRange)
}

float Lfo::getParam(Param_t param) const{
	return mParamValues[param];
}

float Lfo::process(){
	return buffer.getPostInc();
}

bool Lfo::isInParamRange(Param_t param, float value) const{
	return (mParamRanges[param][0] <= value && value <= mParamRanges[param][1]);
}
