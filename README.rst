Wordclock
=========

This is the sourcecode to an Arduino based wordclock. There exist many of these
wordclocks on the internet, or even on github. But this one is somewhat special
as it also encodes names of people's birthdays. It uses a genetic algorithm to
encode all the words in a sane way. See the ``scripts/`` directory for those
source files.

Hardware
--------

- Arduino Micro
- Chronodot
- Neopixel ledstrip (60 leds/meter)

Statistics
----------

The wordclock is a square of 10 by 10 characters encoding 23 words using up a
total space of 103 squares. As such, there is a need for reuse of characters.
Using the genetic algorithm it's able to encode all words while leaving 14
empty squares.

::

  K W A R T I E N F R 
  W V I J F A O V E R 
  Z V O O R J H A L F 
  A Y Z E S A Z N I T 
  N C Q E E N T E C W 
  M V H N V J I G I A 
  P I I T W E E E A A 
  U J N E T L N N L L 
  C F M A R L O E S F 
  K Q D R I E T U U R 


AVR Memory Usage
----------------

:Device: atmega32u4
:Program: 15888 bytes (48.5% Full)
:Data: 1123 bytes (43.9% Full)

