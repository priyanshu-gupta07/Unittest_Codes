/*
 * Copyright 2026, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */
#include <StopWatch.h>
#include <SupportDefs.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>

class BStopWatchTest : public CppUnit::TestCase {
public:
	BStopWatchTest(std::string name)
		:
		CppUnit::TestCase(name)
	{
	}

	void TestNameNull()
	{
		BStopWatch sw(NULL, true);
		CPPUNIT_ASSERT(strcmp(sw.Name(), "") == 0);
	}

	void TestNameValid()
	{
		BStopWatch sw("mywatch", true);
		CPPUNIT_ASSERT(strcmp(sw.Name(), "mywatch") == 0);
	}

	void TestElapsedTimeBasic()
	{
		BStopWatch sw("et", true);
		bigtime_t t1 = sw.ElapsedTime();
		usleep(10000); // 10ms
		bigtime_t t2 = sw.ElapsedTime();
		CPPUNIT_ASSERT(t2 > t1);
	}

	void TestSuspendResume()
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

	void TestLap()
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

	void TestLapWhileSuspended()
	{
		BStopWatch sw("lap2", true);
		sw.Suspend();
		CPPUNIT_ASSERT_EQUAL((bigtime_t)0, sw.Lap());
	}

	void TestReset()
	{
		BStopWatch sw("reset", true);
		usleep(5000);
		sw.Lap();
		sw.Reset();
		CPPUNIT_ASSERT(sw.ElapsedTime() < 10000);
		CPPUNIT_ASSERT(sw.Lap() < 10000);
	}

	void TestMultipleSuspendResume()
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

	static CppUnit::Test* Suite()
	{
		CppUnit::TestSuite* suite = new CppUnit::TestSuite("BStopWatchTest");
		suite->addTest(
			new CppUnit::TestCaller<BStopWatchTest>("TestNameNull", &BStopWatchTest::TestNameNull));
		suite->addTest(new CppUnit::TestCaller<BStopWatchTest>("TestNameValid",
			&BStopWatchTest::TestNameValid));
		suite->addTest(new CppUnit::TestCaller<BStopWatchTest>("TestElapsedTimeBasic",
			&BStopWatchTest::TestElapsedTimeBasic));
		suite->addTest(new CppUnit::TestCaller<BStopWatchTest>("TestSuspendResume",
			&BStopWatchTest::TestSuspendResume));
		suite->addTest(
			new CppUnit::TestCaller<BStopWatchTest>("TestLap", &BStopWatchTest::TestLap));
		suite->addTest(new CppUnit::TestCaller<BStopWatchTest>("TestLapWhileSuspended",
			&BStopWatchTest::TestLapWhileSuspended));
		suite->addTest(
			new CppUnit::TestCaller<BStopWatchTest>("TestReset", &BStopWatchTest::TestReset));
		suite->addTest(new CppUnit::TestCaller<BStopWatchTest>("TestMultipleSuspendResume",
			&BStopWatchTest::TestMultipleSuspendResume));
		return suite;
	}
};

// To use: add BStopWatchTest::Suite() to your test runner
