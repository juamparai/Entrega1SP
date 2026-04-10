#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o /dev/null
#SBATCH -e salidas/ejercicio2_errors.txt
#SBATCH --time=00:15:00

./ejercicio2.o $1 > salidas/ejercicio2_$1.txt