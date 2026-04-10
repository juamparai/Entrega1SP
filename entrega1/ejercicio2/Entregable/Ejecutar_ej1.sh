#!/bin/bash

echo "Enviando q1"
sbatch script1.sh

for n in 10 100 500
do
  echo "Enviando quadratic2 con n=$n"
  sbatch quadratic2.sh $n
  echo "Enviando quadratic3 con n=$n"
  sbatch quadratic3.sh $n
done
