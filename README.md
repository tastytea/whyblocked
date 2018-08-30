**whyblocked** reminds you why you blocked someone. It is developed with
Mastodon in mind, but can be used for other contexts, of course.

It has a text interface and uses a SQLite-database.

## Usage

```PLAIN
$ whyblocked
This is whyblock 0.1.0.
Type add, remove, view or details. Or just the first letter.
: v
 Blocked: @baduser@social.example.com because: Is very mean
Silenced: @truebelievers.example.com because: Annoying instance
: d
Which user?
@baduser@social.example.com
@baduser@social.example.com is blocked, because: Is very mean
Receipts:
  https:///social.example.com/@baduser/123456789012345678
: q
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
