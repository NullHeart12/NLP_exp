#include "generation_and_tokenization.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    generation_and_tokenization w;
    w.show();
    return a.exec();
}
