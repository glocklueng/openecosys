
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include "ModuleConfiguration.h"
#include "UserPreferences.h"


class NETVConsoleApp : public QApplication
{
public:

    NETVConsoleApp(int argc, char** argv)
        : QApplication(argc,argv)
    {
        qDebug("Starting netvconsole...");
    }


};




int main(int argc, char* argv[])
{

    NETVConsoleApp app(argc,argv);

    app.exec();

    return 0;
}
