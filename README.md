Gridquencer
===========

A sequencer for grid interfaces, scattering my attention between javascript, ableton, nodejs, and teensy microcontrollers. 


## TODO

- Add JSON loading with ArduinoJSON library (should data be stored in objects as JSON too?)
- Sequence to JSON Exporting

## Pre-requisites: 

- https://formulae.brew.sh/formula/teensy_loader_cli


Installation
------------

Checkout repo

	> git clone git@github.com:drart/gridquencer.git

Install node.js modules:

	> npm install 

Set up IAC Bus 1
----------------

https://support.apple.com/en-ca/guide/audio-midi-setup/ams1013/mac

Running
-------

    > node gridquencer.js


Useful Links
------------

https://structure-void.com/ableton-live-push-and-scripts/#usermode

http://www.edsko.net/2020/12/26/trichords-part1/

http://www.edsko.net/2020/12/27/trichords-part2/

https://github.com/adamjmurray/ableton-push.js

https://github.com/adamjmurray/ableton-launchpad-sequencer

https://github.com/adamjmurray/ableton-midi-sculptor

https://gist.github.com/adamjmurray/21d7d3ae1f2ef8c66a19

https://github.com/gluon/AbletonLive9_RemoteScripts/blob/master/Push/StepSeqComponent.py

https://github.com/ffont/push2-python#interface-with-the-display-static-content





ableton-push-canvas-display
https://afterglow-guide.deepsymmetry.org/afterglow/readme#introduction

https://blog.logrocket.com/reading-writing-json-files-nodejs-complete-tutorial/
https://nodejs.org/en/knowledge/command-line/how-to-parse-command-line-arguments/

Features to add:
    - region modifications
    - note distribution functions 
    - save to json?
    - load from json?
    - definitions for various midi controllers
    - notes off when sequencer stops? 
