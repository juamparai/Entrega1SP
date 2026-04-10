#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o /dev/null
#SBATCH -e salidas/bloques_errors.txt
#SBATCH --time=00:15:00

./bloques.o $1 > salidas/bloques_$1.txt