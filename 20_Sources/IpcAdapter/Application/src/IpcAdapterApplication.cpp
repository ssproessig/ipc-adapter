#include "Core/api/Logger.h"

#include <QCoreApplication>


int main(int argc, char* argv[])
{
    LOG_DEBUG(nullptr) << "IpcAdapter starting...";

    // Must be instantiated in order for QCoreApplication::arguments() to work
    QCoreApplication app(argc, argv);
    return app.exec();
}
