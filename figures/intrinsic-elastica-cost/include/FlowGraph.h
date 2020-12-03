#ifndef FLOW_GRAPH_H
#define FLOW_GRAPH_H

#include <DGtal/helpers/StdDefs.h>
#include <lemon/adaptors.h>
#include <lemon/graph_to_eps.h>
#include <lemon/list_graph.h>
#include <lemon/preflow.h>

#include "CostData.h"
#include "CostFunction.h"

class FlowGraph {
 public:
  typedef lemon::ListDigraph ListDigraph;
  typedef ListDigraph::Node Node;
  typedef ListDigraph::ArcMap<double> ArcWeightMap;  // MinCut computation
  typedef lemon::Preflow<ListDigraph, ListDigraph::ArcMap<double> >
      FlowComputer;

  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::Z2i::Point Point;

  typedef std::unordered_map<Point, Node> PointToNode;
  typedef ListDigraph::NodeMap<Point> NodeToPoint;

 public:
  FlowGraph(const DigitalSet& vertexSet, const std::set<Point>& sourcePoints,
            const std::set<Point>& targetPoints,
            const CostFunction& costFunction);
  ~FlowGraph() { delete flow; }
  FlowGraph(const FlowGraph& fg) = delete;
  FlowGraph(FlowGraph&& fg) = delete;

  void run() { flow->run(); }
  inline double cutValue() { return flow->flowValue(); }
  inline bool isInSource(const Point& p) { return flow->minCut(ptn[p]); }

 private:
  void addNode(Point p);

 private:
  ListDigraph digraph;
  ArcWeightMap arcWeightMap;

  ListDigraph::Node terminalSource;
  ListDigraph::Node terminalTarget;

  PointToNode ptn;
  NodeToPoint ntp;

  FlowComputer* flow;
};

#endif