# VivalPi — Generative Music Composition through Live Meteorological Data Sonification

Three centuries ago, Italian baroque composer Antonio Vivaldi wrote one of the most popular compositions of classical music: _Le Quattro Stagioni_ ("The Four Seasons"). The representation of seasonal patterns in music brought along a revolution in musical conception. Vivaldi's arrangement consisted of a total of four concertos, each named after the season it expressed. _La Primavera_ ("Spring") represents flowing creeks and the chirping of birds, _L'estate_ ("Summer") expresses the merciless heat of the sun, _L'autunno_ ("Autumn") celebrates the rich harvests and mild air, finishing the concerto with the icy snow of _L'inverno_ ("Winter"). On the three-hundredth anniversary of Vivaldi's masterpiece, this project asks the listener to interact with their understanding of seasonal changes in a more personal way, and to engage with a tangible representation of our changing climate.

The Maximilian library allows for the computation of code-written compositions where these weather variables can be used. The program accesses weather data through API's such as OpenWeatherMap, and applies these metrics to the generated composition. The audio-synthesis can either be played on the device by using the built-in 3.5mm audio-jack, or transmitted through a server to the web. The latter would complicate the project but allow for world-wide access, as well as offer interesting possibilities such as using the browser as a medium for a visual representation of the composition. This project utilises contemporary technologies such as sonification, real-world quantification, and computation. The goal is for the music to show significant differences in its composition between summer and winter, and between a rainy day and a sunny day, allowing listeners to perceive weather in an audible format.

## Technical Approach

The project holds a series of `maximilian` classes such as `maxiOsc`, `maxiFilter`, `maxiClock`, as well as an `xml` file using `ofxXmlSettings` to read its contents. Furthermore, the script declares a series of `Instrument` classes, such as `rain`, `wind`, and `bass` that are part of the composition.

The user first must run the `getMetrics.py` script, where they are prompted with the question for which city/location they would like to fetch weather metrics, i.e. `London`. The python script then fetches the weather metrics using the OpenWeatherMap API and stores these metrics in the `metrics.xml` file. These metrics can then be accessed by the OpenFrameworks app.

When the sketch is executed, it will start with starting the clocks, using `clock.ticker()` and keep track of a `playhead`. The main body of the `ofApp::audioOut()` method is a series of logical operations that allow for the composition to remain pleasing to the ear over time and not sound too repetitive.

On every 256th beat (`playhead % 256 == 0`), the `setMotif()` method is called that changes the motif based on the `temperature` metric. When temperatures are low, the `notes` and `chords` will have a low range on low octaves with little deviation. When the `temperature` is high, the `notes` and `chords` will go up. With this given range, the `randomInt(notesRange.x, notesRange.y)` will fill the notes array with random `midi` notes within the calculated range. The same logic applies for the chords. This motif is used for both the `bass` instrument and the `lead` melody. The notes of the `bass` instrument are set through the `Instrument::setFrequency()` method as:

```
bass.setFrequency(bass_filter.hires(mtof.mtof(notes[playhead % 16] + chords[bass_chord % 8]), bass_cutoff, bass_resonance));
```

When the `playhead` hits each 64th beat, and passes a random check (`randomFloat() > 0.15`) the booleans that keep track of what instrument should be in the arrangement are updated:

```
wind_isPlaying = randomFloat() < ofMap(windSpeed, windSpeed.getMin(), windSpeed.getMax(), 0.0, 1.0) ? true : false;

rain_isPlaying = randomFloat() < ofMap(precipitation, precipitation.getMin(), precipitation.getMax(), 0.0, 1.0) ? true : false;

bass_isPlaying = randomFloat() < 0.70 ? true : false;
beat_isPlaying = randomFloat() < 0.80 ? true : false;

lead_isPlaying = randomFloat() < ofMap(cloudiness, cloudiness.getMin(), cloudiness.getMax(), 1.0, 0.0) ? true : false;
```

For example: the boolean `wind_isPlaying` will be true if `randomFloat()` is smaller then the scaled presence of `windSpeed`. In other words: if `windSpeed` is little, `wind_isPlaying` will be more likely to be `false`, and vice versa. This same principle is applied on `rain_isPlaying` (using `precipitation`), and `lead_isPlaying` (using `cloudiness`). `bass_isPlaying` and `beat_isPlaying` are not weather dependant, and are relatively unlikely to be left out of the arrangement.

At last, the audio buffer that will be processed as output is dependant on the following logic:

```
float beat = (kick.play() + (snare.play() * snare_volume) + (hihat.play() * hihat_volume)) * beat_volume;

arrangement = (beat_isPlaying * beat) + (wind_isPlaying * wind.play()) + (rain_isPlaying * rain.play()) + (bass_isPlaying * bass.play()) + (lead_isPlaying * (lead.play() * lead_volume));

float out = arrangement;
```

By using boolean multiplication the arrangement is assigned to whatever instruments 'should' be playing now, which is then passed as an output to the output buffer.

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
