#!/bin/bash

# Проверка на количество аргументов
if [ "$#" -ne 3 ]; then
    echo "Использование: $0 <файл_источник> <префикс_файла-результата> <количество_потоков>"
    exit 1
fi

# Присваиваем аргументы переменным
inp=$1
out_prefix=$2
num_threads=$3

# Цикл для запуска исполняемого файла заданное количество раз
for ((i = 1; i <= num_threads; i++)); do
    out="${out_prefix}${i}.qoi"
    echo "running\t./qoiconv ${inp} ${out} ${i} ..."
    ./qoiconv $inp $out $i
done