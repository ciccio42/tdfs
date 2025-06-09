import argparse
import glob
import os
from collections import OrderedDict

def convert(src_file_path, dest_file_path, data_graph=False, graph_name=''):

    # open file
    with open(src_file_path) as f:

        # read lines
        lines = f.readlines()

        num_nodes = 0
        # num_edges = 0
        nodes_dict = OrderedDict()
        edges_dict = OrderedDict()
        num_edges = 0
        max_vertex_label = 0
        max_edge_label = 1
        for line_indx, line in enumerate(lines):
            if line_indx == 0:
                num_nodes = int(line.split('\n')[0])
                print(f"Number of nodes: {num_nodes}")
            
            elif(line_indx - 1) < num_nodes:
                line_elements = line.split(' ')  
                vertex_label = int(line_elements[1].split('\n')[0])  
                nodes_dict[int(line_elements[0])] = vertex_label # id, label 
                if vertex_label > max_vertex_label:
                    max_vertex_label = vertex_label
            
            line_elements = line.split(' ')
            if ((line_indx - 1) >= num_nodes and (len(line_elements) == 1)):
                num_edges += int(line_elements[0])
            elif ((line_indx - 1) >= num_nodes and (len(line_elements) == 2)):
                if edges_dict.get(int(line_elements[0]), None) is None:
                    edges_dict[int(line_elements[0])] = []
                edges_dict[int(line_elements[0])].append(int(line_elements[1].split('\n')[0])) # src, dest
                
                
    # print(edges_dict) 
    # start convertion
    with open(dest_file_path, 'w') as f:
        # write header
        if not data_graph:      
            f.write(f"# t 0\n")
        else:
            f.write(f"# \n# {graph_name}\n# Nodes: {num_nodes} # Edges: {num_edges}\n")
        # f.write(f"{num_nodes} {num_edges} {max_vertex_label} {max_edge_label}\n")
        
        for node_id, node_label in nodes_dict.items():
            f.write(f"v {node_id} {node_label}\n")
            
        for src in edges_dict.keys():
            for dest in edges_dict[src]:
                f.write(f"e {src} {dest} {1}\n")
        if not data_graph:
            pass
            #f.write("# t -1\n")
    f.close()
        


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--src_folder_path', type=str,
                        default='/dataset/DBLP')
    parser.add_argument('--dest_folder_path', type=str,
                        default='/dataset/DBLP/GSI_format')
    parser.add_argument('--graph_path', type=str,
                        default='')
    parser.add_argument('--graph_name', type=str,
                        default='DBLP')
    parser.add_argument('--data_graph', action='store_true')
    args = parser.parse_args()

    
    # debugpy.listen(('0.0.0.0', 5678))
    # debugpy.wait_for_client()

    print(f"Loading files from {args.src_folder_path}")
    src_graph_paths = glob.glob(os.path.join(
        args.src_folder_path, args.graph_path, "*.grf"))
    print(src_graph_paths)

    out_folder = os.path.join(os.path.join(
        args.dest_folder_path, args.graph_path))
    if not os.path.isdir(out_folder):
        os.makedirs(out_folder, exist_ok=True)

    for indx, graph_path in enumerate(src_graph_paths):
        # if indx == 0:
        file_name = graph_path.split('/')[-1]
        print(f"Converting file {graph_path}")
        convert(src_file_path=graph_path,
                dest_file_path=os.path.join(out_folder, file_name),
                data_graph=args.data_graph,
                graph_name=args.graph_name)
