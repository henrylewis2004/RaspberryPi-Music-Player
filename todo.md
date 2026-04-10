# TODO

- ~~sd file reading~~
- ~~sd file writing~~
- ~~playing sounds~~
- ~~reading wav header~~ 
- ~~playing wav file~~
### next TODO
- ~~play / pause function~~ //NOTE: maybe change dac_mute to ramp vol instead
- ~~stop function~~
- reset song
> - just stop and start song ? - need to keep track of currently playing song however so maybe implement when creating the queue
- ~~dac register volume functions~~
- ~~higher system volume functions (audio_player.c)~~
### next TODO
- song queue
> - ~~add multiple songs to sd card (5-10)
> - ~~linked list with a pointer to next song~~
> - ~~contains wav file path~~
> - song info? link to song data type? (question for file system, now just get songs to play in order with skip)
- ~~skip song function~~
- ~~implement song ending~~ //NOTE: needs testing
### next TODO
- play / pause button
- stop button
- volume buttons
- home button (placeholder until home screen implemented)
- 2D analog stick button wiring
- skip song button
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


