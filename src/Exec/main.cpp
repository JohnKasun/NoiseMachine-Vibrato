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

	CAudioFileIf* audioFileIn  = nullptr;
	CAudioFileIf* audioFileOut = nullptr;
	CAudioFileIf::FileSpec_t fileSpec;

	float** inputAudioData  = nullptr;
	float** outputAudioData = nullptr;
	unique_ptr<Vibrato> vibrato = nullptr;

	string inputFilePath{};
	string outputFilePath{};
	float widthInSec{};
	float freqInHz{};

	if (argc < 5){
		cout << "Incorrect number of arguments!" << endl;
		return -1;
	}

	inputFilePath  = argv[1];
	outputFilePath = argv[2];
	widthInSec	   = atof(argv[3]);
	freqInHz       = atof(argv[4]);

	// Open Audio Files
	CAudioFileIf::create(audioFileIn);
	audioFileIn->openFile(inputFilePath, CAudioFileIf::FileIoType_t::kFileRead);
	if (!audioFileIn->isOpen()) {
		std::cout << "Couldn't open input file..." << std::endl;
		CAudioFileIf::destroy(audioFileIn);
		return -1;
	}
	audioFileIn->getFileSpec(fileSpec);

	CAudioFileIf::create(audioFileOut);
	audioFileOut->openFile(outputFilePath, CAudioFileIf::FileIoType_t::kFileWrite, &fileSpec);
	if (!audioFileOut->isOpen()) {
		std::cout << "Couldn't open output file..." << std::endl;
		CAudioFileIf::destroy(audioFileOut);
		return -1;
	}

	// Create instance
	vibrato.reset(new Vibrato());

	// Initialize vibrato
	if (vibrato->init(fileSpec.iNumChannels, fileSpec.fSampleRateInHz) != Error_t::kNoError ||
		vibrato->setParam(Vibrato::Param_t::widthInSec, widthInSec)	   != Error_t::kNoError ||
		vibrato->setParam(Vibrato::Param_t::freqInHz, freqInHz)		   != Error_t::kNoError) {
		std::cout << "Invalid parameters..." << std::endl;
		CAudioFileIf::destroy(audioFileIn);
		CAudioFileIf::destroy(audioFileOut);
		vibrato.reset();
		return -1;
	}

	// Allocate memory
	inputAudioData  = new float* [fileSpec.iNumChannels]{};
	outputAudioData = new float* [fileSpec.iNumChannels]{};
	for (int c = 0; c < fileSpec.iNumChannels; c++){
		inputAudioData[c]  = new float[fileBlockSize]{};
		outputAudioData[c] = new float[fileBlockSize]{};
	}

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