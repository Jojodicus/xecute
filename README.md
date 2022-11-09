# XECUTE

Inspired by [RootDO](https://codeberg.org/sw1tchbl4d3/rdo), this is an even more stripped-down version

I do not guarantee the safety and/or security of this program, everything you do is at your own risk.

## Installation

```
git clone https://github.com/Jojodicus/xecute
cd xecute
make
su -c "make install"
```

to uninstall:

```
su -c "make uninstall"
```

## Usage

```
xte <command>
```

There is no need to configure any files, as everything is done at compile-time.

By default, there are only two users allowed to use `xte`: root itself and the user who compiled the program
