![](preview.gif)

# puri: Minimal CLI URL launcher

puri is a minimal TUI for searching and launching URLs from a given file.

puri is a ~100 line POSIX shellscript.

## Features

-  Vim bindings
-  Scrollable list

## Dependencies

-  grep, sed, sort, cut, head, seq, cat

## Installation

```sh
git clone https://github.com/salman-abedin/puri.git && cd puri && sudo make install
```

## Usage

#### Tmux ( Press prefix, 'u' )

```tmux
bind u run 'tmux capture-pane -p > /tmp/tb' \; \
  split-window 'puri /tmp/tb && rm -f /tmp/tb'
```

#### Mutt/Neomutt ( Press 'o' )

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

## Patches

-  **27/07/20**:- Added scrolling support

## Similar Projects

-  [urlscan](https://github.com/firecat53/urlscan)
-  [urlview](https://github.com/sigpipe/urlview)

---

## Repos you might be interested in

| Name                                                    | Description             |
| ------------------------------------------------------- | ----------------------- |
| [bolt](https://github.com/salman-abedin/bolt)           | The launcher wrapper    |
| [uniblocks](https://github.com/salman-abedin/uniblocks) | The status bar wrapper  |
| [tide](https://github.com/salman-abedin/tide)           | Transmission CLI client |
| [devour](https://github.com/salman-abedin/devour)       | Terminal swallowing     |
| [crystal](https://github.com/salman-abedin/crystal)     | The transparent setup   |
| [Magpie](https://github.com/salman-abedin/magpie)       | The dotfiles            |
| [Alfred](https://github.com/salman-abedin/alfred)       | The scripts             |

## Contact

SalmanAbedin@disroot.org
