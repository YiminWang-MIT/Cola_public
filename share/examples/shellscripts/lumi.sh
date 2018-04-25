#! /bin/bash

SETUP=$1
CDROM=/cdrom
RUNLIST=runlist.txt

mount /cdrom 2>/dev/null
mkdir -p $SETUP/luminosity
for f in `find $CDROM -name "run_*" | sort`
do
    name=run_`zcat $f | head -c24 | tail -c12` 
    export LUMI_OUT=$SETUP/luminosity/$name.lumi
    if grep $name $RUNLIST | grep $SETUP | grep -q data && [ ! -f $LUMI_OUT ]
    then
        echo "Calculating Luminosity for "$name
	nice Lumi++ -zf $f
        echo "done."
      else
        echo "Skipping "$name
      fi
done
umount $CDROM
eject `grep $CDROM /etc/fstab | awk '{print $1}'` 






