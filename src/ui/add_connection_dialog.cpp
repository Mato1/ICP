#include "ui/add_connection_dialog.h"
#include <QDialogButtonBox>
#include <QRegExpValidator>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include "ui/schema_w.h"

namespace icp
{
namespace ui
{

AddConnectionDialog::AddConnectionDialog(const std::vector<model::Block *> &blocks,
                                         QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle("Nove Prepojenie");
    this->setAttribute(Qt::WA_QuitOnClose, false);

    QGridLayout * glayout = new QGridLayout();
    combo_output_blocks = new QComboBox();
    combo_output_ports  = new QComboBox();
    combo_input_blocks  = new QComboBox();
    combo_input_ports   = new QComboBox();

    glayout->addWidget(new QLabel(tr("Output Block:")), 0, 0);
    glayout->addWidget(new QLabel(tr("Input Block:")), 0, 1);
    glayout->addWidget(combo_output_blocks, 1, 0);
    glayout->addWidget(combo_input_blocks, 1, 1);

    glayout->addWidget(new QLabel(tr("Output Port:")), 2, 0);
    glayout->addWidget(new QLabel(tr("Input Port:")), 2, 1);
    glayout->addWidget(combo_output_ports, 3, 0);
    glayout->addWidget(combo_input_ports, 3, 1);
    
    combo_input_ports->setMinimumHeight(30);


    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        // items_input << tr((*it)->get_nazov().c_str());
        combo_output_blocks->addItem(tr((*it)->get_nazov().c_str()), QVariant(tr((*it)->get_nazov().c_str())));
        combo_input_blocks->addItem(tr((*it)->get_nazov().c_str()), QVariant(tr((*it)->get_nazov().c_str())));
    }


    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    glayout->addWidget(buttonBox);
    this->setLayout(glayout);

    connect(combo_output_blocks, SIGNAL(activated(int)), this, SLOT(output_block_selected(int)));
    connect(combo_input_blocks, SIGNAL(activated(int)),  this, SLOT(input_block_selected(int)));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    
}

void AddConnectionDialog::output_block_selected(int a)
{
    combo_output_ports->clear();
    SchemaW * schemaw = static_cast<SchemaW*>(parent());

    model::Block * selected_block = schemaw->get_block(combo_output_blocks->currentData().toString().toStdString());
    std::vector<model::Port* > output_ports = selected_block->get_output_ports();
    for (auto it = output_ports.begin(); it != output_ports.end(); it++)
    {
        std::string port_name = (*it)->get_nazov();

        combo_output_ports->addItem(tr(port_name.c_str()), QVariant(port_name.c_str()));
    }
}

void AddConnectionDialog::input_block_selected(int a)
{
    combo_input_ports->clear();
    SchemaW * schemaw = static_cast<SchemaW*>(parent());

    model::Block * selected_block = schemaw->get_block(combo_input_blocks->currentData().toString().toStdString());
    std::vector<model::Port* > input_ports = selected_block->get_input_ports();
    for (auto it = input_ports.begin(); it != input_ports.end(); it++)
    {
        std::string port_name = (*it)->get_nazov();

        combo_input_ports->addItem(tr(port_name.c_str()), QVariant(port_name.c_str()));
    }
}

AddConnectionDialog::~AddConnectionDialog()
{

}

QString AddConnectionDialog::get_selected_output_block()
{
    return combo_output_blocks->currentData().toString();
}

QString AddConnectionDialog::get_selected_output_port()
{
     return combo_output_ports->currentData().toString();
}

QString AddConnectionDialog::get_selected_input_block()
{
    return combo_input_blocks->currentData().toString();
}

QString AddConnectionDialog::get_selected_input_port()
{
     return combo_input_ports->currentData().toString();
}

}
}
