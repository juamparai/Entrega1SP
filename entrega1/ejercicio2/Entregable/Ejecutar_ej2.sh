#!/bin/bash

for n in 512 1024 2048 4096 
do
   echo "Enviando ejecucion con n=$n"
   sbatch ejercicio2.sh $n
done
