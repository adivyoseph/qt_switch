# qt_switch
simple switch for testing mqtt implimented in QT

hardcode to use `localhost` and `port 1883` for now

tested with mosquetto broker

uses mosquetto library

# Features
* Supports multiple switches controlling the same light group
* Switch does not keep state, state is in the lights
* Switches publish switched pressed events `<room>\<light group>_switch` 1 always
* Lights respond publishing on/off state   `<room>\<light group>_light`  1 on, 0 off
* Switches publish level setting changes   `<room>\<light group>_level`  0-99 for level
* Switches subscribe to level changes, so they track each other
* 

# Steps to use
* open __config__ to set _publish topic_
  * __Room name:__ location of the switch (multiple switchs may share this)
  * __Switch name:__ must be unique (entrance, master, readingLight, ...)
* __connect__ to mosquetto broker
  * toggle __switch__ (ON.OFF)
  * move __slider__ to change light level ( 0-100)
* __disconnect__


qt_light will get messages and track actions
