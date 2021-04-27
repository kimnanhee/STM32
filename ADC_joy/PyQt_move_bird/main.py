from draw import *
from PyQt5 import QtCore, QtGui, QtWidgets
import serial
import threading
from time import sleep

ser = serial.Serial(
	port='COM3', # 아두이노 연결 COM 포트
	baudrate=115200, 
	parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1)


def read_thread(ui):
    bird_x = 300
    bird_y = 190
    while True:
        data = ser.readline()
        # print(data[2:6], data[9:13])
        bird_x += (int(data[2:6]) - 2819) / 200
        bird_y += (int(data[9:13]) - 2715) / 200

        if(bird_x > 650):
            bird_x = 650
        if(bird_y > 350):
            bird_y = 350
        if(bird_x < 0):
            bird_x = 0
        if(bird_y < 0):
            bird_y = 0

        # print(bird_x, bird_y) # 확인용 받아온 데이터 출력
        ui.label_bird.move(bird_x, bird_y)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)

    th = threading.Thread(target=read_thread, args=(ui,)) # 스레드 설정, read_thread함수에 인자로 ui를 넘겨준다
    th.daemon = True
    th.start()

    MainWindow.show()
    sys.exit(app.exec_())