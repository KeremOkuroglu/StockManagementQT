#include "mainwindow.h"

#include <QApplication>

#include <QApplication>

#include "mainwindow.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "InventoryManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    InventoryManager manager;

    LoginDialog login(&manager);

    if(login.exec() == QDialog::Accepted)
    {
        MainWindow w(&manager);
        w.show();

        return a.exec();
    }

    return 0;
}