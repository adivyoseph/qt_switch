#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = nullptr);
    ~Config();
    QString getRoomName();
    QString getSwitchName();
    int     getConfigSet();

private slots:
    void on_lineEditRoom_textChanged(const QString &arg1);

    void on_lineEditSwitchName_textChanged(const QString &arg1);

signals:
    void configChanged(void);

private:
    Ui::Config *ui;
    int nameSet;
    int roomSet;

};

#endif // CONFIG_H
