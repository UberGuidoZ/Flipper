![Banner Image](docs/generic-screenshot.png)
# Flipp Pomodoro

Your productivity best friend. Don't let your flipper get borred, let him help you instead.

## Development

### Current state and plans

At the moment following functionality implemented:
![Working Screen](docs/working.png)
![Resting Screen](docs/resting.png)
* Generic pomodoro cycle with two stages (Work: 25 min and rest 5 minutes)
* Automatic and manual phases switch
* Notification(sound, vibration, backlight, led) on stage change.
* Energy Saving workflow

Will do(if I've got time):
* ~~Publish a .fap package to let anyone download and install the app.~~
* * Done, now you can [DOWNLOAD application from releases](https://github.com/Th3Un1q3/flipp_pomodoro/releases)
* Configure CI pipeline for automatic releases
* Stats on exit(how many pomodoros complete)
* Background work or restore from last state
* Integration with passport to develop your flipper profile by completing pomodoros
* Configuration of notifications
* Blind mode(no timer updates, just background and notification) for more energy saving

###  Build and Package
Build application
```shell
# For standard(official) firmware
bash tools/build.sh

# For unleashed firmware
bash tools/build.sh -f unleashed 
```
