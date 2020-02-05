# RPi
import csv
import time
import paho.mqtt.client as mqtt
global tem, vib
def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("Connected with result code " + str(rc))
        print("connected OK")
        client.subscribe("/motor1/temp")
        client.subscribe("/motor1/vib")
    else:
        print("Bad connection Returned code=",rc)
def on_message(client, userdata, msg): 
    print(msg.topic+" "+str( msg.payload)) 
    if msg.topic == '/motor1/temp': 
        print("temperature is: "+str(msg.payload))
        tem=int(msg.payload)
        print (tem)
        temptime=[time.time(),tem]
        with open(r'temperature.csv', 'a') as f:
            writer = csv.writer(f)
            writer.writerow(temptime)
    if msg.topic == '/motor1/vib': 
        print("Vibration level is: "+str(msg.payload))
        vib=int(msg.payload)
        print (vib)      
        vibtime=[time.time(),vib]
        with open(r'vibration.csv', 'a') as f:
            writer = csv.writer(f)
            writer.writerow(vibtime)  
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
try:
    client.loop_start()
    print("Connecting to broker")
    client.connect('localhost', 1883, 60)     #connect to broker
    while 1:
        time.sleep(.2)
    print("in Main Loop")
except KeyboardInterrupt:
    client.loop_stop()    #Stop loop 
    client.disconnect() # disconnect