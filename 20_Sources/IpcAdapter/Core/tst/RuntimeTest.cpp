#include "RuntimeTest.h"

#include "Core/api/Runtime.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Core::Runtime;
using IpcAdapter::Core::RuntimeTest;



RuntimeTest::RuntimeTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SINK-001");
}



void RuntimeTest::test_01_Runtime_initialization_fails_if_not_existing_file_is_passed()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/unknown.xml");,
        "creating Runtime from ':/unknown.xml' configuration must fail",
        "Given configuration ':/unknown.xml' not found!"
    );
}



void RuntimeTest::test_02_Runtime_initialization_fails_for_broken_file()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_00_broken_file.xml");,
        "creating Runtime from broken file must fail",
        "fatalError [1,1]: error occurred while parsing element"
    );
}



void RuntimeTest::test_03_Runtime_initialization_fails_for_wrong_xml_used()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_03_wrong_xml.xml");,
        "creating Runtime from XML with wrong schema must fail",
        "configuration has unsupported namespace ''!"
    );
}



void RuntimeTest::test_04_Runtime_initialization_fails_for_wrong_root_element()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_04_wrong_root_element.xml");,
        "creating Runtime from XML with correct schema but wrong root element must fail",
        "configuration has wrong root element 'source'!"
    );
}



void RuntimeTest::test_05_Runtime_initialization_fails_for_root_element_with_unsupported_version()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_05_root_element_with_wrong_version.xml");,
        "creating Runtime from XML with correct schema and root element, but unsupported version, must fail",
        "configuration has unsupported version '2'!"
    );
}
