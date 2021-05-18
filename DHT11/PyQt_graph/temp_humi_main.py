from PyQt5 import QtCore, QtGui, QtWidgets
from temp_humi import *
import serial
import threading
import pyqtgraph as pg
from PyQt5.QtCore import pyqtSignal

temp_list = [0]
humi_list = [0]

ser = serial.Serial( # stm32와 연결된 Serial 설정 
	port='COM13',
	baudrate=115200, 
	parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1)

class draw_graph(QtWidgets.QMainWindow, Ui_MainWindow):
	global temp_list, humi_list
	uiUpdateDelegate = pyqtSignal(int)

	def __init__(self, parent=None): # 설정 함수
		QtWidgets.QMainWindow.__init__(self, parent=parent)
		self.setupUi(self)
		self.uiUpdateDelegate.connect(self.uiUpdater) # 인터럽트 함수
		self.graphicsView.setBackground('#FFFFFF') # 배경 색깔
		self.graphicsView.showGrid(x=True, y=True) # 그리드
		self.graphicsView.setRange(xRange=[2, 20])
		self.graphicsView.setRange(yRange=[10, 100])
	
	def uiUpdater(self):
		self.graphicsView.clear() # 그래프 지우기
		self.graphicsView.plot(temp_list, pen=pg.mkPen('r', width=2), # 온도
			style=QtCore.Qt.DashLine, symbol=('o'), symbolBrush='r')
		self.graphicsView.plot(humi_list, pen=pg.mkPen('b', width=3), # 습도
			style=QtCore.Qt.DashLine, symbol='x', symbolBrush='b')

def read_thread(ui):
	global temp_list, humi_list
	cnt = 0
	ser.flush()
	while True:
		data = ser.readline().decode('utf-8')
		
		if data:
			if data == "DHT11_ERROR\r\n":
				print(data)
			else:
				data = data.split(" ")
				temp = int(data[1])
				humi = int(data[3])

				ui.label_value_temp.setText(str(temp)) # 라벨에 온도 표시
				temp_list.append(temp) # 리스트에 붙이기
				ui.label_value_humi.setText(str(humi))
				humi_list.append(humi)

				cnt=cnt+1
				if(cnt>20): # 30개 데이터 자르기
					temp_list.pop(0)
					humi_list.pop(0)
					cnt=21
					
				ui.uiUpdateDelegate.emit(1) # updater 호출

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    
    ui = draw_graph()
    ui.show()

    th = threading.Thread(target=read_thread, args=(ui,)) # 스레드 설정, read_thread함수에 인자로 ui를 넘겨준다
    th.daemon = True
    th.start()

    sys.exit(app.exec_())