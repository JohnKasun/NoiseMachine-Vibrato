#include "Vibrato.h"

Vibrato::Vibrato() {
	mParamRanges[widthInSec][0] = 0.0f;
	mParamRanges[widthInSec][1] = 0.005f;
	mParamRanges[freqInHz][0] = 0.0f;
	mParamRanges[freqInHz][1] = 10.0f;

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
	mLfo.reset(new Lfo(mSampleRate));
	for (int c = 0; c < mNumChannels; c++) {
		mDelayLine.emplace_back(new CRingBuffer<float>(2 + std::round(mParamRanges[widthInSec][1] * mSampleRate) * 3));
	}
	mIsInitialized = true;
	return Error_t::kNoError;
}

Error_t Vibrato::reset(){
	if (mIsInitialized) {
		mNumChannels = 0;
		mSampleRate = 0.0f;
		for (int param = 0; param < numParams; param++)
			mParamValues[param] = mParamRanges[param][0];
		mLfo.reset();
		mDelayLine.clear();
		mIsInitialized = false;
	}
	return Error_t::kNoError;
}

Error_t Vibrato::setParam(Param_t param, float value){
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;

	if (!isInParamRange(param, value))
		return Error_t::kFunctionInvalidArgsError;

	switch (param) {
	case freqInHz:
		mLfo->setParam(Lfo::Param_t::freqInHz, value);
		break;
	case widthInSec:
		int widthInSamp = std::round(value * mSampleRate);
		mLfo->setParam(Lfo::Param_t::amplitude, -1 * widthInSamp);
		for (int c = 0; c < mNumChannels; c++) {
			mDelayLine[c]->setWriteIdx(2 + widthInSamp * 3);
			mDelayLine[c]->setReadIdx(mDelayLine[c]->getWriteIdx() - widthInSamp);
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

	for (int i = 0; i < numFrames; i++) {
		float index = mLfo->process();
		for (int c = 0; c < mNumChannels; c++) {
			mDelayLine[c]->putPostInc(inputBuffer[c][i]);
			outputBuffer[c][i] = mDelayLine[c]->getPostInc(index);
		}
	}
	return Error_t::kNoError;
}

bool Vibrato::isInParamRange(Param_t param, float value) const {
	return (mParamRanges[param][0] <= value && value <= mParamRanges[param][1]);
}
