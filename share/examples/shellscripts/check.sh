#! /bin/bash
for f in `grep run_ runlist.txt | awk '{print $1}'`
do  
    echo -n "Checking "$f
    if  ls */filtered/$f.filtered &> /dev/null 
    then 
      if grep  $f runlist.txt | grep -q data
      then 
        echo " data"
      else
        echo " removing "$f.filtered
        rm  */filtered/$f.filtered
      fi
    else
      if grep $f runlist.txt | grep -q data
      then 
        echo -n " missing data "
        grep  $f runlist.txt
      else
      echo
      fi
    fi
done
