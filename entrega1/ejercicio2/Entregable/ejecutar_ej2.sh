#!/bin/bash

for n in 512 1024 2048 4096 
  for j in 16 32 64 128
    do
       echo "Enviando ejecucion con n=$n bs= $j"
       sbatch ejercicio2.sh $n $j
done
