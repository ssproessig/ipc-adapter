#include "Core/api/Logger.h"
#include "Core/api/Runtime.h"

#include <QCoreApplication>

#include <iostream>



int main(int argc, char* argv[])
{
    LOG_DEBUG(nullptr) << "IpcAdapter starting...";

    // Must be instantiated in order for QCoreApplication::arguments() to work
    QCoreApplication app(argc, argv);

    if(app.arguments().length() <= 1)
    {
        std::cout << "Usage: " << qPrintable(app.arguments().at(0)) << " <config>" << std::endl;
        return 1;
    }

    auto runtime = Core::Runtime::createFrom(app.arguments().at(1));


    return app.exec();
}
