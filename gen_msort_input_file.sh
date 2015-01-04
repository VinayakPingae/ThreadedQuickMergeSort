NUM=25
MIN=0
RANGE=200
OF="msort_input"

i=0
while [ $i -lt $NUM ]
do
    n=$(($MIN+$RANDOM%$RANGE))
    
    if [ $i -eq 0 ] 
    then
        echo $n > $OF
    else
        echo $n >> $OF
    fi
    
    i=$(($i+1)) 
done 
