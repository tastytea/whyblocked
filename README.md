**Whyblocked** allows you to store the reason why you blocked someone, along
with "receipts", URLs to the posts that led you to block them.

It has a Qt-interface and uses an SQLite-database.

![Screenshot Qt interface](https://doc.schlomp.space/whyblocked/whyblocked_screenshot.png)

## Usage

Run `whyblocked`. You can also start it from the menu of your desktop
environment.

## Install

### Packages

Every [release](https://schlomp.space/tastytea/whyblocked/releases) includes
a .deb-package, an .rpm-package and a .tar.gz-package with precompiled binaries
for x86_64(amd64) ([more info](https://schlomp.space/tastytea/whyblocked/wiki/Binary-Packages)). These are automatically built and not tested. You can install
them with `dpkg -i` or `rpm -i`, respectively. The packages are signed with the
PGP key [242E5AC4DA587BF9](https://tastytea.de/tastytea_autosign.asc)
(Fingerprint: `F730 1ADF C9ED 2624 48C4  2B64 242E 5AC4 DA58 7BF9`).

Gentoo ebuilds are available via my
[repository](https://schlomp.space/tastytea/overlay).

### From source

#### Dependencies

* C++ compiler (tested: [gcc](https://gcc.gnu.org/) 6/8/9,
  [clang](https://llvm.org/) 3/6)
* [cmake](https://cmake.org/) (at least 3.2)
* [vsqlite++](http://vsqlite.virtuosic-bytes.com/) (tested: 0.3)
* [libxdg-basedir](http://repo.or.cz/w/libxdg-basedir.git) (tested: 1.2)
* [qtcore](https://www.qt.io/) (tested: 5.11/5.5)
* [qtwidgets](https://www.qt.io/) (tested: 5.11/5.5)
* [libconfig++](https://github.com/hyperrealm/libconfig) (tested: 1.5)
* Optional
    * Translations: [lupdate & lrelease](http://doc.qt.io/qt-5/linguist-manager.html) (tested: 5.11/5.5)
    * Manpage: [asciidoc](http://asciidoc.org/) (tested: 8.6)

#### Get sourcecode

Download the current
[release](https://schlomp.space/tastytea/whyblocked/releases) and copy
[xdgcfg](https://schlomp.space/tastytea/xdgcfg) into `xdgcfg/`.

If you clone from git, be sure to `git submodule init` and
`git submodule update` afterwards. See the [submodules article in the git book]
(https://git-scm.com/book/en/v2/Git-Tools-Submodules#_cloning_submodules) for
further info.

#### Compile

```SH
mkdir build
cd build
cmake ..
make
make install
```

cmake options:
* `-DCMAKE_BUILD_TYPE=Debug` for a debug build
* `-DWITH_TRANSLATIONS=NO` to not compile translations
* `-DWITH_MAN=NO` to not compile the manpage
* One of:
    * `-DWITH_DEB=YES` if you want to be able to generate a deb-package
    * `-DWITH_RPM=YES` if you want to be able to generate an rpm-package

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

## Licence & Copyright

```PLAIN
Copyright © 2018 tastytea <tastytea@tastytea.de>.
License GPLv3: GNU GPL version 3 <https://www.gnu.org/licenses/gpl-3.0.html>.
This program comes with ABSOLUTELY NO WARRANTY. This is free software,
and you are welcome to redistribute it under certain conditions.
```

The icon is derived from the icons `messagebox critical` and `help` of the
[Gartoon icon theme](https://commons.wikimedia.org/wiki/Gartoon_icons). The
original authors were Zeus, Patrick Yavitz and La Mula Francis, who released
them under the terms of the
[GPL2+](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) and
[DSL](http://www.fsf.org/licensing/licenses/dsl.html). The new icon is released
under the same terms.
