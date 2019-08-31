#include "Core/api/Logger.h"
#include "Core/api/Runtime.h"

#include <QCoreApplication>

#include <iostream>



int main(int argc, char* argv[])
{
    LOG_DEBUG(nullptr) << "IpcAdapter starting...";

    // Must be instantiated in order for QCoreApplication::arguments() to work
    QCoreApplication app(argc, argv);
    auto const& args = QCoreApplication::arguments();

    if(args.length() <= 1)
    {
        std::cout << "Usage: " << qPrintable(args.at(0)) << " <config>" << std::endl;
        return 1;
    }


    try
    {
        auto runtime = Core::Runtime::createFrom(args.at(1));

        runtime->configure();
        runtime->serveForever();
        return QCoreApplication::exec();
    }
    catch(std::exception const& anException)
    {
        std::cout << "Fatal error occurred: " << anException.what();
        return 2;
    }
}

