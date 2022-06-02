#pragma once

#include <iostream>
#include <fstream>

#include "catch.hpp"
#include "Vibrato.h"
#include "Synthesis.h"
#include "Vector.h"

void CHECK_ARRAY_CLOSE(float* buff1, float* buff2, int numSamples, float tolerance) {
	for (int i = 0; i < numSamples; i++) {
		REQUIRE(abs(buff1[i] - buff2[i]) <= tolerance);
	}
}

void SET_AND_CHECK(Vibrato* vibrato, float freq, float width, float* inputBuffer, float* testBuffer, float* groundBuffer, int numSamples = 1000) {
	vibrato->setParam(Vibrato::Param_t::freqInHz, freq);
	vibrato->setParam(Vibrato::Param_t::widthInSec, width);
	vibrato->process(inputBuffer, testBuffer, numSamples);
	CHECK_ARRAY_CLOSE(testBuffer, groundBuffer, numSamples, 1E-3);
}

TEST_CASE("[Vibrato] Error checking") {
	std::unique_ptr<Vibrato> vibrato = nullptr;
	vibrato.reset(new Vibrato());

	SECTION("Init") {
		REQUIRE(vibrato->init(-1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(48000) == Error_t::kNoError);
		vibrato->reset();
		REQUIRE(vibrato->init(-2342) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(22050) == Error_t::kNoError);
		vibrato->reset();
		REQUIRE(vibrato->init(0) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(1) == Error_t::kNoError);
	}

	SECTION("Param setting") {
		vibrato->init(44100);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0.005) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 0) == Error_t::kNoError);

	}

	SECTION("Param getting") {
		vibrato->init(44100);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 0.00025f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.00025f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);

		// do invalid param setting
		vibrato->setParam(Vibrato::Param_t::widthInSec, 444.0f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, -1.0f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.00025f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);
	}

	vibrato.reset();
}

TEST_CASE("[Vibrato] Correct Output") {
	std::unique_ptr<Vibrato> vibrato = nullptr;
	float* inputBuffer = nullptr;
	float* testBuffer = nullptr;
	float* groundBuffer = nullptr;
	const int numSamples = 1000;
	const float sampleRate = 44100;

	vibrato.reset(new Vibrato());
	vibrato->init(sampleRate);
	inputBuffer = new float[numSamples] {};
	testBuffer = new float[numSamples] {};

	SECTION("Zero Input") {
		CVectorFloat::setZero(inputBuffer, numSamples);
		CVectorFloat::setZero(groundBuffer, numSamples);
		SET_AND_CHECK(vibrato.get(), 1.0f, 0.005f, inputBuffer, testBuffer, groundBuffer);
	}

	SECTION("Zero Depth") {
		CSynthesis::generateSine(inputBuffer, 440, 44100, numSamples);
		CSynthesis::generateSine(testBuffer, 440, 44100, numSamples);
	}

	SECTION("DC") {

	}

	CVectorFloat::setZero(groundBuffer, numSamples);
	CVectorFloat::setZero(testBuffer, numSamples);

	vibrato.reset();
	delete[] inputBuffer;
	delete[] testBuffer;
	delete[] groundBuffer;
	inputBuffer = nullptr;
	testBuffer = nullptr;
	groundBuffer = nullptr;
}
