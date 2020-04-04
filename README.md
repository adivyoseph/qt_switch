# qt_switch
simple switch for testing mqtt implimented in QT

hardcode to use localhost and port 1883 for now

tested with mosquetto broker

uses mosquetto library

Steps to use
* open config to set _publish topic_
 * Room name: location of the switch (multiple switchs may share this)
 * Switch name: must be unique (entrance, master, readingLight, ...)
* connect to mosquetto broker
 * toggle switch (ON OFF)
 * move slider to change light level ( 0-100)
* disconnect


qt_light will get messages and track actions
