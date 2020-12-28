#ifndef FLOW_GRAPH_H
#define FLOW_GRAPH_H

#include <DGtal/helpers/StdDefs.h>
#include <lemon/adaptors.h>
#include <lemon/graph_to_eps.h>
#include <lemon/list_graph.h>
#include <lemon/preflow.h>

#include "LinelData.h"

class FlowGraph {
 public:
  typedef lemon::ListDigraph ListDigraph;
  typedef ListDigraph::Node Node;
  typedef ListDigraph::ArcMap<double> ArcWeightMap;  // MinCut computation
  typedef lemon::Preflow<ListDigraph, ListDigraph::ArcMap<double> >
      FlowComputer;

  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::Z2i::Point Point;
  typedef DGtal::Z2i::KSpace KSpace;

  typedef std::unordered_map<Point, Node> PointToNode;
  typedef ListDigraph::NodeMap<Point> NodeToPoint;

  typedef std::unordered_map<Point, LinelData>& CostFunction;

 public:
  FlowGraph(const KSpace& kspace, const DigitalSet& shape,
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

 public:
  std::unordered_map<Point, int> vertexSet;
  std::set<Point> sourcePoints;
  std::set<Point> targetPoints;
};

#endif