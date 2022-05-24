#pragma once

#include "catch.hpp"
#include "Vibrato.h"
#include "Synthesis.h"

TEST_CASE("Error checking", "[Vibrato]") {
	std::unique_ptr<Vibrato> vibrato = nullptr;
	vibrato.reset(new Vibrato());

	SECTION("Init") {
		REQUIRE(vibrato->init(-1, 44100) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(1, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(2, 48000) == Error_t::kNoError);
		REQUIRE(vibrato->init(-234235, -2342) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(1, 22050) == Error_t::kNoError);
		REQUIRE(vibrato->init(0, 0) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->init(0, 1) == Error_t::kNoError);
	}

	SECTION("Param setting") {
		REQUIRE(vibrato->setParam(Vibrato::Param_t::delayInSec, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, -1) == Error_t::kFunctionInvalidArgsError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::delayInSec, 0) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::delayInSec, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 1) == Error_t::kNoError);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::freqInHz, 0) == Error_t::kNoError);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 0.0f);
		vibrato->setParam(Vibrato::Param_t::delayInSec, 0.25);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::widthInSec, 0.5) == Error_t::kFunctionInvalidArgsError);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 0.1);
		REQUIRE(vibrato->setParam(Vibrato::Param_t::delayInSec, 0.05) == Error_t::kFunctionInvalidArgsError);
	}

	SECTION("Param getting") {
		vibrato->init(2, 44100);
		vibrato->setParam(Vibrato::Param_t::delayInSec, 0.5f);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 0.25f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::delayInSec) == 0.5f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);

		// do invalid param setting
		vibrato->setParam(Vibrato::Param_t::delayInSec, -234.0f);
		vibrato->setParam(Vibrato::Param_t::widthInSec, 444.0f);
		vibrato->setParam(Vibrato::Param_t::freqInHz, -1.0f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::delayInSec) == 0.5f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::widthInSec) == 0.25f);
		REQUIRE(vibrato->getParam(Vibrato::Param_t::freqInHz) == 0.25f);
	}

	vibrato.reset();
}
