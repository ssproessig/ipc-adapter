#pragma once

// we use all convenience macros, but add more for testing scope
#include "Core/api/Logger.h"

#include "Shared/api/ConvenienceMacros.h"

#include <QTest>
#include <iostream>



// private data implementation; real internal data structure must be added in implementation
#define PIMPLED_TEST_DATA(className)												\
    private:																		\
        struct Data;																\
        Data* d = nullptr



// some macros for later log-step catching
#define LOG_TEST_STEP(TYPE, TEXT)													\
	std::cout << TYPE << TEXT << std::endl;

#define TEST_SPEC_META_INFORMATION(author)

// some macros we will use for documentation generation
#define TEST_REQUIREMENT(requirement_id)                                            \
    LOG_TEST_STEP("Requirement tested    ", requirement_id)                         \

#define TEST_ANNOTATION(some_description_what_is_being_tested)                      \
    LOG_TEST_STEP("                      ", some_description_what_is_being_tested)  \
    LOG_DEBUG(nullptr) << some_description_what_is_being_tested;

#define FUTURE_REALIZE_REQUIREMENT(requirement_id)                                  \
    LOG_DEBUG(nullptr) << "Future requirement    " << requirement_id;

#define FUTURE_TEST(some_description_of_what_shall_be_tested_later)                 \
    LOG_TEST_STEP("Future test                  ",									\
		some_description_of_what_shall_be_tested_later)								\
    LOG_DEBUG(nullptr) << "Future test step      " <<								\
        some_description_of_what_shall_be_tested_later;

#define EXECUTE(expression, step)                                                   \
    LOG_TEST_STEP("Step                  ", step)                                   \
    LOG_TEST_STEP("  execute             ", #expression)                            \
    expression

#define VERIFY(expression, expectation)                                             \
    LOG_TEST_STEP("  expectation         ", expectation)                            \
    LOG_TEST_STEP("  verify that         ", #expression)                            \
    QVERIFY2(expression, expectation)



#define COMPARE(actual, expected, expectation)                                      \
    LOG_TEST_STEP("Expectation           ", expectation)                            \
    LOG_TEST_STEP("  actual value        ", #actual)                                \
    LOG_TEST_STEP("  expected value      ", #expected)								\
    QCOMPARE(actual, expected)



#define EXPECT_EXCEPTION(expression, expectation, ...)                              \
    try {                                                                           \
        LOG_TEST_STEP("Expected exception    ", expectation)                        \
        LOG_TEST_STEP("  execute             ", #expression)                        \
        LOG_TEST_STEP("  exception contains  ", #__VA_ARGS__)                       \
        expression                                                                  \
        QVERIFY2(false, "Expected exception not thrown!");                          \
    }                                                                               \
    catch(std::exception const& anException) {                                      \
        QString const& aWhat = anException.what();                                  \
        for (auto const& contains : {__VA_ARGS__})  {                               \
            if (!aWhat.contains(contains)) {                                        \
                QByteArray const exception =                                        \
                    (QString("Exception what() did not contain '%1'!").             \
                    arg(contains)).toLocal8Bit();                                   \
                QVERIFY2(false, exception.constData());                             \
            }                                                                       \
        }                                                                           \
    }

#define EXPECT_NO_EXCEPTION(expression, step)										\
    try {                                                                           \
        LOG_TEST_STEP("Step                  ", expectation)                        \
        LOG_TEST_STEP("  exectue             ", #expression)                        \
        expression                                                                  \
    }                                                                               \
    catch(std::exception const& anException) {                                      \
        QVERIFY2(false, anException.what());                                        \
    }

#define TEST_FIX_FOR(ticketing_system_id)
