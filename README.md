# ESP8266-covid-19-tracker
This a IoT Device that shows current covid stats

Parts Required:
================
1. NodeMCU (ESP8266)
2. ST7735 Display

# How to get updated fingerprint of disease.sh
Go to [grc.com](https://www.grc.com/fingerprints.htm) site and paste the below link in the search bar and click Fingerprint Site.
```
disease.sh
```
![alt text](https://github.com/Sunil531999/IoT-based-Covid19-tracker/blob/main/grc.png)

# API
Here i have used the [disease.sh](https://disease.sh/) website API to make a HTTPS GET request

# Changes to be made for seeing country specific data
*Below is base URL*
```
https://disease.sh/v3/covid-19/countries/<country_name>
```
Replace the *<country_name>* to specific country's shorthand notation or the full country name without space
*Example*
For India use *in* America use *usa* for Germany use *de*
```
https://disease.sh/v3/covid-19/countries/<country_name>
```
# Image
![alt text](https://github.com/Sunil531999/IoT-based-Covid19-tracker/blob/main/image.jpg?raw=true)
