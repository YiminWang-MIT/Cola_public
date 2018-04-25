SETUP=$1
CDROM=/cdrom/
RUNLIST=~/cdrom/mitruns3.doc

export -n DISPLAY
export -n VERBOSE
export CINDY_FAST_CLEAR=true
export ONLINE_COL=filter.col
export ONLINE_HIS=$SETUP/$name.his
export LUMINOSITY_OUT=$SETUP/$name.lumi

DIR=$SETUP/filtered

while [ 1 = 1 ]
do
  mount $CDROM
  for f in `find $CDROM -name "run_*" | sort`
  do
    name=run_`zcat $f | head -c24 | tail -c12` 
    if grep $name $RUNLIST | grep $SETUP >/dev/null
    then
        echo "Analysing "$name
	nice Cola++ -zf $f -o $DIR/$name.filtered 
        echo " ... done"
      else
        echo "Skipping "$name
      fi

  done
  umount $CDROM
  if [ `hostname` = a1i15 ] 
  then
    eject /dev/sr0
  else
    eject /dev/hdc
  fi
  echo "Hit [enter] to continue ...."
  read muell
done



