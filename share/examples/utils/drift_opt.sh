run_name=`basename $1 | cut -f1 -d.`

for drifttime in 5000 5025 5050 5075 5100 5125 5150 5175 5200 5225 5250 5275 5300 5325 5350 5375 5400
do
 cd rundb
 echo "A.VDC.Driftvelocity = 0.0"$drifttime"0 * 0.50" >  this_DV.db
 echo "B.VDC.Driftvelocity = 0.0"$drifttime"0 * 0.50" >> this_DV.db
 echo "C.VDC.Driftvelocity = 0.0"$drifttime"0 * 0.25" >> this_DV.db
 ln -sf this_DV.db test_DV.db
 date
 cat test_DV.db
 cd ..
 ONLINE_COL=col/VCSraw.col Cola++ -dzf $1 -n $2
 mv Online.his his/testVDC/$run_name.$drifttime.his
 echo
 date
done

cd rundb
ln -sf empty.db test_DV.db
cd ..

