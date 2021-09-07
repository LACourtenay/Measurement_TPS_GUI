#include "Measurement_Software.h"
#include "File_Management.h"
#include "ui_Measurement_Software.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <string>
#include <qdebug.h>
#include "Morphologika.h"
#include "Measurements.h"

Measurement_Software::Measurement_Software(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.btn_compute->setEnabled(false);
    ui.btn_save->setEnabled(false);

}

void Measurement_Software::on_btn_load_clicked()
{
    this->directory = QFileDialog::getExistingDirectory(this, "Open Directory", "C:/",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    this->str_dir = directory.toStdString();

    this->filters << "*.tps";

    QDir dir(this->directory);
    dir.setNameFilters(this->filters);

    bool empty_result = dir.isEmpty();
    if (empty_result == true) {
        ui.btn_compute->setEnabled(false);
        ui.btn_save->setEnabled(false);
        ui.textEdit->setText("");
        QMessageBox::warning(this, "Directory Warning", "Selected directory has no .tps files to process.");
        return;
    }
    else {
        ui.btn_compute->setEnabled(true);
    }

    std::string message_information = std::to_string(dir.count()) + " .tps files found in the selected folder: " + str_dir;

    QMessageBox::information(this, "Selected Directory", QString::fromStdString(message_information));  

}

void Measurement_Software::on_btn_compute_clicked()
{

    ui.textEdit->setText("");

    QDir dir(this->directory);
    dir.setNameFilters(this->filters);

    QString extract_label = ui.lbl_sample->text();
    std::string lb = extract_label.toStdString();

    check_label(&lb);

    this->sample_label = lb;

    if (ui.rbtn_meas->isChecked()) {

        if (this->sample_label == "") {
            this->output_results = "File_Name,WIS,WIM,WIB,LDC,RDC,OA,D";
        }
        else {
            this->output_results = "File_Name,Sample,WIS,WIM,WIB,LDC,RDC,OA,D";
        }

        for (QFileInfo var : dir.entryInfoList()) {

            std::string file_name = var.fileName().toStdString();
            std::string full_path = this->str_dir + "/" + file_name;

            std::ifstream infile;
            infile.open(full_path);

            if (infile.fail()) {
                QMessageBox::warning(this, "Warning", "Error in opening file");
                exit(1);
            }

            bool c_delim;
            if (ui.rbtn_comma->isChecked()) {
                c_delim = true;
            }
            else {
                c_delim = false;
            }

            std::string results = read_measurements(infile, file_name, &c_delim, this->sample_label);
            this->output_results = this->output_results + "\n" + results;

            infile.close();

        }
    }
    else {

        std::string landmarks;
        std::string* pt_landmarks = &landmarks;
        std::string lb_sample;
        std::string* pt_sample = &lb_sample;
        std::string names;
        std::string* pt_names = &names;

        this->output_results = "[individuals]\n" + std::to_string(dir.count()) + "\n" +
            "[landmarks]\n7\n[dimensions]\n2\n[names]\n";

        for (QFileInfo var : dir.entryInfoList()) {

            std::string file_name = var.fileName().toStdString();
            std::string full_path = this->str_dir + "/" + file_name;

            std::ifstream infile;
            infile.open(full_path);

            if (infile.fail()) {
                QMessageBox::warning(this, "Warning", "Error in opening file");
                exit(1);
            }

            morph_individual_string(infile, &file_name, pt_landmarks);
            morph_sample_string(this->sample_label, pt_sample);
            morph_names_string(file_name, pt_names);

            infile.close();

        }

        this->output_results = this->output_results + *pt_names;

        if (this->sample_label != "") {
            this->output_results = this->output_results + "[labels]\nSample\n[labelvalues]\n" + *pt_sample;
        }
        
        this->output_results = this->output_results + "[rawpoints]\n" + *pt_landmarks;
        this->output_results.resize(this->output_results.size() - 2);

        //QMessageBox::information(this, "..", "Morphologika feature coming soon...");
        //this->output_results = "Morphologika feature currently unavailable...";

    }

    ui.textEdit->setText(QString::fromStdString(this->output_results));

    ui.btn_save->setEnabled(true);
}

void Measurement_Software::on_btn_save_clicked()
{
    QString output_text;
    output_text = ui.textEdit->toPlainText();
    std::string str_output_text = output_text.toStdString();

    QString saveFileName = QFileDialog::getSaveFileName(this, "Save as...");
    std::string str_saveFileName = saveFileName.toStdString();

    if (ui.rbtn_meas->isChecked()) {
        std::string real_extension = str_saveFileName;
        real_extension.erase(0, real_extension.length() - 4);
        if (real_extension != ".csv") {
            str_saveFileName = str_saveFileName + ".csv";
        }
    }
    else {
        std::string real_extension = str_saveFileName;
        real_extension.erase(0, real_extension.length() - 4);
        if (real_extension != ".txt") {
            str_saveFileName = str_saveFileName + ".txt";
        }
    }

    std::ofstream outfile;
    outfile.open(str_saveFileName);
    outfile << str_output_text;

    std::string message_info = "Results saved to: " + str_saveFileName;
    QMessageBox::information(this, "File Saved Succesfully", QString::fromStdString(message_info));

    outfile.close();
}

void Measurement_Software::on_rbtn_morph_clicked()
{
    ui.rbtn_comma->setEnabled(false);
    ui.rbtn_semi->setEnabled(false);
}

void Measurement_Software::on_rbtn_meas_clicked()
{
    ui.rbtn_comma->setEnabled(true);
    ui.rbtn_semi->setEnabled(true);
}

void Measurement_Software::on_actionAuthor_triggered()
{
    QMessageBox::information(this, "Info",
        tr("About the Author\n\n"
            "The present application was designed and created by Lloyd A. Courtenay."
            "\n(TIDOP Research Group, University of Salamanca) \nLast Update: 07/09/2021"
            "\nIn case of issues contact ladc1995@gmail.com"
            "\n\nAknowledgements:\nAlberto Morcillo Sanz (TIDOP Research Group)\nEsteban Ruiz de O\u00D1a Crespo (TIDOP Research Group)"));
}