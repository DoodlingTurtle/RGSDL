
#!/bin/bash

echo ./lib/libRGSDL.a | entr notify-send "Lib Compiler finished" &

while sleep 0.1 ; do
    ls src/* lib/* | entr -d make
done
