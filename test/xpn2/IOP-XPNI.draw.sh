#!/bin/sh
#set -x


draw() 
{
#
DAT_NAME=$1
EPS_NAME=$2
COL1=$3
COL2=$4
ROW=$5

#
cat > 2_avg.awk << EOF2

BEGIN { 
        i=0;
        n=$ROW;
        nc=3;
      }

      { 
        v[0,i]=\$4; 
        v[1,i]=\$5; 
        v[2,i]=\$6; 
        i++; 

        if (i==n) 
        { 
           i=0; 

           printf("%s",\$3);
           for(k=0;k<nc;k++)
           {
		   s=0; 
		   for(j=0;j<n;j++) 
		   {
		      s=s+v[k,j];
		   } 
                   printf(" %f",(s/n));
           }
           printf("\n");
        }
      }

EOF2

#
FIRST_LINE=$(cat $DAT_NAME | head -1)
cat $DAT_NAME | grep -v "$FIRST_LINE" | awk -f 2_avg.awk > 0.data
rm -fr ./*.awk

#
grep raid5outer 0.data | awk '{ print $'$COL1'" "$'$COL2' }' > 1.data
grep raid5inner 0.data | awk '{ print $'$COL1'" "$'$COL2' }' > 2.data
grep raid5inner 0.data | awk '{ print $'$COL1' }' > i.data

#
dos2unix 1.data
dos2unix 1.data
join 1.data 2.data | awk '{ print $2" "$3 }' > 3.data
dos2unix 3.data

#
sed 's/\.0\{6,25\}//g' i.data > 4.data
mv 4.data i.data
cat > 2_draw.sed << EOF5
s/32768/32KB/g
s/524288/512KB/g
s/2097152/2MB/g
s/16777216/16MB/g
s/67108864/64MB/g
s/134217728/128MB/g
s/268435456/256MB/g
EOF5

sed -f 2_draw.sed i.data > 4.data
mv 4.data i.data
rm -fr 2_draw.sed

N=1
L=0
NL=$(wc -l i.data | awk '{print $1}')
XTICS="set xtics ( "
while [ "$N" -le "$NL" ]; do

       IX=$(head -$N i.data | tail -1)
       XTICS=$XTICS"\"$IX\" $L"

       N=$(expr $N + 1)
       if [ "$N" -le "$NL" ]; then
           XTICS=$XTICS", "
       fi

       L=$(expr $L + 1)

done
XTICS=$XTICS" )"

#
cat > 2_draw.gp << EOF1

set label 1 "" at 10,0.50 right
set label 2 "" at 10,0.25 right

set key left top box 2 spacing 2.0 

${XTICS}

set noxzeroaxis
set xlabel "File Size (bytes)"

set noyzeroaxis
set ylabel "Time (seconds)"
set ytics

set grid

#set term postscript eps monochrome "Times-Roman" 22
set term postscript eps color solid "Times-Roman" 22
set output "${EPS_NAME}"

plot '< cat 3.data | cut -f1 -d" "' title "R5-outer" with linespoints 1 1, \
     '< cat 3.data | cut -f2 -d" "' title "R5-inner" with linespoints 3 3

EOF1

#
gnuplot < 2_draw.gp 
rm -fr ./*.data
rm -fr ./*.gp

#
##kghostview $EPS_NAME &
}




#
# Main
#

# Usage
if [ $# -lt 2 ]; then
        echo ""
	echo "Usage: $0 <results file> <number of clients> <number of servers>"
        echo ""
        echo "Example: $0 iop-xpni.8.1.txt 1 8"
        echo ""
	exit 1;
fi

# T_XCH_SEC
draw $1 "file-xch-$3.eps" 2 4 $2

# T_WRT_SEC
draw $1 "file-wrt-$3.eps" 2 3 $2


