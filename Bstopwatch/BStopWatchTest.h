/*
 * Copyright 2026, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef B_STOPWATCH_TEST_H
#define B_STOPWATCH_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

class BStopWatchTest : public CppUnit::TestCase {
public:
	BStopWatchTest(std::string name);

	void								TestNameNull();
	void								TestNameValid();
	void								TestElapsedTimeBasic();
	void								TestSuspendResume();
	void								TestLap();
	void								TestLapWhileSuspended();
	void								TestReset();
	void								TestMultipleSuspendResume();

	static CppUnit::Test* Suite();
};

#endif // B_STOPWATCH_TEST_H
