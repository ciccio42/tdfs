DATABASE_PATH=/dataset/DBLP/TDFS_format/

txtdir=../data/txt_graph/
bindir=../data/bin_graph/
mkdir -p ${txtdir}
mkdir -p ${bindir}

# Copy all data*.grf files to txtdir
cp ${DATABASE_PATH}data*.grf ${txtdir}

for graph in ${txtdir}*; do
    filename=${graph##*/}
    filename=${filename%.txt}
    mkdir -p ${bindir}/${filename}
    cp ${graph} ${bindir}/${filename}/snap.txt
done

# Copy all pattern graphs to patterns directory
mkdir -p ../data/pattern
find "$DATABASE_PATH" -mindepth 1 -maxdepth 1 -type d -exec cp -r {} ../data/pattern/ \;

for graph in ${bindir}*; do
    echo "Converting ${graph} to binary format"
    bash convert_vf_database.sh ${graph}
done

chmod 777 -R ../data
# graph="../data/bin_graph/data_64.grf"
# bash convert_vf_database.sh ${graph}
