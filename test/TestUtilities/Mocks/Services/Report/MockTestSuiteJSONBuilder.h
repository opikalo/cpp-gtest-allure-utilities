#pragma once

#include "GTestAllureUtilities/Model/TestSuite.h"
#include "GTestAllureUtilities/Services/Report/ITestSuiteJSONBuilder.h"


namespace systelab { namespace gtest_allure { namespace test_utility {

	class MockTestSuiteJSONBuilder : public service::ITestSuiteJSONBuilder
	{
	public:
		MockTestSuiteJSONBuilder();
		virtual ~MockTestSuiteJSONBuilder();

		MOCK_CONST_METHOD1(buildJSONFiles, void(const model::TestSuite&));
	};

}}}

