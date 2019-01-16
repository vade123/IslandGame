/* file: main.cpp
 * description: Starts and closes the program. Checks for errors in opening and
 * reading the initialization files.
 */

#include "mainwindow.hh"
#include "ioexception.hh"
#include "formatexception.hh"

#include <memory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Student::StartDialog d;
    int accepted = d.exec();
    Student::MainWindow *w;
    if (accepted == QDialog::Accepted) {
        unsigned players = d.getPlayers();
        unsigned pawns = d.getPawns();
        QMessageBox error;
        error.setIcon(QMessageBox::Critical);
        try {
            w = new Student::MainWindow(players, pawns);
        } catch (Common::IoException &e) {
            error.setText(QString::fromStdString(e.msg()));
            error.exec();
            return 1;
        } catch (Common::FormatException &e) {
            error.setText(QString::fromStdString(e.msg()));
            error.exec();
            return 1;
        }
    } else {
        return 0;
    }
    w->show();
    return a.exec();
}
