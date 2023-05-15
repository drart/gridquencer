Gridquencer
===========

A sequencer for grid interfaces, scattering my attention between javascript, ableton, nodejs, and teensy microcontrollers. 

## Requirements

- https://formulae.brew.sh/formula/teensy_loader_cli
- Built with Platformio plugin for VS Code - https://platformio.org/install/ide?install=vscode

### Supported Devices

- Ableton Push 1 (alpha)
- Ableton Push 2 (conforms to Push 1 MIDI)
- Novation Launchpad (roughed in but not enabled)

### Useful Links

- https://github.com/Carlborg/hardpush/
- https://structure-void.com/ableton-live-push-and-scripts/#usermode
- http://www.edsko.net/2020/12/26/trichords-part1/
- http://www.edsko.net/2020/12/27/trichords-part2/
- https://github.com/adamjmurray/ableton-push.js
- https://github.com/adamjmurray/ableton-launchpad-sequencer
- https://github.com/adamjmurray/ableton-midi-sculptor
- https://gist.github.com/adamjmurray/21d7d3ae1f2ef8c66a19
- https://github.com/gluon/AbletonLive9_RemoteScripts/blob/master/Push/StepSeqComponent.py

Features to add:
    - region modifications
    - note distribution functions 
    - save to json?
    - load from json?
    - definitions for various midi controllers
    - notes off when sequencer stops? 
