import serial, time, json

arduino = serial.Serial('COM3', 9600)
time.sleep(1)
while True:
    string = arduino.readline()
    print(string)
    print("********")
   # arduino.close()
