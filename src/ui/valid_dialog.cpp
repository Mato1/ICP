/**
 * @brief 
 * 
 * @file valid_dialog.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */

#include "valid_dialog.h"
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

namespace icp
{
namespace ui
{

ValidDialog::ValidDialog(std::string regex,
                         std::string title,
                         std::string popis,
                         std::string default_text,
                         std::string placeholder,
                         QWidget * parent)

    : QDialog(parent), regex(regex)
{
    setWindowTitle(title.c_str());
    this->setAttribute(Qt::WA_QuitOnClose, false);

    QVBoxLayout * vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel(tr(popis.c_str())));

    line_edit = new QLineEdit();
    line_edit->setPlaceholderText(tr(placeholder.c_str()));
    line_edit->setText(tr(default_text.c_str()));

    vbox->addWidget(line_edit);

    QRegExpValidator * v = new QRegExpValidator(QRegExp(regex.c_str()));
    line_edit->setValidator(v);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel);
    vbox->addWidget(buttonBox);
    this->setLayout(vbox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ValidDialog::~ValidDialog()
{

}

QString ValidDialog::get_value()
{
    return line_edit->text();
}

}
}
