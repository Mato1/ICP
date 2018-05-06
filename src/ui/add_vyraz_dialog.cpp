/**
 * @brief 
 * 
 * @file add_vyraz_dialog.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */
#include "add_vyraz_dialog.h"
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace icp
{
namespace ui
{

AddVyrazDialog::AddVyrazDialog(const std::vector<model::Port *> &input_ports,
                               const std::vector<model::Port *> &output_ports,
                               QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle("Novy vyraz");
    this->setAttribute(Qt::WA_QuitOnClose, false);

    QVBoxLayout * vbox = new QVBoxLayout();

    vbox->addWidget(new QLabel(tr("Input port:")));
    combo_input_ports = new QComboBox();
    combo_input_ports->setMinimumHeight(30);


    for (auto it = input_ports.begin(); it != input_ports.end(); it++)
    {
        // items_input << tr((*it)->get_nazov().c_str());
        combo_input_ports->addItem(tr((*it)->to_string().c_str()),
                                   QVariant(tr((*it)->get_nazov().c_str())));
    }


    vbox->addWidget(combo_input_ports);

    vbox->addWidget(new QLabel(tr("Output port:")));
    combo_output_ports = new QComboBox();
    combo_output_ports->setMinimumHeight(30);

    for (auto it = output_ports.begin(); it != output_ports.end(); it++)
    {
        combo_output_ports->addItem(tr((*it)->to_string().c_str()),
                                    QVariant(tr((*it)->get_nazov().c_str())));
    }

    // combo_output_ports->addItems(items_output);
    vbox->addWidget(combo_output_ports);

    vbox->addWidget(new QLabel(tr("Vyraz:")));
    QHBoxLayout * hbox = new QHBoxLayout();
    line_edit = new QLineEdit();
    line_edit->setPlaceholderText(tr("a+b+c+d"));
    hbox->addWidget(line_edit);
    hbox->addWidget(new QLabel(tr("=")));
    line_edit2 = new QLineEdit();
    hbox->addWidget(line_edit2);
    vbox->addLayout(hbox);


    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel);
    vbox->addWidget(buttonBox);
    this->setLayout(vbox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

AddVyrazDialog::~AddVyrazDialog()
{

}

QString AddVyrazDialog::get_value()
{
    return line_edit->text();
}

QString AddVyrazDialog::get_premenna()
{
    return line_edit2->text();
}

QString AddVyrazDialog::get_selected_input_port()
{
    return combo_input_ports->currentData().toString();
}

QString AddVyrazDialog::get_selected_output_port()
{
    return combo_output_ports->currentData().toString();
}

}
}
