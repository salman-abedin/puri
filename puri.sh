#!/bin/sh
#
# Launches a TUI of URLs to pick searched from a given file
# Dependencies: grep, sed, sort, cut, stty, head, seq, cat
# Usage: puri [FILE...]

URLS=/tmp/puri_urls
view=/tmp/puri_view
marks=/tmp/tide_marks

# Minimal 'wc'
# Stolen from https://github.com/dylanaraps/pure-sh-bible#get-the-number-of-lines-in-a-file
mwc() {
    while IFS= read -r line || [ -n "$line" ]; do
        lines=$((lines + 1))
    done < "$1"
    echo "$lines"
}

quit() {
    printf "\033[?7h\033[?25h\033[2J\033[H"
    rm -f $URLS $marks $view
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
            if [ "$cursor" = "$LIMIT" ] && [ "$end" -lt "$ITEMS" ]; then
                end=$((end + 1))
                start=$((start + 1))
                drawui
            else
                cursor=$((cursor < end ? cursor + 1 : cursor))
            fi
            ;;
        k)
            if [ "$cursor" = 1 ] && [ "$start" -gt 1 ]; then
                start=$((start - 1))
                end=$((end - 1))
                drawui
            else
                cursor=$((cursor > 1 ? cursor - 1 : cursor))
            fi
            ;;
        l) setsid "$BROWSER" "$(cat $marks)" > /dev/null 2>&1 ;;
    esac
}

drawitems() {
    goto 4 0
    i=0
    while read -r url; do
        i=$((i + 1))
        if [ "$i" = "$cursor" ]; then
            mark "$url"
        else
            echo "$url"
        fi
    done < $view
}

mark() {
    printf "\033[7m%s\033[27m\n" "$1"
    echo "${1%% *}" > "$marks"
}

goto() { printf "\033[%s;%sH" "$1" "$2"; }

drawui() {
    printf "\033[?7l\033[?25l\033[2J\033[H"

    printf "\033[2m"

    goto 2 "$((COLUMNS / 2 - 10))"
    echo "puri: Puny URL Launcher"
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 2))" 0
    for i in $(seq "$COLUMNS"); do printf "%s" "-"; done
    goto "$((LINES - 1))" "$((COLUMNS / 2 - 15))"
    echo "h:Quit   j:Down   k:Up   l:launch"

    printf "\033[m"

    sed -n "$start,$end p" "$URLS" > $view
}

init() {
    URLSTRING=/tmp/puri_urlstring
    while read -r line; do
        printf "%s" "$line"
    done < "$1" >> $URLSTRING
    grep -Eo 'http[s]?://[-#a-Z0-9_./?=_%:+&]*' $URLSTRING | sort -u > "$URLS"
    rm -f $URLSTRING

    ITEMS=$(mwc "$URLS")
    end=$((ITEMS > LIMIT ? LIMIT : ITEMS))
    cursor=1
    start=1
}

setscreen() {
    LINES=$(stty size | cut -d' ' -f1)
    COLUMNS=$(stty size | cut -d' ' -f2)
    LIMIT=$((LINES - 6))
}

main() {
    setscreen
    init "$@"
    drawui
    trap 'quit' INT TERM QUIT EXIT
    trap 'setscreen; drawui; drawitems' WINCH
    while :; do
        drawitems
        handleinput
    done
}
main "$@"
