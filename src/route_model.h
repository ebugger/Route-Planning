#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:

        Node * parent = nullptr;
        //4 basic attr used for A* Search
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node *> neighbors;
        //find the distance between two nodes,This will allow the search algorithm to find the closest node to the current node
        //The distance method shouldn't change the object being passed, so you can make it a const method (add const after the 
        //function name).
        float distance(Node other) const {
          //euclidean distance
          return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
        }
        void FindNeighbors();
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
        int GetDistance()  const { return index; }
      
      private:
        int index;
        //it allows each node to access data stored in the parent model that the node belongs to
        RouteModel * parent_model = nullptr;
        // to return a pointer to the closest unvisited node from a vector of node indices, where the distance is measured to the 
        // current node (this).
        Node* FindNeighbor(std::vector<int> node_indices);
    };
    
    // Add public RouteModel variables and methods here.
    RouteModel(const std::vector<std::byte> &xml); 
    // public "getter" method SNodes. This method return a reference to the vector of Nodes stored as m_Nodes.
    auto &SNodes() {return m_Node;}
    // This variable will eventually store the path that is found by the A* search.
    std::vector<Node> path; 
    //used for testing
    auto &GetNodeToRoadMap() {return node_to_road;}
    RouteModel::Node &FindClosestNode(float x, float y);
  private:
    //method will operate only on the node_to_road variable declared below, and only within the RouteModel class, so it can 
    //be private, it needs no arguments, and can have void return type.
    void CreateNodeToRoadHashmap();
    //an unordered_map with an int key type, and a vector of const Model::Road* as the value type.
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
    //This will store all of the nodes from the Open Street Map data.
    std::vector<Node> m_Node;

};

#endif
