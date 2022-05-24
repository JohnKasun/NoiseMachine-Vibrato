#include "Vibrato.h"

Vibrato::Vibrato() {
	mParamRanges[delayInSec][0] = 0.0f;
	mParamRanges[delayInSec][1] = 1.0f;
	mParamRanges[widthInSec][0] = 0.0f;
	mParamRanges[widthInSec][1] = 1.0f;
	mParamRanges[freqInHz][0] = 0.0f;
	mParamRanges[freqInHz][1] = 1.0f;

	for (int param = 0; param < numParams; param++)
		mParamValues[param] = mParamRanges[param][0];

	for (int c = 0; c < mNumChannels; c++) {
		mLfo.emplace_back(new Lfo(mSampleRate));
	}
}

Vibrato::~Vibrato(){
	reset();
}

Error_t Vibrato::init(int numChannels, float sampleRate){
	if (numChannels < 0 || sampleRate <= 0.0f)
		return Error_t::kFunctionInvalidArgsError;

	reset();

	mNumChannels = numChannels;
	mSampleRate = sampleRate;
	mLfo.clear();
	for (int c = 0; c < mNumChannels; c++) {
		mLfo.emplace_back(new Lfo(mSampleRate));
	}
	return Error_t::kNoError;
}

Error_t Vibrato::reset(){
	mNumChannels = 2;
	mSampleRate = 44100.0f;
	mLfo.clear();
	for (int c = 0; c < mNumChannels; c++) {
		mLfo.emplace_back(new Lfo(mSampleRate));
	}
	return Error_t::kNoError;
}

Error_t Vibrato::setParam(Param_t param, float value){
	if (!isInParamRange(param, value))
		return Error_t::kFunctionInvalidArgsError;

	mParamValues[param] = value;
	return Error_t::kNoError;
}

float Vibrato::getParam(Param_t param) const{
	return mParamValues[param];
}

Error_t Vibrato::process(float** inputBuffer, float** outputBuffer, int numFrames){
	return Error_t();
}

bool Vibrato::isInParamRange(Param_t param, float value) const {
	return (mParamRanges[param][0] <= value && value <= mParamRanges[param][1]);
}
