# Super basic port of Butterscotch to the DS
Theres a 80% chance this isn't getting finished, just doing this more so for fun and learn more about ds programming

## How to compile (un-finished)
This project use's makefiles because I HATE CMAKE WITH EVERY CELL OF MY BODY<br>
just run make in the root and if you have devkitpro correctly setup, compile fine!

## Current progress
- UNDERTALE boots and runs fails something at the first froggit when toriel appears
- Force skipping forward in undertale overworld runs fullspeed (battles 1-2 fps)
- Inputs
- Basic sprite renderer
- Pre-processer works
- DELTARUNE boots

## TO-DO (Highest to lowest priorty)
- ~~Get undertale booting~~ ✔
- ~~Get undertale to the first room (currently gets stuck at "Loaded "UNDERTALE")~~ ✔
- ~~Create a pre-processer (currently you have to dump each texture page yourself via utmt)~~ ✔
- finish the ndslib render backend
- ~~Get deltarune booting~~ ✔
- Write the full ndslib audio backend

## Compatibility List (un-finished)
Undertale - Loads, Flowey crashes but can be avoided if you skip their dialog fast enough and crashes with the first froggit.
DELTARUNE Chapter 1 - Works, but vessel creation takes at minimum 35 minutes. Crashes when attempting to load save files?
DELTARUNE Chapter 2 - Crashes on the subheadings (emulator) and crashes when leaving Kris' and Asriel's room. 
Pizza Tower - Too big for it to read... (skips most of the important code, making it think its the wrong bytecode)
Undertale Yellow - Crashes on trying to parse rooms?
