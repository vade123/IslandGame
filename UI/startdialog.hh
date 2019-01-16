/* file: startdialog.hh
 * description: Header for class StartDialog. StartDialog asks the number of
 * players and pawns to be initialized to the game.
 */

#ifndef DIALOG_HH
#define DIALOG_HH

#include <QDialog>

namespace Ui {
class StartDialog;
}

namespace Student {

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

    unsigned getPlayers();
    unsigned getPawns();

public slots:
    virtual void accept();

private:
    unsigned players_;
    unsigned pawns_;

    Ui::StartDialog *ui_;
};

}
#endif // DIALOG_HH
