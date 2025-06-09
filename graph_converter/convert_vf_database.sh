#!/bin/bash -e

dir="${1}"
fname="${dir}/snap.txt"

# make clean
make

echo
date
printf "\nSNAP TO BIN\n"
# gdbserver localhost:1236
bin/snapToBin "${fname}" 1
# rm "${fname}"

# NOTE fname.bin and fname.rev.bin are BIG-ENDIAN

echo
date
printf "\nSORT REVERSE\n"
bin/bsort -k 4 -r 8 -v "${fname}.rev.bin"

echo
date
printf "\nMAKE LISTS\n"
# gdbserver localhost:1236
bin/makeLists "${fname}"
rm "${fname}.bin" "${fname}.rev.bin" "${fname}.raw.degree.bin"

echo
date
printf "\nCOMPACTIFY\n"
bin/compactify "${fname}"
rm "${fname}.raw.edge.bin" "${fname}.raw.vertex.bin"

echo
date
printf "\nHALVE\n"
bin/halve "${fname}"

echo
date
printf "\nLABELIZE\n"
# gdbserver localhost:1236
bin/labelize "${fname}" 1

#echo
#date
#printf "\nRELABEL\n"
#bin/relabel "${fname}"
