Gridquencer
===========

A sequencer for grid interfaces, scattering my attention between javascript, ableton, nodejs, and teensy microcontrollers. 

## Requirements

- Teensy 3.6 or 4.1 (should work with 4.0)
- USB Host cable and MIDI output (currently prototyping with attowatt i2c2midi eurorack module: https://github.com/attowatt/i2c2midi)
- https://formulae.brew.sh/formula/teensy_loader_cli
- Built with Platformio plugin for VS Code - https://platformio.org/install/ide?install=vscode

### Supported Devices (Aspiring)

- Ableton Push 1 (alpha)
- Ableton Push 2 (conforms to Push 1 MIDI)
- Novation Launchpad (roughed in but not enabled)

### Useful Links

- https://github.com/Carlborg/hardpush/
- https://structure-void.com/ableton-live-push-and-scripts/#usermode
- https://github.com/adamjmurray/ableton-launchpad-sequencer
- https://github.com/adamjmurray/ableton-midi-sculptor
- https://gist.github.com/adamjmurray/21d7d3ae1f2ef8c66a19

Features to add:
    - region modifications
    - shifting start points
    - change tempo
    - LCD screen showing messages for different contexts (notes, sequences)
    - selecting sequences with the button pads
    - note distribution functions 
    - save to json?
    - load from json?
    - definitions for various midi controllers
    - notes off when sequencer stops? 
