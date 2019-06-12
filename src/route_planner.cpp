#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y); 
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    std::vector<RouteModel::Node> path_found;
    distance = 0.0f;
    RouteModel::Node parent;
    while (current_node->parent != nullptr) {
        path_found.push_back(*current_node);
        parent = *(current_node->parent);
        distance += current_node->distance(parent);
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);
    distance *= m_Model.MetricScale();
    return path_found;
}

float RoutePlanner::CalculateHValue(const RouteModel::Node * other) {
    return other->distance(*end_node);
}

RouteModel::Node * RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd) {
        return _1st->h_value + _1st->g_value < _2nd->h_value + _2nd->g_value;
    });
    RouteModel::Node * lowest_node = open_list.front();
    open_list.erase(open_list.begin());

    return lowest_node;
}

void RoutePlanner::AddNeighbors(RouteModel::Node *tmp_node) {
    tmp_node->FindNeighbors();
    //for (auto tmp : tmp_node->neighbors) {
    //std::cout<< tmp << "\n";
    //}
    //std::cout << "there are " << tmp_node->neighbors.size() <<  "neighbours";
    for (auto neigb_node : tmp_node->neighbors) {
        //Set the neighbors parent to the current_node
        neigb_node->parent = tmp_node;
        //std::cout<< "neigb_node->parent = tmp_node;" << "\n";
        float new_g = tmp_node->g_value + tmp_node->distance(*neigb_node);
        //std::cout<< "new_g" << new_g <<"\n";
        neigb_node->g_value = new_g;
        //neigb_node->g_value = tmp_node->g_value + tmp_node->distance(*neigb_node);
        //std::cout<< "g_value = tmp_node->g_value + tmp_node->distance(*neigb_node); done" << "\n";
        neigb_node->h_value = CalculateHValue(neigb_node);
        //std::cout<< "neigb_node->h_value = CalculateHValue(neigb_node); done" << "\n";
        open_list.push_back(neigb_node);
        neigb_node->visited = true;
    }
}

void RoutePlanner::AStarSearch() {
    //below 3 lines used to directly link start and end node
    //end_node->parent = start_node;
    //m_Model.path = ConstructFinalPath(end_node);
    //return;
    start_node->visited = true;
    open_list.push_back(start_node);
    RouteModel::Node *current_node = nullptr;
    while (open_list.size() > 0)
    {
        current_node = NextNode();
        //std::cout<< "current_node = NextNode() done" << "\n";
        if (current_node->distance(*end_node) == 0)
        {
            m_Model.path = ConstructFinalPath(current_node);
            return ;
        }
        AddNeighbors(current_node);
        //std::cout<< "AddNeighbors(current_node) done" << "\n";
     }   
}