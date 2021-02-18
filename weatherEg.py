def rainBehaviour() :
    pixels.setBrightness(100)
    even = True;
    odd = False;

    while True :
        for i in range(51):
            if i % 2 == 0 :
                if even :
                    pixels.setPixelColor( i, pixels.Color(65, 105, 225) )
                else :
                    pixels.setPixelColor( i, pixels.Color(20, 20, 205) )
                even = not even
            else :
                if odd :
                    pixels.setPixelColor( i, pixels.Color(65, 105, 225) )
                else :
                    pixels.setPixelColor( i, pixels.Color(20, 20, 205) )
                odd = not odd
            pixels.show()

        pixels.show()
        pixels.delay(3)


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
