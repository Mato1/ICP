/**
 * @brief 
 * 
 * @file add_connection_dialog.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#ifndef ICP_BLOCKBUILDER_SRC_UI_ADDCONNECTIONDIALOG_H
#define ICP_BLOCKBUILDER_SRC_UI_ADDCONNECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <string>
#include "model/block.h"
#include <vector>
#include <QComboBox>

namespace icp
{
namespace ui
{

class AddConnectionDialog : public QDialog
{
    Q_OBJECT

private slots:
    void output_block_selected(int a);
    void input_block_selected(int a);


private:
    QComboBox * combo_output_blocks;
    QComboBox * combo_output_ports;
    QComboBox * combo_input_blocks;
    QComboBox * combo_input_ports;

    QStringList items_output_blocks;
    QStringList items_output_ports;
    QStringList items_input_blocks;
    QStringList items_input_ports;


public:
    AddConnectionDialog(const std::vector<model::Block *> &blocks, QWidget * parent);

    ~AddConnectionDialog();

    QString get_selected_output_block();
    QString get_selected_output_port();
    QString get_selected_input_block();
    QString get_selected_input_port();

};
}
}

#endif // !ICP_BLOCKBUILDER_SRC_UI_ADDCONNECTIONDIALOG_H