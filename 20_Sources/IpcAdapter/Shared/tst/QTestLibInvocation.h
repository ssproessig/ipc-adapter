#pragma once

// from Qt
#include <QCoreApplication>
#include <QDir>
#include <QProcessEnvironment>
#include <QTest>

// from STL
#include <iostream>
#include <typeinfo>



/**
 * Checks whether the given @c aTestName matches one of the filters in @c aFilterList. If
 * no filters are given this methods assumes that any @c aTestName matches.
 *
 * NOTE: Currently this only checks if string @c aTestName contains one of the filters.
 *       Maybe use regular expressions later.
 *
 * @param   aTestName       name of the test we check for the filters
 * @param   aFilterList     a list of filters that decide if a tests is executed at all
 *
 * @retval  true            if @c aTestName matches at least one of the filter OR there are no filters
 * @retval  false           if @c aTestName did not match any of the filter
 */
bool testNameMatchesFilters(QString const& aTestName, QStringList const& aFilterList)
{
    if(aFilterList.isEmpty())
    {
        return true;
    }

    for(auto const& filter : aFilterList)
    {
        if(aTestName.contains(filter))
        {
            return true;
        }
    }

    return false;
}



/**
 * Templated execution of a @c QTestLib based test using @c QTest::qExec.
 *
 * @param anOutputDir       output directory of the test result XML.
 * @param anArgumentsList   a list of arguments to pass to the test runner
 * @param aFilterList       a list of filters that decide if a tests is executed at all
 *
 * @see: prepareEnv(...) for preparation of @c anArgumentsList and @c aFilterList
 */
template <typename T>
int runTest(const QString& anOutputDir, QStringList anArgumentsList, QStringList aFilterList)
{
    // first check the test name itself matches any of the filter expressions
    QString testName = typeid(T).name();

    if(!testNameMatchesFilters(testName, aFilterList))
    {
        std::cerr << "Skipping '" << qPrintable(testName) << "' because of unmatched filter." << std::endl;
        return 0;
    }

    // instantiate and execute the test
    T theTest;

    QString targetFile = anOutputDir;
    targetFile.append(testName);
    targetFile.append(".xml");
    targetFile = targetFile.replace(" ", "_");
    targetFile = targetFile.replace("::", "_");
    anArgumentsList.last() = targetFile;

    return QTest::qExec(&theTest, anArgumentsList);
}



/**
 * Templated execution of a @c QTestLib based test using @c QTest::qExec.
 *
 * @param argc              argc from the test's `main(...)`
 * @param argv              argv from the test's `main(...)`
 * @param anOutputDir       output directory of the test result XML - will be updated to the temporary directory
 *                          when environment variable 'UNIT_TEST_RESULTS' is not set
 * @param anArgumentsList   a list of arguments for test execution; will only forward @c --output-dir given and augment
 *                          the test runner for XUnit XML result files
 * @param aFilterList       a list of filters that decide if a tests is executed at all
 *
 * @see: runTest(...) for usage of @c anArgumentsList and @c aFilterList
 */
void prepareEnv(int argc, char* argv[], QString& anOutputDir, QStringList& anArgumentsList, QStringList& aFilterList)
{
    // init the output dir
    QProcessEnvironment pe = QProcessEnvironment::systemEnvironment();
    anOutputDir = pe.value("UNIT_TEST_RESULTS", QDir::tempPath());

    // if dir not exists, create it
    if(QDir(anOutputDir).exists() == false)
    {
        QDir().mkpath(anOutputDir);
    }

    // ignore all command line arguments and use only our own
    anArgumentsList.clear();

    // add the binary itself as first options (is thrown away by qt anyway...)
    anArgumentsList.append(argv[0]);

    // iterate the options we got
    for(int i = 1; i < argc; ++i)
    {
        QString arg(argv[i]);
        bool hasNext = (i + 1 < argc);

        // parse output directory
        if(arg == "--output-dir" && hasNext)
        {
            anOutputDir = argv[i + 1];
        }
        else if(arg == "--filter" && hasNext)
        {
            aFilterList.append(argv[i + 1]);
        }

        // ignore the rest
    }

    // add a separator
    anOutputDir.append(QDir::separator());

    // add our QTestLib arguments
    anArgumentsList.append("-xunitxml"); // output as xUnit XML
    anArgumentsList.append("-o"); // output to file
    anArgumentsList.append("will_be_replace_in_runTest.xml"); // will be overridden by each test
}
