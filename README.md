# VivalPi — Generative Music Composition through Live Meteorological Data Sonification

Three centuries ago, Italian baroque composer Antonio Vivaldi wrote one of the most popular compositions of classical music: _Le Quattro Stagioni_ ("The Four Seasons"). The representation of seasonal patterns in music brought along a revolution in musical conception. Vivaldi's arrangement consisted of a total of four concertos, each named after the season it expressed. _La Primavera_ ("Spring") represents flowing creeks and the chirping of birds, _L'estate_ ("Summer") expresses the merciless heat of the sun, _L'autunno_ ("Autumn") celebrates the rich harvests and mild air, finishing the concerto with the icy snow of _L'inverno_ ("Winter"). On the three-hundredth anniversary of Vivaldi's masterpiece, this project asks the listener to interact with their understanding of seasonal changes in a more personal way, and to engage with a tangible representation of our changing climate.

The Maximilian library allows for the computation of code-written compositions where these weather variables can be used. The program accesses weather data through API's such as OpenWeatherMap, and applies these metrics to the generated composition. The audio-synthesis can either be played on the device by using the built-in 3.5mm audio-jack, or transmitted through a server to the web. The latter would complicate the project but allow for world-wide access, as well as offer interesting possibilities such as using the browser as a medium for a visual representation of the composition. This project utilises contemporary technologies such as sonification, real-world quantification, and computation. The goal is for the music to show significant differences in its composition between summer and winter, and between a rainy day and a sunny day, allowing listeners to perceive weather in an audible format.

## Technical Infrastructure

## Panopto Recording

The Panopto Recording explaining the code can be found on [this link](https://ual.cloud.panopto.eu/Panopto/Pages/Viewer.aspx?id=a093115f-4771-4062-8ad8-ad0c010c9bb7) but this link is only accessible if you have access to the UAL environment.

## Demo Video

I did three tests for the locations of London, Tokyo and Oslo and the results can be found on [this Vimeo](https://vimeo.com/537779883).

## Resources

- Maximilian
  - [Repository](https://github.com/micknoise/Maximilian)
  - [Web Audio Guide](https://mimicproject.com/guides/maximJS)
- Weather Data
  - [OpenWeatherMap API](https://openweathermap.org/api)
  - [Extract from Google Weather with `bs4`](https://www.thepythoncode.com/article/extract-weather-data-python)
  - [Sentiment Analysis article by Ashley Ng](https://medium.com/@xyng17/scraping-twitter-and-sentiment-analysis-using-python-c5a44b9288ab)
- Inspiration
  - [Four Tet's Generative Audio and Video Livestream](https://youtu.be/Ty6m-tL5iQY)
  - [Programming as Performance by Sam Aaron](https://youtu.be/TK1mBqKvIyU)
- Le Quattro Stagioni by Vivaldi
  - [Spotify](https://open.spotify.com/album/0i56ngy4MMo44H5vDbBRTZ?si=UYLPflksT3-Z3BXsp5qRkA)
  - [Apple Music](https://music.apple.com/nl/album/vivaldi-the-four-seasons/883897569?l=en)
