#/bin/sh
set -x

./cp2xpn M.mpeg "/PNFS/pp$1"
./cpfromxpn     "/PNFS/pp$1" "M.mpeg.$1"
diff M.mpeg "M.mpeg.$1"

echo "The end"
