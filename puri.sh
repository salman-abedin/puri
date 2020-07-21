#!/bin/sh
#
# Launches a TUI of URLs to pick searched from a given file
# Dependencies: grep, sed, sort, uniq, tr, wc, cut, stty, head, read, printf, echo, seq
# Usage: puri [FILE...]

URLS=/tmp/puri_urls
cursor=1
mark=/tmp/tide_mark

quit() {
    printf "\033[?7h\033[?25h\033[2J\033[H"
    rm -f $URLS $mark
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
        j)
            ITEMS=$(wc -l "$URLS" | cut -d' ' -f1)
            if [ "$cursor" -lt "$ITEMS" ]; then
                cursor=$((cursor + 1))
            else
                cursor=1
            fi
            ;;
        k)
            if [ "$cursor" -gt 1 ]; then
                cursor=$((cursor - 1))
            else
                cursor=$ITEMS
            fi
            ;;

        l) setsid "$BROWSER" "$(cat $mark)" > /dev/null 2>&1 ;;
    esac
}

drawitems() {
    goto 5 0
    i=0
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
    printf "\033[7m%s\033[27m\n" "$1"
    echo "${1%% *}" > "$mark"
}

goto() { printf "\033[%s;%sH" "$1" "$2"; }

setscreen() {
    printf "\033[?7l\033[?25l\033[2J\033[H"
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)

    printf "\033[2m"

    goto 2 "$((COLUMNS / 2 - 10))"
    echo "puri: Puny URL Launcher"
    goto 3 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 2))" 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 15))"
    echo "h:Quit   j:Down   k:Up   l:launch"

    printf "\033[m"

}

init() {
    grep -Pzo \
        '(http|https)://[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*\n*[a-zA-Z0-9+&@#/%?=~_|!:,.;-]*' "$@" |
        tr -d '\n' |
        sed -e 's/http/\nhttp/g' -e 's/$/\n/' |
        sed '1d' | sort | uniq > "$URLS"
}

main() {
    init "$@"
    setscreen
    trap 'quit' INT EXIT
    while :; do
        drawitems
        handleinput
    done
}
main "$@"
