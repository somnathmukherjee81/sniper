all:
    gcc sniper.c init.c haskkeys.c display.c bitboards.c board.c test.c data.c attack.c io.c mobegen.c validate.c makemove.c perft.c search.c misc.c pvtable.c evaluate.c uci.c xboard.c -o sniper -static-libgcc -static-libstdc++
