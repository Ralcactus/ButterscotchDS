# Super basic port of Butterscotch to the DS
Theres a 80% chance this isn't getting finished, just doing this more so for fun and learn more about ds programming

## How to compile (un-finished)
This project use's makefiles because I HATE CMAKE WITH EVERY CELL OF MY BODY<br>
just run make in the root and if you have devkitpro correctly setup, compile fine!

put the data.win and pre-processed sprites into:
sd:/NDS/butterscotch/

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
