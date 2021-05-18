from flask import Flask, render_template
import serial

ser = serial.Serial( # STM32가 연결된 Serial 설정 
	port='COM4',
	baudrate=115200, 
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1)

app = Flask(__name__)

def led_on():
	ser.write(b"\x02LED ON\x03")
	return

def led_off():
	ser.write(b"\x02LED OFF\x03")
	return

@app.route('/')
def index():
	led_off()
	return render_template('index.html', state='OFF')
	
@app.route('/on/')
def on():
	led_on()
	print ('Switch On')
	return render_template('index.html', state='ON')

@app.route('/off/')
def off():
	led_off()
	print ('Switch Off')
	return render_template('index.html', state='OFF')
		
if __name__=='__main__':
	print('Web Server Starts')
	app.run(debug=True, use_reloader=False, host='127.0.0.1', port=8000)
	print ('Web Server Ends')