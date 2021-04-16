import requests
import xml.etree.ElementTree as ET

API_key = "45aa09b8550d9cf1ff2179647bb4a402"
base_url = "http://api.openweathermap.org/data/2.5/weather?"
city_name = input("Enter City Name : ")
request_url = base_url + "appid=" + API_key + "&q=" + city_name + "&units=metric"

weather_data = requests.get(request_url).json()

weather = ET.Element('weather')
precipitation = ET.SubElement(weather, 'precipitation')
windSpeed = ET.SubElement(weather, 'windSpeed')
temperature = ET.SubElement(weather, 'temperature')
cloudiness = ET.SubElement(weather, 'cloudiness')

precipitation.text = str(weather_data['clouds']['all'] * 2.86)
windSpeed.text = str(weather_data['wind']['speed'])
temperature.text = str(weather_data['main']['temp'])
cloudiness.text = str(weather_data['clouds']['all'])

mydata = ET.tostring(weather, encoding="unicode")
myfile = open("bin/data/metrics.xml", "w")
myfile.write(mydata)

print('\nTemperature : ',temperature.text)
print('\nWind Speed : ',windSpeed.text)
print('\nPrecipitation : ',precipitation.text)
print('\nCloudiness : ',cloudiness.text)
