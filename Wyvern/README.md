# Conversion of a Norwich C356

This conversion use multiple Arduinos (Arduini?) connected via Serial (RX/TX pins) to each other, and then a Pro Micro on the front of it as the single USB output to the computer.

Because I hate soldering - because my eyes are old - I prefer to use the Mega or Leonardo with a screw shield. This project could be done much more cheaply and efficiently with scan matrices, but I really can't be bothered with the stress of soldering badly.

- wyvern-manuals-matrix-v1 - this is for 8x8 keyboard encoding, designed for a Leonardo board (Pro Micro or standard Leonardo)
- mega_pistons - one mega, one thumb or toe piston per wire, also has 2 potentiometers for swell shoes
- midi-pipe-serial - for the "USB interface" Pro Micro - take whatever it goes over serial and pass it on