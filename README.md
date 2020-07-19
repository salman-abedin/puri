# puri: A minimal URL launcher for CLI

puri is an extremely minimal url launcher written in pure POSIX shellscript.

## Installation

```sh
git clone https://github.com/salman-abedin/puri.git && cd puri && sudo make install
```

## Usage

### Shell

```sh
puri [FILE...]
```

### Tmux

```sh
bind u run 'tmux capture-pane -C -J -S -1024 -E 1024 -p > /tmp/tb' \; \
  new-window 'puri /tmp/tb && rm -f /tmp/tb'
```

## Uninstallation

```sh
sudo make uninstall
```

---

## Repos you might be interested in

[Bolt](https://github.com/salman-abedin/bolt)
: The lightning fast workflow creator

[Crystal](https://github.com/salman-abedin/crystal)
: The transparent setup

[Magpie](https://github.com/salman-abedin/magpie)
: The dotfiles

[Alfred](https://github.com/salman-abedin/alfred)
: The scripts

[Devour](https://github.com/salman-abedin/devour)
: Terminal swallowing

[Uniblocks](https://github.com/salman-abedin/uniblocks)
: The status bar wrapper

## Contact

SalmanAbedin@disroot.org
