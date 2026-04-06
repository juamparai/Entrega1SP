#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o directorioSalida/output.txt
#SBATCH -e directorioSalida/errors.txt
#SBATCH --time=00:05:00

./q1.o

for n in 10 100 1000
do
  ./q2.o $n
  ./q3.o $n
done