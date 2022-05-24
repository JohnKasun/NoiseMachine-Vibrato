#include "Vibrato.h"

Vibrato::Vibrato() {
	mParamRanges[widthInSec][0] = 0.0f;
	mParamRanges[widthInSec][1] = 10.0f;
	mParamRanges[freqInHz][0] = 0.0f;
	mParamRanges[freqInHz][1] = 1.0f;

	for (int param = 0; param < numParams; param++)
		mParamValues[param] = mParamRanges[param][0];
}

Vibrato::~Vibrato(){
	reset();
}

Error_t Vibrato::init(int numChannels, float sampleRate){
	if (mIsInitialized)
		return Error_t::kMemError;

	if (numChannels < 0 || sampleRate <= 0.0f)
		return Error_t::kFunctionInvalidArgsError;

	mNumChannels = numChannels;
	mSampleRate = sampleRate;
	for (int c = 0; c < mNumChannels; c++) {
		mLfo.emplace_back(new Lfo(mSampleRate));
		mDelayLine.emplace_back(new CRingBuffer<float>(2 + 3 * mParamRanges[widthInSec][1] * mSampleRate));
	}
	mIsInitialized = true;
	return Error_t::kNoError;
}

Error_t Vibrato::reset(){
	if (mIsInitialized) {
		mNumChannels = 0;
		mSampleRate = 0.0f;
		mLfo.clear();
		mDelayLine.clear();
		mIsInitialized = false;
	}
	return Error_t::kNoError;
}

Error_t Vibrato::setParam(Param_t param, float value){
	if (!isInParamRange(param, value))
		return Error_t::kFunctionInvalidArgsError;

	if (param == Param_t::widthInSec) {
		for (int c = 0; c < mNumChannels; c++) {
			mDelayLine[c]->setWriteIdx(2 + 3 * value * mSampleRate);
		}
	}
	mParamValues[param] = value;
	return Error_t::kNoError;
}

float Vibrato::getParam(Param_t param) const{
	return mParamValues[param];
}

Error_t Vibrato::process(float** inputBuffer, float** outputBuffer, int numFrames){
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;

	for (int c = 0; c < mNumChannels; c++) {
		for (int i = 0; i < numFrames; i++) {
			float mod = mLfo[c]->process();
			float tap = 1 + (mParamValues[widthInSec] + mod * mParamValues[widthInSec]) * mSampleRate;
			mDelayLine[c]->putPostInc(inputBuffer[c][i]);
			outputBuffer[c][i] = mDelayLine[c]->get(tap);
		}
	}
	return Error_t::kNoError;
}

bool Vibrato::isInParamRange(Param_t param, float value) const {
	return (mParamRanges[param][0] <= value && value <= mParamRanges[param][1]);
}
