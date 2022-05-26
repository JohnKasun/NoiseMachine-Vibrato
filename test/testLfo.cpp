#pragma once

#include "catch.hpp"
#include "Lfo.h"

void CHECK_ARRAY_CLOSE_1(float* buff1, float* buff2, int numSamples, float tolerance) {
	for (int i = 0; i < numSamples; i++) {
		REQUIRE(abs(buff1[i] - buff2[i]) <= tolerance);
	}
}

TEST_CASE("[Lfo] Correct Output") {
	std::unique_ptr<float> outBuffTest = nullptr;
	std::unique_ptr<float> outBuffGrd = nullptr;
	std::unique_ptr<Lfo> lfo = nullptr;

	int numSamples = 100;
	float sampleRate = 44100.0f;
	float freqInHz = 1.0f;
	float amplitude = 1.0f;

	outBuffTest.reset(new float[numSamples] {});
	outBuffGrd.reset(new float[numSamples] {});
	lfo.reset(new Lfo(sampleRate));

	CSynthesis::generateSine(outBuffGrd.get(), freqInHz, sampleRate, numSamples, amplitude);
	lfo->setParam(Lfo::Param_t::freqInHz, freqInHz);
	lfo->setParam(Lfo::Param_t::amplitude, amplitude);
	for (int i = 0; i < numSamples; i++) {
		outBuffTest.get()[i] = lfo->process();
	}

	CHECK_ARRAY_CLOSE_1(outBuffTest.get(), outBuffGrd.get(), numSamples, 1E-4);

	lfo.reset();
	outBuffGrd.reset();
	outBuffTest.reset();
}