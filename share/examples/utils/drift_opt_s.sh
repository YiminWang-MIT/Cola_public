run_name=`basename $1 | cut -f1 -d.`

#for drifttime in 0191 0201 0211 0221 0231 0241 0251 0261 0271 0281 0291 0301
for drifttime in 0311 0321 0331 0341 0351 0361 0371 0381 0391 0401
do
 cd rundb
 echo "A.VDC.S_corrDriftvelocity = 1."$drifttime >  this_DV.db
 echo "B.VDC.S_corrDriftvelocity = 1."$drifttime >> this_DV.db
 echo "C.VDC.S_corrDriftvelocity = 1."$drifttime >> this_DV.db
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

