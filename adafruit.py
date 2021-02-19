#import libraries
from Adafruit_IO import *
from time import sleep
import RPi.GPIO as GPIO
import requests

GPIO.setmode(GPIO.BCM)
#Sets pins 25 and 22 as outputs
GPIO.setup(22, GPIO.OUT)
GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)
GPIO.setup(25, GPIO.OUT)

#Modes:
# [GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 0, 0] --> Default mode
# [GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 0, 1] --> Thunder 
#[GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 1, 0] --> Rain/Drizzle
#[GPIO22, GPIO23, GPIO24, GPIO25] = [0, 0, 1, 1] --> Snow
#[GPIO22, GPIO23, GPIO24, GPIO25] = [0, 1, 0, 0] --> Clear
#[GPIO22, GPIO23, GPIO24, GPIO25] = [0, 1, 0, 1] --> Clouds 


#This is sensitive data, will change with final pull, at the moment it doesnt matter
aio = Client('MrBoogle', 'aio_dhnt10VALnVRAZBubZlxi2KWDkDQ')




#fetches weather data
API_KEY = '31fd8c74992a33f319e0a38eb52f03c7';
CITY_QUERY = "Toronto, CA";
REQUEST_URL = "https://api.openweathermap.org/data/2.5/weather?q=" + CITY_QUERY + "&appid=" + API_KEY;
req = requests.get(
    REQUEST_URL
);

jsonMap = req.json()

#Always check if G Home is used
while True:
    dataIn = aio.receive('ifttt').value

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
    if int(weatherID / 100) == 2 and dataIn == 1:
        print("Thunder")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.HIGH)
        
    # Drizzle
    elif int(weatherID / 100) == 3:
        print("Drizzle")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.LOW)
    # Rain
    elif int(weatherID / 100) == 5:
        #rainBehaviour()
        print("Rain")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.LOW)
    # Snow
    elif int(weatherID / 100) == 6:
        print("Snow")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.HIGH)
    # Atmosphere
    elif int(weatherID / 100) == 7:
        print("Pplaceholder")
    # Clear
    elif int(weatherID) == 800:
        print("Clear")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)
    # Clouds
    elif int(weatherID / 10) == 80:
        print("Cloudy")
        GPIO.output(22, GPIO.LOW)
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.HIGH)



