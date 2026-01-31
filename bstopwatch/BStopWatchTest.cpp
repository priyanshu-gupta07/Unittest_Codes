/*
 * Copyright 2026, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */


#include "BStopWatchTest.h"

#include <StopWatch.h>

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>

#include <cstring>
#include <unistd.h>


class BStopWatchTest : public BTestCase {
public:
	BStopWatchTest(std::string name = "");

	void TestNameNull();
	void TestNameValid();
	void TestElapsedTimeBasic();
	void TestSuspendResume();
	void TestLap();
	void TestLapWhileSuspended();
	void TestReset();
	void TestMultipleSuspendResume();
};


BStopWatchTest::BStopWatchTest(std::string name)
	:
	BTestCase(name)
{
}


void
BStopWatchTest::TestNameNull()
{
	BStopWatch sw(NULL, true);
	CPPUNIT_ASSERT(strcmp(sw.Name(), "") == 0);
}


void
BStopWatchTest::TestNameValid()
{
	BStopWatch sw("mywatch", true);
	CPPUNIT_ASSERT(strcmp(sw.Name(), "mywatch") == 0);
}


void
BStopWatchTest::TestElapsedTimeBasic()
{
	BStopWatch sw("et", true);
	bigtime_t t1 = sw.ElapsedTime();
	usleep(10000); // 10ms
	bigtime_t t2 = sw.ElapsedTime();
	CPPUNIT_ASSERT(t2 > t1);
}


void
BStopWatchTest::TestSuspendResume()
{
	BStopWatch sw("sr", true);
	usleep(5000);
	sw.Suspend();
	bigtime_t t1 = sw.ElapsedTime();
	usleep(10000);
	bigtime_t t2 = sw.ElapsedTime();
	CPPUNIT_ASSERT_EQUAL(t1, t2);
	sw.Resume();
	usleep(5000);
	bigtime_t t3 = sw.ElapsedTime();
	CPPUNIT_ASSERT(t3 > t2);
}


void
BStopWatchTest::TestLap()
{
	BStopWatch sw("lap", true);
	usleep(2000);
	bigtime_t l1 = sw.Lap();
	usleep(2000);
	bigtime_t l2 = sw.Lap();
	CPPUNIT_ASSERT(l2 > l1);
	// Test more than 10 laps
	for (int i = 0; i < 12; i++)
		sw.Lap();
	CPPUNIT_ASSERT(sw.Lap() > 0);
}


void
BStopWatchTest::TestLapWhileSuspended()
{
	BStopWatch sw("lap2", true);
	sw.Suspend();
	CPPUNIT_ASSERT_EQUAL((bigtime_t)0, sw.Lap());
}


void
BStopWatchTest::TestReset()
{
	BStopWatch sw("reset", true);
	usleep(5000);
	sw.Lap();
	sw.Reset();
	CPPUNIT_ASSERT(sw.ElapsedTime() < 10000);
	CPPUNIT_ASSERT(sw.Lap() < 10000);
}


void
BStopWatchTest::TestMultipleSuspendResume()
{
	BStopWatch sw("multi", true);
	usleep(2000);
	sw.Suspend();
	usleep(2000);
	sw.Resume();
	usleep(2000);
	sw.Suspend();
	usleep(2000);
	sw.Resume();
	usleep(2000);
	CPPUNIT_ASSERT(sw.ElapsedTime() > 0);
}


CppUnit::Test*
BStopWatchTestSuite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("BStopWatch");

	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestNameNull",
		&BStopWatchTest::TestNameNull));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestNameValid",
		&BStopWatchTest::TestNameValid));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestElapsedTimeBasic",
		&BStopWatchTest::TestElapsedTimeBasic));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestSuspendResume",
		&BStopWatchTest::TestSuspendResume));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestLap",
		&BStopWatchTest::TestLap));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestLapWhileSuspended",
		&BStopWatchTest::TestLapWhileSuspended));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestReset",
		&BStopWatchTest::TestReset));
	suite->addTest(new CppUnit::TestCaller<BStopWatchTest>(
		"BStopWatchTest::TestMultipleSuspendResume",
		&BStopWatchTest::TestMultipleSuspendResume));

	return suite;
}
