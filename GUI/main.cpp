#include <QtGui/QApplication>
#include "profox.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    profox w;
    w.show();
    
    return a.exec();
}
