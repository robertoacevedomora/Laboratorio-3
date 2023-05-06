import serial, time, json

fileName = "datos_labo.csv"
samples = 10
print_labels = False

ser = serial.Serial('COM3', 9600)
file = open(fileName,"a")
print("Creates file")

line = 0

while line <= samples:

    if print_labels:
        if line==0:
            print("Printing Column Headers")
        else:
            print("Line " + str(line) + ":writing...")
    getData = str(ser.readline())    
    data = getData[0:][:-2]
    print(data)
    file = open(fileName, "a")
    file.write(data + "\n")
    line = line + 1
    print("Data collection complete!");
