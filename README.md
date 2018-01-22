# Snowflake-php

Snowflake is a network service for generating unique ID numbers at high scale with some simple guarantees.

This is a C version for php extension.

 
### Performance
 * minimum 10k ids per second per process
 * response rate 2ms (plus network latency)
 
 
##  Solution
* Thrift Server written in Scala
* id is composed of:
  * time - 41 bits (millisecond precision w/ a custom epoch gives us 69 years)
  * configured machine id - 10 bits - gives us up to 1024 machines
  * sequence number - 12 bits - rolls over every 4096 per machine (with protection to avoid rollover in the same ms)

### System Clock Dependency

You should use NTP to keep your system clock accurate.  Snowflake protects from non-monotonic clocks, i.e. clocks that run backwards.  If your clock is running fast and NTP tells it to repeat a few milliseconds, snowflake will refuse to generate ids until a time that is after the last time we generated an id. Even better, run in a mode where ntp won't move the clock backwards. See http://wiki.dovecot.org/TimeMovedBackwards#Time_synchronization for tips on how to do this.

 

## building

To build and test

```shell
cd snowflake
phpize
./configure
make && make install
```
To use unique in php:

```php

echo sid();

```
And U can add server id into PHP.ini（Optional）:
```php

[snowflake]
snowflake.server_id = 10

```
