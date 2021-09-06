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

Measurement_Software::Measurement_Software(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

}

void Measurement_Software::on_btn_load_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Open Directory", "C:/",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    std::string str_dir = directory.toStdString();

    QStringList filters;
    filters << "*.tps";

    std::string output_results = "File_Name,WIS,WIM,WIB,LDC,RDC,OA,D";

    QDir dir(directory);
    dir.setNameFilters(filters);

    bool empty_result = dir.isEmpty();
    if (empty_result == true) {
        QMessageBox::warning(this, "..", "Selected directory has no .tps files to process.");
        return;
    }

    for (QFileInfo var : dir.entryInfoList()) {

        std::string file_name = var.fileName().toStdString();
        std::string full_path = str_dir + "/" + file_name;

        std::ifstream infile;
        infile.open(full_path);

        if (infile.fail()) {
            QMessageBox::warning(this, "..", "Error in opening file");
            exit(1);
        }

        std::string results = read_measurements(infile, file_name);
        output_results = output_results + "\n" + results;

        infile.close();

    }

    ui.textEdit->setText(QString::fromStdString(output_results));
    
}

void Measurement_Software::on_btn_save_clicked()
{
    QString output_text;
    output_text = ui.textEdit->toPlainText();
    std::string str_output_text = output_text.toStdString();

    QString saveFileName = QFileDialog::getSaveFileName(this, "Save as:");
    std::string str_saveFileName = saveFileName.toStdString();
    str_saveFileName = str_saveFileName + ".csv";

    std::ofstream outfile;
    outfile.open(str_saveFileName);
    outfile << str_output_text;

    outfile.close();
}
