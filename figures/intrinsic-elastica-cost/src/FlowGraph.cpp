#include "FlowGraph.h"

FlowGraph::FlowGraph(const DigitalSet& vertexSet,
                     const std::set<Point>& sourcePoints,
                     const std::set<Point>& targetPoints,
                     const CostFunction& costFunction)
    : arcWeightMap(digraph), ntp(digraph) {
  terminalSource = digraph.addNode();
  terminalTarget = digraph.addNode();

  for (Point p : vertexSet) addNode(p);

  for (Point p : sourcePoints) addNode(p);
  for (Point p : targetPoints) addNode(p);

  // Terminal arcs
  for (auto p : sourcePoints) {
    ListDigraph::Arc a = digraph.addArc(terminalSource, ptn[p]);
    arcWeightMap[a] = 1e5;
  }

  for (auto p : targetPoints) {
    ListDigraph::Arc a = digraph.addArc(ptn[p], terminalTarget);
    arcWeightMap[a] = 1e5;
  }

  // Elastica arcs
  for (auto pk : costFunction) {
    Point linelKCoords = pk.first;
    CostData cd = pk.second;

    ListDigraph::Arc a1 = digraph.addArc(ptn[cd.pixel1], ptn[cd.pixel2]);
    ListDigraph::Arc a2 = digraph.addArc(ptn[cd.pixel2], ptn[cd.pixel1]);

    arcWeightMap[a1] = cd.cost;
    arcWeightMap[a2] = cd.cost;
  }

  flow = new FlowComputer(digraph,arcWeightMap,terminalSource,terminalTarget);
  flow->run();
}

void FlowGraph::addNode(Point p) {
  ptn[p] = digraph.addNode();
  ntp[ptn[p]] = p;
}