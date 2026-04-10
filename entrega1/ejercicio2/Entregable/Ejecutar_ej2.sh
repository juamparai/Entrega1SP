#!/bin/bash

for n in 512 1024 2048 4096 
do
   echo "Enviando ejecucion de un barrido con n=$n"
   sbatch unbarrido.sh $n
   echo "Enviando ejecucion varios con n=$n"
   sbatch bloques.sh $n
done
