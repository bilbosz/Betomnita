# Betomnita

## Description
**Betomnita** is real-time strategy game with programmable units. Name of the project come from Greek letters **Bet**a, **Omni**cron and **Ta**u which stand for bot(βοτ with Greek letters). Bot is synonym for [software agent](https://en.wikipedia.org/wiki/Software_agent), program that is defined as unit, executes tasks in environment, is able to communicate, monitors its local environment and makes authonomic decisions to achieve determined goal.

## Requirements
### Build
Primary, in order to build repository you need to have external libraries in version described in `conf/build.conf`. Secondary you need to establish `res` directory in project root folder for resources repository; path and revision in placed in `conf/build.conf`.
#### External dependencies
* [**Boost**](https://www.boost.org/) - free peer-reviewed portable C++ source libraries,
* [**Simple Fast Multimadia Library**](https://www.sfml-dev.org/) - interface for various components of PC, to ease development of games and multimedia applications.
#### Environment variables
* `BOOST_ROOT` - root directory for **Boost**
* `SFML_ROOT` - root directory for **Simple Fast Multimadia Library**