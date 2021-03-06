#!/bin/bash
#
# This is an example how to split up a directory of compressed  opterm files
# into histograms by Settings.
#
# -b is the starttime, use first numbers of the first run of your setting
# -e endtime
#

export -n DISPLAY

zcat *03.96-* | Printlog++ -f - -b96032207 -e96032218 -o opterm_2-1.his
zcat *03.96-* | Printlog++ -f - -b96032120 -e96032207 -o opterm_2-2.his
zcat *11.95-* | Printlog++ -f - -b95111813 -e95111822 -o opterm_2-3.his
zcat *11.95-* | Printlog++ -f - -b95111901 -e95111910 -o opterm_2-4.his

zcat *03.96-* | Printlog++ -f - -b96032222 -e96032412 -o opterm_3-1.his
zcat *03.96-* | Printlog++ -f - -b96032415 -e96032507 -o opterm_3-2.his
zcat *11.95-* | Printlog++ -f - -b95111703 -e95111807 -o opterm_3-3.his
zcat *11.95-* | Printlog++ -f - -b95111520 -e95111620 -o opterm_3-5.his


zcat *02.96-* | Printlog++ -f - -b96021001 -e96021023 -o opterm_0-3.his
zcat *05.96-* | Printlog++ -f - -b96050119 -e96050303 -o opterm_0.3-3.his
zcat *02.96-* | Printlog++ -f - -b96021700 -e96021804 -o opterm_1-3.his






