#/bin/sh
set -x

TEST_FILE=mpeg

./cp2xpn     "$TEST_FILE" "/PNFS/file-1"
./cpfromxpn               "/PNFS/file-1" "$TEST_FILE"-1
diff         "$TEST_FILE"                "$TEST_FILE"-1

