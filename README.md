**whyblocked** reminds you why you blocked someone. It is developed with
Mastodon in mind, but can be used for other contexts, of course.

It has a text interface and uses a SQLite-database.

## Usage

### Example

```PLAIN
$ whyblocked
This is whyblocked 0.4.1.
Type add, remove, view or details. Or just the first letter.
Type quit or q to quit the program.
: add
User or instance: @tastytea@soc.ialis.me
Blocked(b) or silenced(s): b
Reason: Too nice
@tastytea@soc.ialis.me added.
Add receipt? [y/n] y
URL: https://tastytea.de/
Receipt added.
Add receipt? [y/n] n
: view
 Blocked: @tastytea@soc.ialis.me because: Too nice
: details
User or instance: @tastytea@soc.ialis.me
@tastytea@soc.ialis.me is blocked, because: Too nice
Receipts:
  https://tastytea.de/
: remove
User or instance: @tastytea@soc.ialis.me
@tastytea@soc.ialis.me removed.
: view
: details
User or instance: @tastytea@soc.ialis.me
@tastytea@soc.ialis.me is not in the database.
: quit
```

## Install

### Dependencies

* C++ compiler (tested: gcc 6/7/8)
* [cmake](https://cmake.org/) (at least 3.7)
* [vsqlite++](http://vsqlite.virtuosic-bytes.com/) (tested: 0.3.13)
* [libxdg-basedir](http://repo.or.cz/w/libxdg-basedir.git) (tested: 1.2)

### Compile

```SH
mkdir build
cd build
cmake ..
make
make install
```

# Copyright

```PLAIN
Copyright © 2018 tastytea <tastytea@tastytea.de>.
License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl-3.0.html>.
This program comes with ABSOLUTELY NO WARRANTY. This is free software,
and you are welcome to redistribute it under certain conditions.
```
