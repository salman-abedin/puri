#!/bin/sh
#
# Launches a tui of urls from a given file
# Usage: puri [FILE...]
# Dependencies: grep, sed, uniq, tr, wc, cut, stty, head, read

if [ -p /dev/stdin ]; then
    FILES=/dev/stdin
else
    FILES=$*
fi
URLS=/tmp/urls

grep -Pzo '(http|https)://[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*\n*[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*' "$FILES" | tr -d '\n' | sed -e 's/http/\nhttp/g' -e 's/$/\n/' | sed '1d' | uniq > "$URLS"

SHOWCURSOR="\033[?25h"
HIDECURSOR="\033[?25l"
CLEAR="\033[2J\033[H"

ITEMS=$(wc -l "$URLS" | cut -d' ' -f1)
cursor=1

quit() {
    printf "%b" "$SHOWCURSOR$CLEAR"
    rm -f "$URLS"
    exit
}

getkey() {
    CURRENT_TTY_SETTINGS=$(stty -g)
    stty -icanon -echo
    head -c1
    stty "$CURRENT_TTY_SETTINGS"
}

handleinput() {
    case "$(getkey)" in
        ';')
            setsid "$BROWSER" "$(sed "${cursor}q;d" "$URLS")"
            quit
            ;;
        l)
            [ "$cursor" -lt "$ITEMS" ] && cursor=$((cursor + 1))
            ;;
        k) [ "$cursor" -gt 1 ] && cursor=$((cursor - 1)) ;;
        j) quit ;;
    esac
}

drawitems() {
    goto 5 0
    i=
    while read -r url; do
        i=$((i + 1))
        if [ "$i" = "$cursor" ]; then
            mark "$url"
        else
            echo "$url"
        fi
    done < "$URLS"
}

mark() {
    printf "\033[7m"
    echo "$@"
    printf "\033[27m"
}

goto() { printf "%b" "\033[${1};${2}H"; }

footer() {
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 10))"
    mark "$@"
}

header() {
    goto 3 "$((COLUMNS / 2 - 10))"
    mark "$@"
    printf "\n\n\n"
}

setscreen() {
    printf "%b" "$HIDECURSOR$CLEAR"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
}

main() {
    setscreen
    header TUP: tmux url picker
    footer j:Down k:Up l:launch h:Quit
    while :; do
        drawitems
        handleinput
    done
}
main
