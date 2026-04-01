# TODO

- ~~sd file reading~~
- ~~sd file writing~~
- ~~playing sounds~~
- ~~reading wav header~~ 
- ~~playing wav file~~
- play / pause button
- stop button
- skip song button
- volume buttons
- song queue
- file system
- select song
- settings file
- song data type
- safe power off


## Power saving measures to implement

### CPU
- reduce clock speed (recalc PIO values)
- use __wfi, wait for interupt
- sleep cores?

### sd
- add audio buffer cache (4? read ahead caches)
- use a metadata file on the sd card but keep some songs in ram and use a scrolling window cache
- only update window cache when near edge

### dac
- mute dac when music paused
- stop dma transfer when music paused
- enter dac low power mode when no music playing
- power down dac components when no music playing

### display
- dim display when no buttons pressed for set time
- turn off display when no buttons pressed for set time and no music playing
- turn off device when display is off for set amount of time
- only redraw the display when something changes (might be frequent enough with progress bar?)

### other
- disable the wifi chip (might later be need, is used for the led?)


