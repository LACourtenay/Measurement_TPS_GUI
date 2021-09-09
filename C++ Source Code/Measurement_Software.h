#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Measurement_Software.h"
#include <qdir.h>

class Measurement_Software : public QMainWindow
{
    Q_OBJECT

public:
    Measurement_Software(QWidget *parent = Q_NULLPTR);

public:
    QString directory;
    std::string str_dir;
    QStringList filters;
    std::string sample_label;
    std::string output_results;

private slots:
    void on_btn_load_clicked();
    void on_btn_save_clicked();
    void on_btn_compute_clicked();
    void on_rbtn_morph_clicked();
    void on_rbtn_meas_clicked();
    void on_actionAuthor_triggered();

private:
    
    Ui::Measurement_SoftwareClass ui;
};
