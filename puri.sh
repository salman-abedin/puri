#!/bin/sh
#
# Launches a TUI of URLs to pick searched from a given file
# Usage: puri [FILE...]
# Dependencies: grep, sed, uniq, tr, wc, cut, stty, head, read

URLS=/tmp/urls
cursor=1

quit() {
    printf "%b" "\033[?25h\033[2J\033[H"
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
        h) quit ;;
        j) [ "$cursor" -lt "$ITEMS" ] && cursor=$((cursor + 1)) ;;
        k) [ "$cursor" -gt 1 ] && cursor=$((cursor - 1)) ;;
        l) setsid "$BROWSER" "$(sed "${cursor}q;d" "$URLS")" > /dev/null 2>&1 ;;
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

setfooter() {
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 10))"
    mark "$@"
}

setborder() {
    goto 3 0
    for i in $(seq $COLUMNS); do printf "%s" "-"; done
    goto "$((LINES - 2))" 0
    for i in $(seq $COLUMNS); do printf "%s" "-"; done
}

setheader() {
    goto 2 "$((COLUMNS / 2 - 10))"
    mark "$@"
    printf "\n\n\n"
}

setscreen() {
    printf "%b" "\033[?25l\033[2J\033[H"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
}

init() {
    grep -Pzo \
        '(http|https)://[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*\n*[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*' "$@" |
        tr -d '\n' |
        sed -e 's/http/\nhttp/g' -e 's/$/\n/' |
        sed '1d' | sort | uniq > "$URLS"
    ITEMS=$(wc -l "$URLS" | cut -d' ' -f1)
}

main() {
    init "$@"
    setscreen
    setborder
    setheader PURI: POSIX URL Launcher
    setfooter j:Down k:Up l:launch h:Quit

    trap 'quit' INT

    while :; do
        drawitems
        handleinput
    done
}
main "$@"
