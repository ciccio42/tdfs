#! /bin/bash

# Run container with name DOCKER_NAME, open a terminal, mount the folder algorithm, give name gpsm_container to the container
DOCKER_NAME="tdfs"

docker run --name tdfs_container --rm -v /user/frosa/graph_matching/graph-matching-analysis:/graph-matching-analysis -v /user/frosa/graph_matching/dataset:/dataset --gpus all -p 1236:1236 -p 2222:22 -it --detach-keys=ctrl-d $DOCKER_NAME /bin/bash
