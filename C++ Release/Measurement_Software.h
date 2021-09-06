#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Measurement_Software.h"

class Measurement_Software : public QMainWindow
{
    Q_OBJECT

public:
    Measurement_Software(QWidget *parent = Q_NULLPTR);

private slots:
    void on_btn_load_clicked();
    void on_btn_save_clicked();
    //void on_btn_calculate_clicked();

private:
    
    Ui::Measurement_SoftwareClass ui;
};
