/**
 * @brief 
 * 
 * @file valid_dialog.h
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#ifndef ICP_BLOCKBUILDER_SRC_UI_VALIDDIALOG_H
#define ICP_BLOCKBUILDER_SRC_UI_VALIDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <string>

namespace icp
{
namespace ui
{

class ValidDialog : public QDialog
{
    Q_OBJECT

private:
    std::string regex;
    QLineEdit * line_edit;



public:
    ValidDialog(std::string regex,
                std::string title = "",
                std::string popis = "",
                std::string default_text = "",
                std::string placeholder = "",
                QWidget * parent = 0);

    ~ValidDialog();
    QString get_value();

};
}
}

#endif // !ICP_BLOCKBUILDER_SRC_UI_VALIDDIALOG_H