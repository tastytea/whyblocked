**Whyblocked** reminds you why you blocked someone. It is developed with
Mastodon in mind, but can be used for other contexts, of course.

It has a text- and a Qt-interface and uses a SQLite-database.

![Screenshot Qt interface](https://doc.schlomp.space/whyblocked/whyblocked_screenshot.png)

## Usage

For the text interface start `whyblocked`, for the Qt interface
`whyblocked-gui`. You can also start it from the menu of your desktop
environment.

## Install

### Packages

Every [release](https://schlomp.space/tastytea/whyblocked/releases) includes
a .deb-package, an .rpm-package and a .tar.gz-package with precompiled binaries
for x86_64(amd64). These are automatically built and not tested. You can install
them with `dpkg -i` or `rpm -i`, respectively.

Gentoo ebuilds are available via my
[repository](https://schlomp.space/tastytea/overlay).

### From source

#### Dependencies

* C++ compiler (tested: [gcc](https://gcc.gnu.org/) 6/7/8,
  [clang](https://llvm.org/) 3/5/6)
* [cmake](https://cmake.org/) (at least 3.6)
* [vsqlite++](http://vsqlite.virtuosic-bytes.com/) (tested: 0.3.13)
* [libxdg-basedir](http://repo.or.cz/w/libxdg-basedir.git) (tested: 1.2)
* [qtwidgets](https://www.qt.io/) (tested: 5.11/5.7)

#### Get sourcecode

Download the current
[release](https://schlomp.space/tastytea/whyblocked/releases).

#### Compile

```SH
mkdir build
cd build
cmake ..
make
make install
```

## Contributing

Contributions are always welcome. You can submit them as pull requests or via
email to `tastytea`@`tastytea.de`.

### Translations

To start a new translation, go to `translations/` and copy `whyblocked_en.ts` to
a file [appropriate](https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2#Officially_assigned_code_elements)
for the new language. If you want to translate to french for example, that file
name would be `whyblocked_fr.ts`.

Use [Linguist](http://doc.qt.io/qt-5/qtlinguist-index.html) to edit the
`.ts`-files.

## Bugs

Please report them on the
[issue tracker](https://schlomp.space/tastytea/whyblocked/issues) or to
`tastytea`@`tastytea.de`.

## Copyright

```PLAIN
Copyright © 2018 tastytea <tastytea@tastytea.de>.
License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl-3.0.html>.
This program comes with ABSOLUTELY NO WARRANTY. This is free software,
and you are welcome to redistribute it under certain conditions.
```
