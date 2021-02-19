#import libraries
from Adafruit_IO import *
from time import sleep
import RPi.GPIO as GPIO
import requests

GPIO.setmode(GPIO.BCM)
#Sets pins 25 and 22 as outputs
GPIO.setup(25, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)

#Modes:
# [GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 0, 0] --> Default mode, shows relaxing aura based on time
#[GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 0, 1] --> Weather mode, triggered by G home and



#This is sensitive data, will change with final pull, at the moment it doesnt matter
aio = Client('MrBoogle', 'aio_tXHi39ob3RXqoI7p8aA0VafxN3Mt')


#fetches weather data
API_KEY = '31fd8c74992a33f319e0a38eb52f03c7';
CITY_QUERY = "Toronto, CA";
REQUEST_URL = "https://api.openweathermap.org/data/2.5/weather?q=" + CITY_QUERY + "&appid=" + API_KEY;
req = requests.get(
    REQUEST_URL
);

jsonMap = req.json()


try:
  weatherData = jsonMap["list"][0]["weather"][0]
except:
    try:
      weatherData = jsonMap["weather"][0]
    except:
        print("Wrong Data Received")

weatherID = weatherData["id"]

# Weather and Corresponding Ids (https://openweathermap.org/weather-conditions)
# Thunder Storm
if int(weatherID / 100) == 2 :
    print("Pplaceholder")
# Drizzle
elif int(weatherID / 100) == 3:
    print("Pplaceholder")
# Rain
elif int(weatherID / 100) == 5:
    rainBehaviour()
# Snow
elif int(weatherID / 100) == 6:
    print("Pplaceholder")
# Atmosphere
elif int(weatherID / 100) == 7:
    print("Pplaceholder")
# Clear
elif int(weatherID) == 800:
    print("Pplaceholder")
# Clouds
elif int(weatherID / 10) == 80:
    print("Pplaceholder")



while True:
    dataIn = aio.receive('ifttt').value
    if int(dataIn) == 1:
        #heres an example of setting an pin as High and low
        GPIO.output(25, GPIO.HIGH)
    else:
        GPIO.output(25, GPIO.LOW)
    
    GPIO.output(22, int(dataIn))
    print(dataIn)
    # set GPIO24 to 1/GPIO.HIGH/True  
    #sleep(0.5)                 # wait half a second  
    #GPIO.output(24, 0)         # set GPIO24 to 0/GPIO.LOW/False  
    #sleep(0.5) 