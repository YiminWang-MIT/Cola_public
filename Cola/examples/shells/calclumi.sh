for file in /data/kpha1/online0/960501/run*
do
	nice Luminosity -f $file >/dev/null
	mv Luminosity.out ~/scr/data/Setting05-3/`basename $file`.luminosity
done
