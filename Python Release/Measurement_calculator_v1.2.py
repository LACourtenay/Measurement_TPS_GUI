

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMessageBox, QFileDialog

import os
import glob
import linecache
from numpy import array
import math
import pandas as pd

# additional functions

def set_directory(dir):
    os.chdir(dir)

def load_files():
    files = glob.glob("*.tps")
    return(files)

def euclid_distance(vector1, vector2):
    if vector1.shape[0] == 2:
        x = (vector1[0] - vector2[0])**2
        y = (vector1[1] - vector2[1])**2
        dist = math.sqrt(x + y)
        return(dist)
    elif vector1.shape[0] == 3:
        x = (vector1[0] - vector2[0])**2
        y = (vector1[1] - vector2[1])**2
        z = (vector1[2] - vector2[2])**2
        dist = math.sqrt(x + y + z)
        return(dist)
    else: print("error")
    
def angle(side_1, side_2, side_3):
	return(math.degrees(math.acos((side_1 * side_1 + side_2 * side_2 - side_3 * side_3)/(2.0 * side_1 * side_2))))

def perimiter(side_1, side_2, side_3):
    return(side_1 + side_2 + side_3)

def heron(side_1, side_2, side_3):
    s = perimiter(side_1, side_2, side_3)/2
    A = math.sqrt(s * (s - side_1) * (s - side_2) * (s - side_3))
    return(A)
    
def height(base, area):
    h = (2 * area) / base
    return(h)

# GUI and programme

