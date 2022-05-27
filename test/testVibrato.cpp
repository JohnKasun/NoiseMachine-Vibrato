#pragma once

#include <iostream>
#include <fstream>

#include "catch.hpp"
#include "Vibrato.h"
#include "Synthesis.h"
#include "Vector.h"

void CHECK_ARRAY_CLOSE(float** buff1, std::vector<float>& buff2, int numSamples, float tolerance) {
	for (int i = 0; i < numSamples; i++) {
		REQUIRE(abs(buff1[0][i] - buff2[i]) <= tolerance);
	}
}

void loadFile(const char* filePath, std::vector<float>& dataVec) {
	std::ifstream in_file;
	in_file.open(filePath);
	if (!in_file) {
		std::cout << "Error opening file " << filePath << std::endl;
		return;
	}
	float value{};
	while (in_file >> value) {
		dataVec.push_back(value);
	}
	in_file.close();
}

TEST_CASE("[Vibrato] Error checking") {
	std::unique_ptr<Vibrato> vibrato = nullptr;
	vibrato.reset(new Vibrato());

	SECTION("Init") {
		REQUIRE(vibrato->init(-1, 44100) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(1, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(2, 48000) == Error_t::kNoError);
		vibrato->reset();
		REQUIRE(vibrato->init(-234235, -2342) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(1, 22050) == Error_t::kNoError);
		vibrato->reset();
		REQUIRE(vibrato->init(0, 0) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(0, 1) == Error_t::kNoError);
	}

	SECTION("Param setting") {
		vibrato->init(2, 44100);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0.5) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 0) == Error_t::kNoError);

	}

	SECTION("Param getting") {
		vibrato->init(2, 44100);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 0.25f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);

		// do invalid param setting
		vibrato->setParam(Vibrato::Param_t::widthInSec, 444.0f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, -1.0f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);
	}

	vibrato.reset();
}

TEST_CASE("[Vibrato] Correct Output") {
	std::unique_ptr<Vibrato> vibrato = nullptr;
	float** inputBuffer = nullptr;
	float** testBuffer = nullptr;
	const int numChannels = 1;
	const int numSamples = 1000;
	const float sampleRate = 44100;
	std::vector<float> groundBuffer;
	float freq{};
	float width{};

	vibrato.reset(new Vibrato());
	vibrato->init(numChannels, sampleRate);
	inputBuffer = new float* [numChannels] {};
	testBuffer = new float* [numChannels] {};
	for (int c = 0; c < numChannels; c++) {
		inputBuffer[c] = new float[numSamples] {};
		testBuffer[c] = new float[numSamples] {};
	}

	SECTION("Increment") {
		for (int c = 0; c < numChannels; c++) {
			for (int i = 0; i < numSamples; i++) {
				inputBuffer[c][i] = i + 1;
			}
		}
		SECTION("Params1") {
			loadFile("C:/Users/JohnK/Documents/ASE/Vibrato/project/test/IncrementTest1.txt", groundBuffer);
			freq = 5.0f;
			width = 0.0005f;
		}
		SECTION("Params2") {
			loadFile("C:/Users/JohnK/Documents/ASE/Vibrato/project/test/IncrementTest2.txt", groundBuffer);
			freq = 1.0f;
			width = 0.005f;
		}
		SECTION("Params3") {
			loadFile("C:/Users/JohnK/Documents/ASE/Vibrato/project/test/IncrementTest3.txt", groundBuffer);
			freq = 3.0f;
			width = 0.001f;
		}
	}

	vibrato->setParam(Vibrato::Param_t::freqInHz, freq);
	vibrato->setParam(Vibrato::Param_t::widthInSec, width);
	vibrato->process(inputBuffer, testBuffer, numSamples);
	CHECK_ARRAY_CLOSE(testBuffer, groundBuffer, std::min<int>(numSamples, groundBuffer.size()), 1E-3);
	groundBuffer.clear();
	for (int c = 0; c < numChannels; c++)
		CVectorFloat::setZero(testBuffer[c], numSamples);

	vibrato.reset();
	for (int c = 0; c < numChannels; c++) {
		delete[] inputBuffer[c];
		delete[] testBuffer[c];
	}
	delete[] inputBuffer;
	delete[] testBuffer;
	inputBuffer = nullptr;
	testBuffer = nullptr;
}
