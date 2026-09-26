from PyQt5 import QtCore, QtGui, QtWidgets
from anyio import Path
import image
import firebase_admin
from firebase_admin import credentials, firestore

json_path = Path(__file__).parent / "my-project-rtos-firebase-adminsdk-fbsvc-fed8d50283.json"
cred = credentials.Certificate(str(json_path))
firebase_admin.initialize_app(cred)

db = firestore.client()

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(828, 614)
        font = QtGui.QFont()
        font.setPointSize(9)
        MainWindow.setFont(font)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(-40, -30, 731, 211))
        self.label.setStyleSheet("image: url(:/myimage/1. Logo HCM-UTE-20260306T034258Z-3-001/1. Logo HCM-UTE/B_Tagline - HCM-UTE - 1.png);")
        self.label.setText("")
        self.label.setObjectName("label")
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setGeometry(QtCore.QRect(-11, -11, 841, 161))
        self.frame.setStyleSheet("background-color: white")
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.label_2 = QtWidgets.QLabel(self.frame)
        self.label_2.setGeometry(QtCore.QRect(680, 0, 161, 161))
        self.label_2.setStyleSheet("image: url(:/myimage/1. Logo HCM-UTE-20260306T034258Z-3-001/logo_eee.png);")
        self.label_2.setText("")
        self.label_2.setObjectName("label_2")
        self.frame_2 = QtWidgets.QFrame(self.centralwidget)
        self.frame_2.setGeometry(QtCore.QRect(-20, 140, 851, 80))
        self.frame_2.setStyleSheet("background-color: #0072B9;")
        self.frame_2.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame_2.setObjectName("frame_2")
        self.label_3 = QtWidgets.QLabel(self.frame_2)
        self.label_3.setGeometry(QtCore.QRect(100, 10, 650, 70))
        font = QtGui.QFont()
        font.setPointSize(23)
        font.setBold(True)
        font.setWeight(75)
        self.label_3.setFont(font)
        self.label_3.setStyleSheet("color: white")
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(330, 220, 151, 61))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_4.setFont(font)
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(80, 300, 71, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(80, 350, 71, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")
        self.label_7 = QtWidgets.QLabel(self.centralwidget)
        self.label_7.setGeometry(QtCore.QRect(80, 400, 71, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_7.setFont(font)
        self.label_7.setObjectName("label_7")
        self.label_8 = QtWidgets.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(80, 500, 71, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_8.setFont(font)
        self.label_8.setObjectName("label_8")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.label_9.setGeometry(QtCore.QRect(80, 450, 71, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_9.setFont(font)
        self.label_9.setObjectName("label_9")
        self.input1 = QtWidgets.QLineEdit(self.centralwidget)
        self.input1.setGeometry(QtCore.QRect(190, 300, 241, 31))
        self.input1.setObjectName("input1")
        self.input2 = QtWidgets.QLineEdit(self.centralwidget)
        self.input2.setGeometry(QtCore.QRect(190, 350, 241, 31))
        self.input2.setObjectName("input2")
        self.input3 = QtWidgets.QLineEdit(self.centralwidget)
        self.input3.setGeometry(QtCore.QRect(190, 400, 241, 31))
        self.input3.setObjectName("input3")
        self.input4 = QtWidgets.QLineEdit(self.centralwidget)
        self.input4.setGeometry(QtCore.QRect(190, 450, 241, 31))
        self.input4.setObjectName("input4")
        self.input5 = QtWidgets.QLineEdit(self.centralwidget)
        self.input5.setGeometry(QtCore.QRect(190, 500, 241, 31))
        self.input5.setObjectName("input5")
        self.label_10 = QtWidgets.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(600, 270, 150, 40))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.label_10.setFont(font)
        self.label_10.setStyleSheet("background-color: #fff3a4;\n"
"border-radius: 10px;\n"
"border: 2px solid #8fd3ff;\n"
"padding: 4px;")
        self.label_10.setAlignment(QtCore.Qt.AlignCenter)
        self.label_10.setObjectName("label_10")
        self.group_preview = QtWidgets.QFrame(self.centralwidget)
        self.group_preview.setGeometry(QtCore.QRect(550, 260, 251, 231))
        self.group_preview.setStyleSheet("background-color: #d9f2ff;\n"
"border-radius: 15px;\n"
"border: 2px solid #8fd3ff;")
        self.group_preview.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.group_preview.setFrameShadow(QtWidgets.QFrame.Raised)
        self.group_preview.setObjectName("group_preview")
        self.label_11 = QtWidgets.QLabel(self.group_preview)
        self.label_11.setGeometry(QtCore.QRect(20, 60, 70, 21))
        self.label_11.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.label_11.setObjectName("label_11")
        self.label_12 = QtWidgets.QLabel(self.group_preview)
        self.label_12.setGeometry(QtCore.QRect(20, 90, 70, 21))
        self.label_12.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.label_12.setObjectName("label_12")
        self.label_13 = QtWidgets.QLabel(self.group_preview)
        self.label_13.setGeometry(QtCore.QRect(20, 120, 70, 21))
        self.label_13.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.label_13.setObjectName("label_13")
        self.label_14 = QtWidgets.QLabel(self.group_preview)
        self.label_14.setGeometry(QtCore.QRect(20, 150, 70, 21))
        self.label_14.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.label_14.setObjectName("label_14")
        self.label_15 = QtWidgets.QLabel(self.group_preview)
        self.label_15.setGeometry(QtCore.QRect(20, 180, 70, 21))
        self.label_15.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.label_15.setObjectName("label_15")
        self.val1 = QtWidgets.QLabel(self.group_preview)
        self.val1.setGeometry(QtCore.QRect(100, 57, 160, 25))
        self.val1.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.val1.setObjectName("val1")
        self.val2 = QtWidgets.QLabel(self.group_preview)
        self.val2.setGeometry(QtCore.QRect(100, 87, 160, 25))
        self.val2.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.val2.setObjectName("val2")
        self.val3 = QtWidgets.QLabel(self.group_preview)
        self.val3.setGeometry(QtCore.QRect(100, 117, 160, 25))
        self.val3.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.val3.setObjectName("val3")
        self.val4 = QtWidgets.QLabel(self.group_preview)
        self.val4.setGeometry(QtCore.QRect(100, 147, 160, 25))
        self.val4.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.val4.setObjectName("val4")
        self.val5 = QtWidgets.QLabel(self.group_preview)
        self.val5.setGeometry(QtCore.QRect(100, 177, 160, 25))
        self.val5.setStyleSheet("font-size: 12pt;\n"
"border: none;\n"
"background: transparent;")
        self.val5.setObjectName("val5")
        self.btnSend = QtWidgets.QPushButton(self.centralwidget)
        self.btnSend.setGeometry(QtCore.QRect(620, 500, 110, 40))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.btnSend.setFont(font)
        self.btnSend.setStyleSheet("QPushButton#btnSend {\n"
"    background-color: #D3D3D3;   /* light gray */\n"
"    color: black;\n"
"    border: 1px solid #999;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#btnSend:hover {\n"
"    background-color: #C0C0C0;   /* hover */\n"
"}\n"
"\n"
"QPushButton#btnSend:pressed {\n"
"    background-color: #A9A9A9;   /* khi bấm */\n"
"}")
        self.btnSend.setObjectName("btnSend")
        self.group_preview.raise_()
        self.frame_2.raise_()
        self.frame.raise_()
        self.label.raise_()
        self.label_4.raise_()
        self.label_5.raise_()
        self.label_6.raise_()
        self.label_7.raise_()
        self.label_8.raise_()
        self.label_9.raise_()
        self.input1.raise_()
        self.input2.raise_()
        self.input3.raise_()
        self.input4.raise_()
        self.input5.raise_()
        self.label_10.raise_()
        self.btnSend.raise_()
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 828, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        # connect input → preview realtime
        self.input1.textChanged.connect(self.updatePreview)
        self.input2.textChanged.connect(self.updatePreview)
        self.input3.textChanged.connect(self.updatePreview)
        self.input4.textChanged.connect(self.updatePreview)
        self.input5.textChanged.connect(self.updatePreview)

        self.btnSend.clicked.connect(self.sendData)

        # Enter để chuyển ô
        self.input1.returnPressed.connect(lambda: self.input2.setFocus())
        self.input2.returnPressed.connect(lambda: self.input3.setFocus())
        self.input3.returnPressed.connect(lambda: self.input4.setFocus())
        self.input4.returnPressed.connect(lambda: self.input5.setFocus())

        # Enter ở ô cuối → gửi luôn
        self.input5.returnPressed.connect(self.sendData)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "RTOS Firestore Control - Group 1"))
        MainWindow.setWindowIcon(QtGui.QIcon("icon.png"))
        self.label_3.setText(_translate("MainWindow", "RTOS Firestore Control - Group 1"))
        self.label_4.setText(_translate("MainWindow", "Send Data"))
        self.label_5.setText(_translate("MainWindow", "Data 1: "))
        self.label_6.setText(_translate("MainWindow", "Data 2: "))
        self.label_7.setText(_translate("MainWindow", "Data 3: "))
        self.label_8.setText(_translate("MainWindow", "Data 5: "))
        self.label_9.setText(_translate("MainWindow", "Data 4: "))
        self.label_10.setText(_translate("MainWindow", "Preview "))
        self.label_11.setText(_translate("MainWindow", "Data 1:"))
        self.label_12.setText(_translate("MainWindow", "Data 2:"))
        self.label_13.setText(_translate("MainWindow", "Data 3:"))
        self.label_14.setText(_translate("MainWindow", "Data 4:"))
        self.label_15.setText(_translate("MainWindow", "Data 5:"))
        self.val1.setText(_translate("MainWindow", "Value 1"))
        self.val2.setText(_translate("MainWindow", "Value 2"))
        self.val3.setText(_translate("MainWindow", "Value 3"))
        self.val4.setText(_translate("MainWindow", "Value 4"))
        self.val5.setText(_translate("MainWindow", "Value 5"))
        self.btnSend.setText(_translate("MainWindow", "Send"))

    def updatePreview(self):    
        self.val1.setText(self.input1.text())
        self.val2.setText(self.input2.text())
        self.val3.setText(self.input3.text())
        self.val4.setText(self.input4.text())
        self.val5.setText(self.input5.text())

    def sendData(self):    
        data = {
                "data1": self.input1.text(),
                "data2": self.input2.text(),
                "data3": self.input3.text(),
                "data4": self.input4.text(),
                "data5": self.input5.text()
        }

        db.collection("sensor_data").document("device1").update(data)

        print("Đã gửi:", data)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