class Ui_MainWindow(object):

    directory = ""
    files = []
    df = pd.DataFrame(columns = ["Photo_Name", "File_Name", "WIS", "WIM", "WIB", "LDC", "RDC", "OA", "D"])

    def setupUi(self, MainWindow):

        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(769, 284)
        MainWindow.setAutoFillBackground(False)

        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        # save button

        self.bt_save_data = QtWidgets.QPushButton(self.centralwidget)
        self.bt_save_data.setGeometry(QtCore.QRect(610, 180, 141, 41))
        font = QtGui.QFont()
        font.setPointSize(9)
        font.setBold(False)
        font.setWeight(50)
        self.bt_save_data.setFont(font)
        self.bt_save_data.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.bt_save_data.setObjectName("bt_save_data")
        
        self.bt_save_data.setEnabled(False)

        # view button

        self.bt_calculate_data = QtWidgets.QPushButton(self.centralwidget)
        self.bt_calculate_data.setGeometry(QtCore.QRect(160, 180, 141, 41))
        font = QtGui.QFont()
        font.setPointSize(9)
        font.setBold(False)
        font.setWeight(50)
        self.bt_calculate_data.setFont(font)
        self.bt_calculate_data.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.bt_calculate_data.setObjectName("bt_calculate_data")

        self.bt_calculate_data.setEnabled(False)

        # load button

        self.bt_load_data = QtWidgets.QPushButton(self.centralwidget)
        self.bt_load_data.setGeometry(QtCore.QRect(10, 180, 141, 41))
        font = QtGui.QFont()
        font.setPointSize(9)
        font.setBold(False)
        font.setWeight(50)
        self.bt_load_data.setFont(font)
        self.bt_load_data.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.bt_load_data.setObjectName("bt_load_data")
        
        # label

        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(20, 10, 721, 31))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(False)
        font.setWeight(50)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.label.setAlignment(QtCore.Qt.AlignCenter)

        # view directory 

        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(20, 70, 121, 16))
        self.label_2.setObjectName("label_2")
        self.dir_label = QtWidgets.QLabel(self.centralwidget)
        self.dir_label.setGeometry(QtCore.QRect(140, 70, 601, 16))
        font = QtGui.QFont()
        font.setBold(True)
        font.setWeight(75)
        self.dir_label.setFont(font)
        self.dir_label.setAutoFillBackground(False)
        self.dir_label.setObjectName("dir_label")

        MainWindow.setCentralWidget(self.centralwidget)
        
        # menu bar
        
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 769, 26))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtWidgets.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionInfo = QtWidgets.QAction(MainWindow)
        self.actionInfo.setObjectName("actionInfo")
        self.menubar.addAction(self.menuFile.menuAction())
        self.actionAbout = QtWidgets.QAction(MainWindow)
        self.actionAbout.setObjectName("actionAbout")
        self.menuFile.addAction(self.actionAbout)
        self.menubar.addAction(self.menuFile.menuAction())

        # actions

        self.actionAbout.triggered.connect(self.show_info_popup)

        self.bt_load_data.clicked.connect(self.find_directory)
        self.bt_load_data.clicked.connect(self.update_directory_label)
        
        self.bt_calculate_data.clicked.connect(self.extract_measurements)
        #self.bt_calculate_data.clicked.connect(self.success_popup)

        self.bt_save_data.clicked.connect(self.save_table)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.bt_save_data.setStatusTip(_translate("MainWindow", "Save data as a .csv file (Ctrl+S)"))
        self.bt_save_data.setText(_translate("MainWindow", "Save Table"))
        self.bt_save_data.setShortcut(_translate("MainWindow", "Ctrl+S"))
        self.bt_calculate_data.setStatusTip(_translate("MainWindow", "Calculate Measurements (Ctrl+M)"))
        self.bt_calculate_data.setText(_translate("MainWindow", "Calculate"))
        self.bt_calculate_data.setShortcut(_translate("MainWindow", "Ctrl+M"))
        self.bt_load_data.setStatusTip(_translate("MainWindow", "Load data from folder (Ctrl+O)"))
        self.bt_load_data.setText(_translate("MainWindow", "Load Data"))
        self.bt_load_data.setShortcut(_translate("MainWindow", "Ctrl+O"))
        self.label.setText(_translate("MainWindow", "Application to load and view measurement data from landmark coordinates."))
        self.label_2.setText(_translate("MainWindow", "Selected Directory:"))
        self.dir_label.setText(_translate("MainWindow", "No Directory Currently Selected"))
        self.dir_label.setStyleSheet("background-color: white")
        self.menuFile.setTitle(_translate("MainWindow", "Info"))
        self.actionInfo.setText(_translate("MainWindow", "Info"))
        self.actionInfo.setStatusTip(_translate("MainWindow", "Info"))
        self.actionAbout.setText(_translate("MainWindow", "About..."))
        self.actionAbout.setStatusTip(_translate("MainWindow", "About..."))

    def show_info_popup(self):
        msg = QMessageBox()
        msg.setWindowTitle("Info")
        msg.setText("Measurement Application")
        msg.setInformativeText("The present application was designed and created by Lloyd A. Courtenay.\n(TIDOP Research Group, University of Salamanca)" +
        "\nLast Update: 11/04/2021" +
        "\nIn case of issues contact ladc1995@gmail.com")
        msg.setIcon(QMessageBox.Information)

        x = msg.exec_()

    def find_directory(self):
        self.directory = str(QFileDialog.getExistingDirectory(None, "Select Directory"))
        if self.directory != "":
            self.bt_calculate_data.setEnabled(True)

    def update_directory_label(self):
        self.dir_label.setText(self.directory)

    def extract_measurements(self):

        set_directory(self.directory)
        self.files = load_files()
        if len(self.files) == 0:
            self.error_popup()
        else:
            for i in range(len(self.files)):

                # find scale in .tps file
                last_line = 0
                with open(self.files[i]) as f:
                    for line in f:
                        last_line += 1

                Photo_Name = linecache.getline(self.files[i], last_line - 1)
                Photo_Name = Photo_Name[6:]
                Photo_Name = Photo_Name.rstrip("\n")

                scale = linecache.getline(self.files[i], last_line)
                scale = scale.split("=")
                scale = scale[1].rstrip("\n")
                scale = array(scale, dtype = float)

                # load landmarks
                target_lm = linecache.getline(self.files[i], last_line-8)
                lm_values = [float(n) for n in target_lm.split()]
                LM1 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-7)
                lm_values = [float(n) for n in target_lm.split()]
                LM2 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-6)
                lm_values = [float(n) for n in target_lm.split()]
                LM3 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-5)
                lm_values = [float(n) for n in target_lm.split()]
                LM4 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-4)
                lm_values = [float(n) for n in target_lm.split()]
                LM5 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-3)
                lm_values = [float(n) for n in target_lm.split()]
                LM6 = array([lm_values[0], lm_values[1]], dtype = float)
                target_lm = linecache.getline(self.files[i], last_line-2)
                lm_values = [float(n) for n in target_lm.split()]
                LM7 = array([lm_values[0], lm_values[1]], dtype = float)

                LM1 = LM1 * scale
                LM2 = LM2 * scale
                LM3 = LM3 * scale
                LM4 = LM4 * scale
                LM5 = LM5 * scale
                LM6 = LM6 * scale
                LM7 = LM7 * scale

                # Calculate measurements

                WIS = abs(euclid_distance(LM1, LM7))
                WIM = abs(euclid_distance(LM2, LM6))
                WIB = abs(euclid_distance(LM3, LM5))
                LDC = abs(euclid_distance(LM1, LM4))
                RDC = abs(euclid_distance(LM7, LM4))
                OA = angle(side_1 = LDC, side_2 = RDC, side_3 = WIS)
                D = height(WIS, heron(side_1 = LDC, side_2 = RDC, side_3 = WIS))

                # create table

                
                new_row = {
                    "Photo_Name" : Photo_Name,
                    "File_Name" : self.files[i].rstrip(".tps"),
                    "WIS" : WIS,
                    "WIM" : WIM,
                    "WIB" : WIB,
                    "LDC" : LDC,
                    "RDC" : RDC,
                    "OA" : OA,
                    "D" : D
                }
                self.df = self.df.append(new_row, ignore_index = True)
                
            self.success_popup()
            self.bt_save_data.setEnabled(True)
    
    def save_table(self):
        self.df.to_csv("New_Measurements.csv", index = False, header = True, decimal = ".", sep = ",")

    def error_popup(self):
        msg = QMessageBox()
        msg.setWindowTitle("Error!")
        msg.setText("No .tps files were found in the selected directory")
        msg.setIcon(QMessageBox.Warning)
        
        x = msg.exec_()
    
    def success_popup(self):
        msg = QMessageBox()
        msg.setWindowTitle("Success!")
        msg.setText("Measurements calculated with success")
        msg.setIcon(QMessageBox.Information)

        x = msg.exec_()

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
