# A Python TDD session

This script was developed using `unittest` framework to learn TDD with Python.

This is an exercise in applying TDD principles to a real-life problem in Python.

## Wav2mp3

The original problem is to convert all the wav files in a folder to mp3. I have `ffmpeg` installed so it does the actual job of converting very well- all I needed was to run ffmpeg on each file and collate the generated output files.

I could have written a simple bash-script (or run commands by hand). Instead I decided to write a python script to generate the `ffmpeg` commands and then decided to use TDD to built the Python Class step-by-step. So this activity became a lot about learning python and practicing TDD and very little about converting media files.
