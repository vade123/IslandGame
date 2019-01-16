/* file: startdialog.cpp
 * description: Implementation for the class StartDialog.
 */

#include "startdialog.hh"
#include "ui_startdialog.h"

namespace Student {

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::StartDialog)
{
    ui_->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}

StartDialog::~StartDialog()
{
    delete ui_;
}

unsigned StartDialog::getPlayers()
{
    return players_;
}

unsigned StartDialog::getPawns()
{
    return pawns_;
}

void StartDialog::accept()
{
    players_ = static_cast<unsigned int>(ui_->spinBox_players->value());
    pawns_ = static_cast<unsigned int>(ui_->spinBox_pawns->value());
    QDialog::accept();
}

}
