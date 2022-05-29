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

void SET_AND_CHECK(Vibrato* vibrato, float freq, float width, float** inputBuffer, float** testBuffer, std::vector<float>& groundBuffer, int numSamples = 1000) {
	vibrato->setParam(Vibrato::Param_t::freqInHz, freq);
	vibrato->setParam(Vibrato::Param_t::widthInSec, width);
	vibrato->process(inputBuffer, testBuffer, numSamples);
	CHECK_ARRAY_CLOSE(testBuffer, groundBuffer, std::min<int>(numSamples, groundBuffer.size()), 1E-3);
}

void loadFile(const char* filePath, std::vector<float>& dataVec) {
	std::ifstream in_file;
	in_file.open(filePath);
	if (!in_file) {
		std::cout << "Error opening file " << filePath << std::endl;
		in_file.close();
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
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0.005) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 0) == Error_t::kNoError);

	}

	SECTION("Param getting") {
		vibrato->init(2, 44100);
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
	float** inputBuffer = nullptr;
	float** testBuffer = nullptr;
	const int numChannels = 1;
	const int numSamples = 1000;
	const float sampleRate = 44100;
	std::vector<float> groundBuffer;

	vibrato.reset(new Vibrato());
	vibrato->init(numChannels, sampleRate);
	inputBuffer = new float* [numChannels] {};
	testBuffer = new float* [numChannels] {};
	for (int c = 0; c < numChannels; c++) {
		inputBuffer[c] = new float[numSamples] {};
		testBuffer[c] = new float[numSamples] {};
	}

	SECTION("Zero Input") {
		for (int c = 0; c < numChannels; c++) {
			CVectorFloat::setZero(inputBuffer[c], numSamples);
		}
		groundBuffer.clear();
		SET_AND_CHECK(vibrato.get(), 1.0f, 0.005f, inputBuffer, testBuffer, groundBuffer);
	}

	SECTION("Sine Input") {
		for (int c = 0; c < numChannels; c++) {
			CSynthesis::generateSine(inputBuffer[c], 440, 44100, numSamples);
		}
		SECTION("Params1") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest1.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 10.0f, 0.005f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params2") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest2.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 10.0f, 0.002f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params3") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest3.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 3.0f, 0.001f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params4") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest4.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 7.0f, 0.004f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params5") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest5.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 1.0f, 0.00025f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params6") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest6.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 0.0f, 0.005f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("Params7") {
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SineTest7.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 4.0f, 0.0f, inputBuffer, testBuffer, groundBuffer);
		}
	}

	SECTION("Different Sample Rates") {
		vibrato->reset();
		SECTION("1") {
			for (int c = 0; c < numChannels; c++) {
				CSynthesis::generateSine(inputBuffer[c], 440, 1, numSamples);
			}
			vibrato->init(numChannels, 1.0f);
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SrTest1.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 4.0f, 0.0025f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("11025") {
			for (int c = 0; c < numChannels; c++) {
				CSynthesis::generateSine(inputBuffer[c], 440, 11025, numSamples);
			}
			vibrato->init(numChannels, 11025.0f);
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SrTest2.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 7.0f, 0.005f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("22050") {
			for (int c = 0; c < numChannels; c++) {
				CSynthesis::generateSine(inputBuffer[c], 440, 22050, numSamples);
			}
			vibrato->init(numChannels, 22050.0f);
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SrTest3.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 1.0f, 0.001f, inputBuffer, testBuffer, groundBuffer);
		}
		SECTION("48000") {
			for (int c = 0; c < numChannels; c++) {
				CSynthesis::generateSine(inputBuffer[c], 440, 48000, numSamples);
			}
			vibrato->init(numChannels, 48000.0f);
			loadFile("C:/Users/JohnK/Documents/ASE/AudioEffect_Vibrato/project/test/groundData/SrTest4.txt", groundBuffer);
			SET_AND_CHECK(vibrato.get(), 9.0f, 0.0001f, inputBuffer, testBuffer, groundBuffer);
		}
	}

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
