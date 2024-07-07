# ESP32-Weather-station
ESP32 weather station that queries the OpenWeatherMap service and displays the retrieved data on a web page.

![image](https://github.com/iamex/ESP32-Weather-station/assets/26520401/e33370f6-65b8-4829-942d-03a05fbe54c5)


## Description
In this project, an ESP32 is used to query OpenWeatherMap's services to fetch weather data for a city in JSON format. The retrieved data is then displayed on a local web page at the address "weather-station.local". The displayed information includes temperature, humidity, weather description, sunrise, sunset, and more. The ESP32-WROOM-32 was programmed using PlatformIO.
### Possible upgrades
* Display the data on an Color TFT LCD display module
* Connect actuators, relays that intervene based on the received weather data. For example motorized shutter, automatic boiler on/off, twilight switch, automatic irrigation pump for plants etc.
