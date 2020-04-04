# qt_switch
simple switch for testing mqtt implimented in QT

hardcode to use `localhost` and `port 1883` for now

tested with mosquetto broker

uses mosquetto library

#Steps to use
* open __config__ to set _publish topic_
  * __Room name:__ location of the switch (multiple switchs may share this)
  * __Switch name:__ must be unique (entrance, master, readingLight, ...)
* __connect__ to mosquetto broker
  * toggle __switch__ (ON.OFF)
  * move __slider__ to change light level ( 0-100)
* __disconnect__


qt_light will get messages and track actions
