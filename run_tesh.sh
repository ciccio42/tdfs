#!/bin/bash
export CUDA_VISIBLE_DEVICES=0
DATA_GRAPH=${1:-"./data/bin_graph/com-youtube.ungraph/snap.txt"} #data_32.grf/snap.txt
QUERY_GRAPH=${2:-"./data/pattern/1.g"}                           #64/8/node_induced/label_32/connected_query_0_32.sub.grf
#
./bin/lb.out ${DATA_GRAPH} ${QUERY_GRAPH}
# ./bin/lb.out ${DATA_GRAPH} ${QUERY_GRAPH}
# rm core.*
