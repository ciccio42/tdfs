import numpy as np
import struct
from tqdm import tqdm
import os

def read_graph_from_snap_files(DATA_PATH):
    
    # files needed for the edges
    vertex_bin_file = DATA_PATH + ".vertex.bin"
    edge_bin_file = DATA_PATH + ".edge.bin"
    meta_file = DATA_PATH + ".meta.txt"
    vertex_label_file = DATA_PATH + ".label.bin"
    
    with open(meta_file, 'r') as f:
        meta_data = f.readlines()
    meta_data = [line.strip() for line in meta_data if line.strip()]
    num_nodes = int(meta_data[0])
    num_edges = int(meta_data[1])
    print(f"Number of nodes: {num_nodes}, Number of edges: {num_edges}\n")
    
    # read offset file
    vertex_offset = []
    print(f"Reading vertex offset file {vertex_bin_file}")
    with open(vertex_bin_file, 'rb') as f:
        
        vrtx_id = 0
        while True:
            data = f.read(8)
            # print(data)
            if not data:
                break
            offset = struct.unpack('<II', data)[0]
            vertex_offset.append(offset)
            vrtx_id += 1

    print("\toffsets:", vertex_offset[:10])
    print("\tNumber of vertices:", len(vertex_offset))
    assert len(vertex_offset)-1 == num_nodes, f"Vertex offset count {len(vertex_offset)-1} does not match number of nodes {num_nodes}" 
    
    # read edges
    print(f"\nReading edge file {edge_bin_file}")
    edge_list = []    
    with open(edge_bin_file, 'rb') as f:    
        while True:
            data = f.read(4)
            if not data:
                break
            dst_indx = struct.unpack('<I', data)[0]
            # print(f"Vertex {vrtx_id} has edge to {dst_indx}")
            edge_list.append(dst_indx)
    print("\tFirst 10 edges:", edge_list[:10])
    print("\tNumber of edges read:", len(edge_list))
    assert len(edge_list) == num_edges, f"Edge count {len(edge_list)} does not match number of edges {num_edges}"
    
    # read vertex labels
    vertex_label_dict = {}
    vertex_id = 0
    with open(vertex_label_file, 'rb') as f:
        while True:
            data = f.read(4)
            if not data:
                break
            label = struct.unpack('<I', data)[0]
            vertex_label_dict[vertex_id] = label
            vertex_id += 1

    print("\nNumber of vertex labels read:", len(vertex_label_dict.keys()))
    assert len(vertex_label_dict) == num_nodes, "Vertex label count does not match number of nodes"
    
    # write with vf format
    print("\nWriting to vf format...")
    with open(f"{DATA_PATH.split('/')[-2]}_vf.grf", 'w') as f:
        # write number of nodes
        f.write(f"{num_nodes}\n")
        
        # write vrtx_id and label
        for vrtx_id in range(num_nodes):
            label = vertex_label_dict[vrtx_id]
            f.write(f"{vrtx_id} {label}\n")
            
        # for each vertex, write its edges
        for vrtx_id in range(num_nodes):
            start = vertex_offset[vrtx_id]
            end = vertex_offset[vrtx_id + 1]
            edges = edge_list[start:end]
            
            f.write(f"{len(edges)}\n")
            for edge in edges:
                f.write(f"{vrtx_id} {edge}\n")

def convert_graphs(pattern_folder):
    import glob
    pattern_files = glob.glob(os.path.join(pattern_folder, "*.g"))
    for pattern_file in pattern_files:
        print(f"Converting {pattern_file} to vf format...")
        
        vertex_label_dict = {}
        edge_label_dict = {}
        with open(pattern_file, 'r') as f:
            lines = f.readlines()
        
            for line in lines:
                if line[0] == '#':
                    continue
                
                if line[0] == 'v':
                    vrtx_id = int(line[2])
                    label = int(line[4])
                    vertex_label_dict[vrtx_id] = label
                    if vrtx_id not in edge_label_dict:
                        edge_label_dict[vrtx_id] = []
                    print(f"\nVertex {vrtx_id} has label {label}")
                
                elif line[0] == 'e':
                    src_indx = int(line[2])
                    dst_indx = int(line[4])
                    edge_label = int(line[6])
                    
                    edge_label_dict[src_indx].append(dst_indx)

        # write to vf format
        data_path  = pattern_file.replace(".g", "_vf.sub.grf")

        with open(data_path, 'w') as f:
            # write number of nodes
            f.write(f"{len(vertex_label_dict)}\n")

            # write vertex labels
            for vrtx_id, label in vertex_label_dict.items():
                f.write(f"{vrtx_id} {label}\n")

            # write edges
            for src_indx, dst_indxs in edge_label_dict.items():
                f.write(f"{len(dst_indxs)}\n")
                for dst_indx in dst_indxs:
                    f.write(f"{src_indx} {dst_indx}\n")


if __name__ == "__main__":
    DATA = True  # toggle to read data from files
    if DATA:
        DATA_PATH = "/graph-matching-analysis/baseline_algorithms/tdfs/data/bin_graph/com-youtube.ungraph/snap.txt"
        # DEGREE_FILE = EDGE_FILE.replace("snap_edges.bin", "degrees.bin")
        read_graph_from_snap_files(DATA_PATH)
    else:
        pattern_folder = "/graph-matching-analysis/baseline_algorithms/tdfs/data/pattern"
        convert_graphs(pattern_folder)
