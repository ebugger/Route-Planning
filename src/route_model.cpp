#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int counter = 0;
  for (Model::Node node : this->Nodes()) {
    //The second argument should be a pointer to the RouteModel instance that the Node belongs to. Since you are writing your code 
    //inside the RouteModel constructor, you can use the this keyword to get a pointer to the current RouteModel instance. In other 
    //words, you can pass this as the second argument.
    m_Node.push_back(Node(counter, this, node));
    counter++;
  }
  /*
  std::vector<Model::Node> nodes = this->Nodes();
  for(int i=0,i<nodes.size(),i++) {
    m_Node.push_back(Node(i, this, nodes[i]));
    }
  */

  CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap() {
    for (const Model::Road &road : Model::Roads()) {
        if (road.type != Model::Road::Type::Footway) {
           for (int node_idx : Model::Ways()[road.way].nodes) {
               if (node_to_road.find(node_idx) == node_to_road.end()) {
                   node_to_road[node_idx] = std::vector<const Model::Road*> ();
               /*
               std::unordered_map<int, std::vector<const Model::Road *>>::const_iterator got = node_to_road.find(node_idx);
               if (got == node_to_road.end())
               */
               }
               node_to_road[node_idx].push_back(&road);
           } 
        }
    }
}

RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices) {
    Node *closest_node = nullptr;
    Node node;
    for (int node_index : node_indices) {
        //std::cout << "node_indices" << node_index<< "\n";
        node = parent_model->SNodes()[node_index];
        //std::cout << "node_addr" << &node<< "\n";
        if (this->distance(node) != 0 && !node.visited) {
            if (closest_node == nullptr || this->distance(node) < this->distance(*closest_node))  {
                //closest_node = &node; ！！！！！ ptr to ptr
                closest_node = &parent_model->SNodes()[node_index];
                //std::cout << "----closest_node----" << index<<  "----closest_addr----" <<  &node <<"\n";
            }
        }
    }
    return closest_node;
}

void RouteModel::Node::FindNeighbors() {
    RouteModel::Node *neighbor_node = nullptr ;
    for (const Model::Road * road : parent_model->node_to_road[this->index]) {
        //std::cout << "node_to_road:" << road << "\n" ;
        neighbor_node = FindNeighbor(parent_model->Ways()[road->way].nodes);
        if (neighbor_node) {
            this->neighbors.push_back(neighbor_node);
            //std::cout << "neighbor_node" << neighbor_node << "\n" ;
        }
    }
}

RouteModel::Node & RouteModel::FindClosestNode(float x, float y) {
    Node temp_node;
    temp_node.x = x;
    temp_node.y = y;

    float min_dist = std::numeric_limits<float>::max();
    int closest_idx;
    float temp_dis;

    for (const Model::Road &road : Model::Roads()) {
        if (road.type != Model::Road::Type::Footway) {
            for (int node_index : Model::Ways()[road.way].nodes) {
                temp_dis = temp_node.distance(SNodes()[node_index]);
                if (temp_dis < min_dist) {
                    min_dist = temp_dis;
                    closest_idx = node_index;
                }
            }
        }
    } 
    return SNodes()[closest_idx];
}
