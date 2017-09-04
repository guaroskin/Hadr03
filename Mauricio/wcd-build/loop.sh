#!/bin/bash

rm e_sp.dat

for Ek in $(seq 0.01 0.01 0.1);do
  
#  cat input.in | sed -e "s/momentum [0-9].[0-9]/momentum $Ek./" > tmp.in

  cat input.in | sed -e "s/energy [0-9].[0-9]./energy $Ek/" > tmp.in

  ./wcd -m tmp.in > tmp0.dat

  cat tmp0.dat | awk -v ener=$Ek '{if($2=="stepping:")sum+=(100.-$3);}END{print ener,sum/1000}' >> e_sp.dat

#  cat tmp0.dat | awk '{if($1=="Number" && $3=="Cerenkov")print $10}' > e_${Ek}MeV-nphotons.dat
  
done

rm tmp.in tmp0.dat
