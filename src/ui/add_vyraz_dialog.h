/**
 * @brief 
 * 
 * @file add_vyraz_dialog.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#ifndef ICP_BLOCKBUILDER_SRC_UI_ADDVYRAZDIALOG_H
#define ICP_BLOCKBUILDER_SRC_UI_ADDVYRAZDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <string>
#include "model/port.h"
#include <vector>
#include <QComboBox>

namespace icp
{
namespace ui
{

class AddVyrazDialog : public QDialog
{
    Q_OBJECT

private:
    QComboBox * combo_input_ports;
    QComboBox * combo_output_ports;
    QLineEdit * line_edit;
    QLineEdit * line_edit2;
    QStringList items_input;
    QStringList items_output;


public:
    AddVyrazDialog(const std::vector<model::Port *> &input_ports,
                   const std::vector<model::Port *> &output_ports,
                   QWidget * parent = 0);

    ~AddVyrazDialog();
    QString get_value();
    QString get_selected_input_port();
    QString get_selected_output_port();
    QString get_premenna();


};
}
}

#endif // !ICP_BLOCKBUILDER_SRC_UI_ADDVYRAZDIALOG_H