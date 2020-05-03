#include "stdafx.h"
#include "GTestAllureUtilities/Services/Report/TestCaseJSONSerializer.h"

#include "GTestAllureUtilities/Model/TestCase.h"
#include "RapidJSONAdapter/JSONAdapter.h"
#include "JSONAdapterTestUtilities/JSONAdapterUtilities.h"


using namespace testing;
using namespace systelab::json::test_utility;
using namespace systelab::gtest_allure;

namespace systelab { namespace gtest_allure { namespace unit_test {

	class TestCaseJSONSerializerTest : public Test
	{
	protected:
		service::TestCaseJSONSerializer m_service;
		systelab::json::rapidjson::JSONAdapter m_jsonAdapter;
	};


	TEST_F(TestCaseJSONSerializerTest, testSerializeForTCWithoutActions)
	{
		model::TestCase testCase;
		testCase.setUUID("1bf09e3a-cdcf-4994-8718-5476636194f1");
		testCase.setHistoryId("66245a162d0fe05f10625e4c234eb8f0");
		testCase.setName("Test case name");
		testCase.setDescription("Description of the test case");
		testCase.setStatus(model::Status::PASSED);
		testCase.setStage(model::Stage::SCHEDULED);
		testCase.setStart(125);
		testCase.setStop(250);

		std::string expectedSerializedTestCase =
			"{\n"
			"    \"uuid\": \"1bf09e3a-cdcf-4994-8718-5476636194f1\",\n"
			"    \"historyId\": \"66245a162d0fe05f10625e4c234eb8f0\",\n"
			"    \"name\": \"Test case name\",\n"
			"    \"description\": \"Description of the test case\",\n"
			"    \"status\": \"passed\",\n"
			"    \"stage\": \"scheduled\",\n"
			"    \"start\": 125,\n"
			"    \"stop\": 250\n"
			"}";

		std::string serializedTestCase = m_service.serialize(testCase);
		ASSERT_TRUE(compareJSONs(expectedSerializedTestCase, serializedTestCase, m_jsonAdapter));
	}

	TEST_F(TestCaseJSONSerializerTest, testSerializeForTCWithLinksAndLabels)
	{
		model::TestCase testCase;
		testCase.setName("Test case with links and labels");
		testCase.setStatus(model::Status::SKIPPED);
		testCase.setStage(model::Stage::PENDING);
		testCase.setStart(111);
		testCase.setStop(222);

		model::Link link1;
		link1.setName("TC link 1");
		link1.setURL("http://www.mylink1.com");
		link1.setType("tms");
		testCase.addLink(link1);

		model::Link link2;
		link2.setName("TC link 2");
		link2.setURL("http://www.jama.com/link");
		link2.setType("jama");
		testCase.addLink(link2);

		model::Label label1;
		label1.setName("package");
		label1.setValue("UnitTest");
		testCase.addLabel(label1);

		model::Label label2;
		label2.setName("feature");
		label2.setValue("TestCaseJSONSerializer");
		testCase.addLabel(label2);

		std::string expectedSerializedTestCase =
			"{\n"
			"    \"name\": \"Test case with links and labels\",\n"
			"    \"status\": \"skipped\",\n"
			"    \"stage\": \"pending\",\n"
			"    \"start\": 111,\n"
			"    \"stop\": 222,\n"
			"    \"links\": [\n"
			"        {\n"
			"            \"name\": \"TC link 1\",\n"
			"            \"url\": \"http://www.mylink1.com\",\n"
			"            \"type\": \"tms\"\n"
			"        },\n"
			"        {\n"
			"            \"name\": \"TC link 2\",\n"
			"            \"url\": \"http://www.jama.com/link\",\n"
			"            \"type\": \"jama\"\n"
			"        }\n"
			"    ],\n"
			"    \"labels\": [\n"
			"        {\n"
			"            \"name\": \"package\",\n"
			"            \"value\": \"UnitTest\"\n"
			"        },\n"
			"        {\n"
			"            \"name\": \"feature\",\n"
			"            \"value\": \"TestCaseJSONSerializer\"\n"
			"        }\n"
			"    ]\n"
			"}";

		std::string serializedTestCase = m_service.serialize(testCase);
		ASSERT_TRUE(compareJSONs(expectedSerializedTestCase, serializedTestCase, m_jsonAdapter));
	}

	TEST_F(TestCaseJSONSerializerTest, testSerializeForTCWithSingleActionAndExpectedResult)
	{
		model::TestCase testCase;
		testCase.setName("Test case with single action");
		testCase.setStatus(model::Status::FAILED);
		testCase.setStage(model::Stage::FINISHED);
		testCase.setStart(123456);
		testCase.setStop(789012);

		model::Action action;
		action.setName("Execute algorithm");
		action.setStatus(model::Status::BROKEN);
		action.setStage(model::Stage::INTERRUPTED);
		action.setStart(124000);
		action.setStop(789000);

		model::ExpectedResult expectedResult;
		expectedResult.setName("Algorithm result is 10");
		expectedResult.setStatus(model::Status::UNKNOWN);
		expectedResult.setStage(model::Stage::RUNNING);
		expectedResult.setStart(125000);
		expectedResult.setStop(126000);

		model::Parameter expectedResultParam;
		expectedResultParam.setName("ExpectedResult");
		expectedResultParam.setValue("10");
		expectedResult.addParameter(expectedResultParam);

		action.addExpectedResult(expectedResult);
		testCase.addAction(action);

		std::string expectedSerializedTestCase =
			"{\n"
			"    \"name\": \"Test case with single action\",\n"
			"    \"status\": \"failed\",\n"
			"    \"stage\": \"finished\",\n"
			"    \"start\": 123456,\n"
			"    \"stop\": 789012,\n"
			"    \"steps\":\n"
			"    [{\n"
			"        \"name\": \"Action: Execute algorithm\",\n"
			"        \"status\": \"broken\",\n"
			"        \"stage\": \"interrupted\",\n"
			"        \"start\": 124000,\n"
			"        \"stop\": 789000,\n"
			"        \"steps\":\n"
			"        [{\n"
			"            \"name\": \"Algorithm result is 10\",\n"
			"            \"status\": \"unknown\",\n"
			"            \"stage\": \"running\",\n"
			"            \"start\": 125000,\n"
			"            \"stop\": 126000,\n"
			"            \"parameters\":\n"
			"            [{\n"
			"                \"name\": \"ExpectedResult\",\n"
			"                \"value\": \"10\"\n"
			"            }]\n"
			"        }]\n"
			"    }]\n"
			"}";

		std::string serializedTestCase = m_service.serialize(testCase);
		ASSERT_TRUE(compareJSONs(expectedSerializedTestCase, serializedTestCase, m_jsonAdapter));
	}

}}}
