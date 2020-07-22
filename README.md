![](preview.gif)

# puri: A minimal CLI URL launcher

puri is a minimal TUI for searching and launching URLs from a given file.

puri is ~100 lines of POSIX shellscript.

## Dependencies

-  grep, sed, sort, uniq, tr, wc, cut, stty, head, seq

## Installation

```sh
git clone https://github.com/salman-abedin/puri.git && cd puri && sudo make install
```

## Usage

#### Tmux

```tmux
bind u run 'tmux capture-pane -p > /tmp/tb' \; \
  new-window -n puri 'puri /tmp/tb && rm -f /tmp/tb'
```

#### Mutt/Neomutt

```muttrc
macro index,pager o "<pipe-entry>tee /tmp/muttb<enter><shell-escape>puri /tmp/muttb<enter>"
macro index,pager o "<pipe-message>tee /tmp/muttb<enter><shell-escape>puri /tmp/muttb<enter>"
```

#### Shell

```sh
puri [FILE...]
```

## Uninstallation

```sh
sudo make uninstall
```

## TODOs

-  Scrolling support

## Similar Projects

-  [urlscan](https://github.com/firecat53/urlscan)
-  [urlview](https://github.com/sigpipe/urlview)

---

## Repos you might be interested in

[bolt](https://github.com/salman-abedin/bolt)
: The lightning fast workflow creator

[uniblocks](https://github.com/salman-abedin/uniblocks)
: The status bar wrapper

[tide](https://github.com/salman-abedin/puri)
: Minimal Transmission CLI client

[crystal](https://github.com/salman-abedin/crystal)
: The transparent setup

[devour](https://github.com/salman-abedin/devour)
: Terminal swallowing

[Magpie](https://github.com/salman-abedin/magpie)
: The dotfiles

[Alfred](https://github.com/salman-abedin/alfred)
: The scripts

## Contact

SalmanAbedin@disroot.org
