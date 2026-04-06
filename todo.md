# TODO

- ~~sd file reading~~
- ~~sd file writing~~
- ~~playing sounds~~
- ~~reading wav header~~ 
- ~~playing wav file~~
### next TODO
- play / pause function
- stop function
- reset song
- ~~dac register volume functions~~
- higher volume functions (audio_player.c)
### next TODO
- song queue
- skip song function
- skip song button
### next TODO
- play / pause button
- stop button
- volume buttons
- home button (placeholder until home screen implemented)
- 2D analog stick button wiring
### next TODO
- file system
- song data type
- select song
### next TODO
- screen wiring
- screen hello world
### next TODO
- screen pages (home, queue, songs, artists, playing, settings?)
### next TODO
- safe power off
### next TODO
- battery wiring
- power saving stuff
### next TODO
- settings file
> - screen brightness
> - init volume
> - init screen (default to home)



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


