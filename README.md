# RC-Sound_Modul for RC ships or cars

RC-Sound_Modul first based on the work done by Mulder_den and thnilsen on RCGroups 

Forum Mulder_den: https://www.rcgroups.com/forums/showthread.php?2372161-RC-Sound-module-for-20

Forum thnilsen: https://www.rcgroups.com/forums/showpost.php?p=36070567

and TMRpcm library :https://github.com/TMRh20/TMRpcm/wiki

# Features:
-Speed-dependent engine noise only one motor-loop wave file

-4 individual sounds

# SD card content:

The following files must be on the SD card

start.wav -> engine start

shut.wav  -> engine stop

loop.wav -> Movement noise (loop) 

Sound1.wav -> individual sound 1 (like Horn)

Sound2.wav -> individual sound 2 

Sound3.wav -> individual sound 3 

Sound4.wav -> individual sound 4 

Delay.txt -> seconds for Motor stop if standing noise (0 for deaktivate)

Speed.txt -> max samplerate for loop.wav when max RC-input
 
# description
To the engine start put the pin "Diesel" on gnd. To stop open the pin.
Motor stop even if the standing noise for x seconds (in Delay.txt) is pending. By giving gas, the engine is started again.
The sound start when the corresponding pin is set to gnd. The sound is played once

# circuit diagram
![](https://github.com/Ziege-One/RC-Sound_Modul/blob/master/doku/RC-Sound_Modul.png?raw=true)

# build pictures
![](https://github.com/Ziege-One/RC-Sound_Modul/blob/master/doku/Board%20from%20below.jpg?raw=true)
![](https://github.com/Ziege-One/RC-Sound_Modul/blob/master/doku/Board%20from%20above.jpg?raw=true)
![](https://github.com/Ziege-One/RC-Sound_Modul/blob/master/doku/Board%20from%20above%20with%20SD%20board.jpg?raw=true)
![](https://github.com/Ziege-One/RC-Sound_Modul/blob/master/doku/Finished%20housing.jpg?raw=true)

