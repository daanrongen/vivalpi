# VivalPi — Generative Music Composition through Live Meteorological Data Sonification
Three centuries ago, Italian baroque composer Antonio Vivaldi wrote one of the most popular compositions of classical music: *Le Quattro Stagioni* ("The Four Seasons"). The representation of seasonal patterns in music brought along a revolution in musical conception. Vivaldi's arrangement consisted of a total of four concertos, each named after the season it expressed. *La Primavera* ("Spring") represents flowing creeks and the chirping of birds, *L'estate* ("Summer") expresses the merciless heat of the sun, *L'autunno* ("Autumn") celebrates the rich harvests and mild air, finishing the concerto with the icy snow of *L'inverno* ("Winter"). On the three-hundredth anniversary of Vivaldi's masterpiece, this project asks the listener to interact with their understanding of seasonal changes in a more personal way, and to engage with a tangible representation of our changing climate. A musical composition will be generated and broadcast live from a Raspberry Pi, using real-time meteorological data as harmonic variables.

The Maximilian library, as well as the coding environment Sonic Pi, allows for the computation of code-written compositions where these weather variables can be used. The program accesses weather data through API's such as OpenWeatherMap, and applies these metrics to the generated composition. The audio-synthesis can either be played on the device by using the built-in 3.5mm audio-jack, or transmitted through a server to the web. The latter would complicate the project but allow for world-wide access, as well as offer interesting possibilities such as using the browser as a medium for a visual representation of the composition. This project utilises contemporary technologies such as sonification, real-world quantification through sensors, datasets, computation, and the web. The music will show significant differences in its composition between summer and winter, and between a rainy day and a sunny day, allowing listeners to perceive weather in an audible format. VivalPi pays a tribute to the virtuoso and aims to put his works in a contemporary context by using data and computation.

## Technical Infrastructure
in progress

## Resources
- Maximilian
    - [Repository](https://github.com/micknoise/Maximilian)
    - [Web Audio Guide](https://mimicproject.com/guides/maximJS)
- Sonic Pi
    - [Website](https://sonic-pi.net/)
    - [Repository](https://github.com/sonic-pi-net/sonic-pi)
    - [Live Coding Tutorial by Sam Aaron](https://youtu.be/G1m0aX9Lpts)
    - [DEV.to article by Fen Slattery](https://dev.to/sublimemarch/today-i-learned-how-to-generate-music-with-sonic-pi-566g)
- Weather Data
    - [OpenWeatherMap API](https://openweathermap.org/api)
    - [Extract from Google Weather with `bs4`](https://www.thepythoncode.com/article/extract-weather-data-python)
    - [Sentiment Analysis article by Ashley Ng](https://medium.com/@xyng17/scraping-twitter-and-sentiment-analysis-using-python-c5a44b9288ab)
- Broadcasting
    - [Using `ffmpeg` and `rtmp` for YouTube](https://drjohnstechtalk.com/blog/2019/04/live-stream-to-youtube-from-a-raspberry-pi-webcam/)
    - [Using Docker and `rtmp` for YouTube](https://tutorials-raspberrypi.com/raspberry-pi-youtube-live-streaming-setup/)
- Inspiration
    - [Four Tet's Generative Audio and Video Livestream](https://youtu.be/Ty6m-tL5iQY)
    - [Programming as Performance by Sam Aaron](https://youtu.be/TK1mBqKvIyU)
- Le Quattro Stagioni by Vivaldi
    - [Spotify](https://open.spotify.com/album/0i56ngy4MMo44H5vDbBRTZ?si=UYLPflksT3-Z3BXsp5qRkA)
    - [Apple Music](https://music.apple.com/nl/album/vivaldi-the-four-seasons/883897569?l=en)