#pragma once

#include "graph.h"

namespace STMatch
{

  typedef struct
  {
    graph_node_t nodes[2];
  } Job;

  typedef struct
  {
    Job *q;
    int start_level;
    graph_node_t length;
    unsigned long long cur = 0;
    int mutex = 0;
  } JobQueue;

  struct JobQueuePreprocessor
  {

    JobQueue q;

    JobQueuePreprocessor(Graph &g, PatternPreprocessor &p)
    {

      std::cout << "start JobQueuePreprocessor..." << std::endl;
      std::vector<graph_node_t> vr, vc;
      long cnt = 0;
      for (graph_node_t r = 0; r < g.nnodes; r++)
      {
        for (graph_edge_t j = g.rowptr[r]; j < g.rowptr[r + 1]; j++)
        {
          graph_node_t c = g.colidx[j];
          if ((!LABELED && p.partial[0][0] == 1 && r < c) || LABELED || p.partial[0][0] != 1)
          {
            // if (r != c) {
            // if ((g.vertex_label[r] == (1 << p.vertex_labels[0])) && (g.vertex_label[c] == (1 << p.vertex_labels[1])))
            if (!LABELED || (g.vertex_label[r] == p.pat.vertex_labels[0] && g.vertex_label[c] == p.pat.vertex_labels[1]))
            {

              if (g.rowptr[r + 1] - g.rowptr[r] >= p.pat.degree[0] && g.rowptr[c + 1] - g.rowptr[c] >= p.pat.degree[1])
              {
                bool valid = false;
                for (graph_edge_t d = g.rowptr[c]; d < g.rowptr[c + 1]; d++)
                {
                  graph_node_t v = g.colidx[d];
                  if (g.rowptr[v + 1] - g.rowptr[v] >= p.pat.degree[2])
                  {
                    valid = true;
                    break;
                  }
                }

                if (valid)
                {
                  cnt += 1;
                  vr.push_back(r);
                  vc.push_back(c);
                }
              }
            }
          }
        }
      }
      std::cout << "cnt = " << cnt << std::endl;
      q.q = new Job[vr.size()];
      for (graph_edge_t i = 0; i < vr.size(); i++)
      {
        (q.q[i].nodes)[0] = vr[i];
        (q.q[i].nodes)[1] = vc[i];
      }
      q.length = vr.size();
      q.cur = 0;
      q.start_level = 2;
    }

    JobQueue *to_gpu()
    {
      JobQueue qcopy = q;
      cudaMalloc(&qcopy.q, sizeof(Job) * q.length);
      cudaMemcpy(qcopy.q, q.q, sizeof(Job) * q.length, cudaMemcpyHostToDevice);

      JobQueue *gpu_q;
      cudaMalloc(&gpu_q, sizeof(JobQueue));
      cudaMemcpy(gpu_q, &qcopy, sizeof(JobQueue), cudaMemcpyHostToDevice);
      return gpu_q;
    }
  };
}