#!/bin/bash

python3 convert_vf_to_tdfs.py \
    --src_folder_path="/dataset/DBLP" \
    --graph_path="8/node_induced/original_labels" \
    --dest_folder_path="/dataset/DBLP/TDFS_format"

python3 convert_vf_to_tdfs.py \
    --src_folder_path="/dataset/DBLP" \
    --graph_path="8/node_induced/label_64" \
    --dest_folder_path="/dataset/DBLP/TDFS_format"

python3 convert_vf_to_tdfs.py \
    --src_folder_path="/dataset/DBLP" \
    --graph_path="8/node_induced/label_32" \
    --dest_folder_path="/dataset/DBLP/TDFS_format"

# Data-Graphs
python3 convert_vf_to_tdfs.py \
    --src_folder_path="/dataset/DBLP" \
    --graph_path="" \
    --dest_folder_path="/dataset/DBLP/TDFS_format" \
    --data_graph
