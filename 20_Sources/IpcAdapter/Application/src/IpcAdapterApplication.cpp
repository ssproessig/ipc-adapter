#include <QCoreApplication>


int main(int argc, char* argv[])
{
    // Must be instantiated in order for QCoreApplication::arguments() to work
    QCoreApplication app(argc, argv);
    return app.exec();
}
