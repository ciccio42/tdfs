#!/bin/bash

DATA_GRAPH=${1:-"./data/bin_graph/data.grf/snap.txt"}
QUERY_GRAPH=${2:-"./data/pattern/8/edge_induced/original_labels/connected_query_0.sub.grf"}
gdbserver localhost:1236 ./bin/ulb.out ${DATA_GRAPH} ${QUERY_GRAPH}

rm core.*
