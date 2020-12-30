#include "FlowGraph.h"

FlowGraph::FlowGraph(const KSpace& kspace, const DigitalSet& shape,
                     const CostFunction& costFunction)
    : arcWeightMap(digraph), ntp(digraph) {
  terminalSource = digraph.addNode();
  terminalTarget = digraph.addNode();  

  for (auto pk : costFunction) {
    Point linelKCoords = pk.first;
    KSpace::Cell linelCell = kspace.uCell(linelKCoords);
    auto pixels = kspace.uUpperIncident(linelCell);

    Point p1Coords = kspace.uCoords(pixels[0]);
    Point p2Coords = kspace.uCoords(pixels[1]);
    if (vertexSet.find(p1Coords) == vertexSet.end()) {
      vertexSet[p1Coords] = 1;
    } else {
      vertexSet[p1Coords] += 1;
    }

    if (vertexSet.find(p2Coords) == vertexSet.end()) {
      vertexSet[p2Coords] = 1;
    } else {
      vertexSet[p2Coords] += 1;
    }
  }

  for (auto pk : vertexSet) {
    Point coords = pk.first;
    int neighbors = pk.second;

    if (neighbors < 4 ) {    // Terminal point
      if (shape(coords)) {  // Source point
        sourcePoints.insert(coords);
      }else{
        targetPoints.insert(coords);
      }
    }
  }

  for (auto pk : vertexSet) addNode(pk.first);

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
    double cost = pk.second;
    KSpace::Cell linelCell = kspace.uCell(linelKCoords);

    auto pixels = kspace.uUpperIncident( linelCell );
    Point p1 = kspace.uCoords(pixels[0]);
    Point p2 = kspace.uCoords(pixels[1]);

    ListDigraph::Arc a1 =
        digraph.addArc(ptn[p1], ptn[p2]);
    ListDigraph::Arc a2 =
        digraph.addArc(ptn[p2], ptn[p1]);

    arcWeightMap[a1] = cost;
    arcWeightMap[a2] = cost;
  }

  flow =
      new FlowComputer(digraph, arcWeightMap, terminalSource, terminalTarget);
  flow->run();
}

void FlowGraph::addNode(Point p) {
  ptn[p] = digraph.addNode();
  ntp[ptn[p]] = p;
}