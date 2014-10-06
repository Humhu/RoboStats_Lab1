#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Please specify number of trials to run."
    exit 1
fi

TRIALS=$1

echo "Running $1 trial(s)."

for i in `seq 1 $1`;
do

	echo "Running trial $i for Map1_Standard"
	./bin/RunFilter config/Map1_Standard.xml &> Map1_Standard_$i.log
	mv Map1_Standard.avi /host/Users/humphreh/Videos/Map1_Standard_$i.avi

	echo "Running trial $i for Map1_HighConfidence"
	./bin/RunFilter config/Map1_HighConfidence.xml &> Map1_HighConfidence_$i.log
	mv Map1_HighConfidence.avi /host/Users/humphreh/Videos/Map1_HighConfidence_$i.avi

	echo "Running trial $i for Map1_DenseHighConfidence"
	./bin/RunFilter config/Map1_DenseHighConfidence.xml &> Map1_DenseHighConfidence_$i.log
	mv Map1_DenseHighConfidence.avi /host/Users/humphreh/Videos/Map1_DenseHighConfidence_$i.avi

	echo "Running trial $i for Map1_DenseHighConfidenceVariance"
	./bin/RunFilter config/Map1_DenseHighConfidenceVariance.xml &> Map1_DenseHighConfidenceVariance_$i.log
	mv Map1_DenseHighConfidenceVariance.avi /host/Users/humphreh/Videos/Map1_DenseHighConfidenceVariance_$i.avi

	echo "Running trial $i for Map4_Standard"
	./bin/RunFilter config/Map4_Standard.xml &> Map4_Standard_$i.log
	mv Map4_Standard.avi /host/Users/humphreh/Videos/Map4_Standard_$i.avi

	echo "Running trial $i for Map4_HighConfidence"
	./bin/RunFilter config/Map4_HighConfidence.xml &> Map4_HighConfidence_$i.log
	mv Map4_HighConfidence.avi /host/Users/humphreh/Videos/Map4_HighConfidence_$i.avi

	echo "Running trial $i for Map4_DenseHighConfidence"
	./bin/RunFilter config/Map4_DenseHighConfidence.xml &> Map4_DenseHighConfidence_$i.log
	mv Map4_DenseHighConfidence.avi /host/Users/humphreh/Videos/Map4_DenseHighConfidence_$i.avi

	echo "Running trial $i for Map4_DenseHighConfidenceVariance"
	./bin/RunFilter config/Map4_DenseHighConfidenceVariance.xml &> Map4_DenseHighConfidenceVariance_$i.log
	mv Map4_DenseHighConfidenceVariance.avi /host/Users/humphreh/Videos/Map4_DenseHighConfidenceVariance_$i.avi
	
done

echo "All trials finished!"
date