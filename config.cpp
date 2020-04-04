#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
}

Config::~Config()
{
    delete ui;
}


QString Config::getRoomName(){
    if(roomSet){
        return ui->lineEditRoom->text();
    }
    else {
       return nullptr;
    }
}

QString Config::getSwitchName(){
    if(nameSet){
        return ui->lineEditSwitchName->text();
    }
    else {
       return nullptr;
    }
}

int Config::getConfigSet(){
    if(roomSet && nameSet){
        return 1;

    }
    return 0;
}
void Config::on_lineEditRoom_textChanged(const QString &arg1)
{
    roomSet = 1;
    emit configChanged();
}

void Config::on_lineEditSwitchName_textChanged(const QString &arg1)
{
    nameSet = 1;
    emit configChanged();
}
