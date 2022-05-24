#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "Vibrato.h"
#include "AudioFileIf.h"
#include "ErrorDef.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	static const int fileBlockSize = 1023;

	CAudioFileIf* audioFileIn = nullptr;
	CAudioFileIf* audioFileOut = nullptr;
	CAudioFileIf::FileSpec_t fileSpec;

	float** inputAudioData = nullptr;
	float** outputAudioData = nullptr;
	unique_ptr<Vibrato> vibrato = nullptr;

	string inputFilePath{};
	string outputFilePath{};
	float delayInSec{};
	float widthInSec{};
	float freqInHz{};

	if (argc < 6){
		cout << "Incorrect number of arguments!" << endl;
		return -1;
	}

	inputFilePath = argv[1];
	outputFilePath = argv[2];
	delayInSec = atof(argv[3]);
	widthInSec = atof(argv[4]);
	freqInHz   = atof(argv[5]);

	// Open Audio Files
	CAudioFileIf::create(audioFileIn);
	CAudioFileIf::create(audioFileOut);

	audioFileIn->openFile(inputFilePath, CAudioFileIf::FileIoType_t::kFileRead);
	audioFileIn->getFileSpec(fileSpec);

	audioFileOut->openFile(outputFilePath, CAudioFileIf::FileIoType_t::kFileWrite, &fileSpec);

	// Allocate memory
	inputAudioData = new float* [fileSpec.iNumChannels]{};
	outputAudioData = new float* [fileSpec.iNumChannels]{};
	for (int c = 0; c < fileSpec.iNumChannels; c++){
		inputAudioData[c] = new float[fileBlockSize]{};
		outputAudioData[c] = new float[fileBlockSize]{};
	}

	// Create instance
	vibrato.reset(new Vibrato());

	// Initialize
	vibrato->init(fileSpec.iNumChannels, fileSpec.fSampleRateInHz);
	vibrato->setParam(Vibrato::Param_t::widthInSec, 0.001f);
	vibrato->setParam(Vibrato::Param_t::freqInHz, 5.0f);

	// Process
	long long iNumFrames = fileBlockSize;
	while (!audioFileIn->isEof()){
		audioFileIn->readData(inputAudioData, iNumFrames);
		vibrato->process(inputAudioData, outputAudioData, iNumFrames);
		audioFileOut->writeData(outputAudioData, iNumFrames);
	}

	// Clean-up
	for (int c = 0; c < fileSpec.iNumChannels; c++) {
		delete[] inputAudioData[c];
		delete[] outputAudioData[c];
	}
	delete[] inputAudioData;
	delete[] outputAudioData;

	CAudioFileIf::destroy(audioFileOut);
	CAudioFileIf::destroy(audioFileOut);
	vibrato.reset();

	return 0;
}