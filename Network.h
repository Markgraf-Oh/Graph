#pragma once

#include <random>
#include "AdjacencyMultiList.h"

namespace Network
{
	namespace AML = AdjacencyMultiList;
	
    template<typename VT, typename ET>
    void ConnectERNetwork(AML::Graph<VT, ET>* network, int mean_degree);

    template<typename VT, typename ET>
    void InitializeERNetwork(AML::Graph<VT, ET>* network, int mean_degree, int network_size = 0);

    template<typename VT, typename ET>
    void ConnectBANetwork(AML::Graph<VT, ET>* network, int connection_per_step);

    template<typename VT, typename ET>
    void InitializeBANetwork(AML::Graph<VT, ET>* network, int connection_per_step, int network_size);
}

template<typename VT, typename ET>
void Network::ConnectERNetwork(AML::Graph<VT, ET>* network, int mean_degree)
{
    if(mean_degree <= 0) throw std::invalid_argument("mean_degree <= 0");

    if(network == nullptr) throw std::invalid_argument("network == nullptr");

    network->ClearEdge();

    std::random_device rande;

    std::minstd_rand generator_basic(rande());

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    
    float connect_ratio = float(mean_degree) / float(network->vertex_list.size() - 1);

    //Creating ER Network
    for(int i = 1; i < network->vertex_list.size(); i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(uniform_dist(generator_basic) < connect_ratio)
            {
                network->Connect(i, j);
            }
        }
    }
}

template<typename VT, typename ET>
void Network::InitializeERNetwork(AML::Graph<VT, ET>* network, int mean_degree, int network_size)
{

    if(mean_degree <= 0) throw std::invalid_argument("mean_degree <= 0");

    if(network == nullptr) throw std::invalid_argument("network == nullptr");

    if(network_size <= 0 ) throw std::invalid_argument("network_size <= 0");

    network->Initialize(network_size);
    ConnectERNetwork(network, mean_degree);
}

template<typename VT, typename ET>
void Network::ConnectBANetwork(AML::Graph<VT, ET>* network, int connection_per_step)
{
    if(network == nullptr) throw std::invalid_argument("network == nullptr");
    if(connection_per_step < 1) throw std::invalid_argument("connection_per_step < 1");
    int init_size = 2 * connection_per_step + 1;
    if(network->vertex_list.size() < init_size) throw std::invalid_argument("network size < 2 * connection_per_step + 1");

    network->ClearEdge();

    std::random_device rande;

    std::minstd_rand generator(rande());

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

    //make initail complete graph
    for(int i = 1; i < init_size; i++)
    {
        for(int j = 0; j < i; j++)
        {
            network->Connect(i, j);
        }
    }

    for(int i = init_size; i < network->vertex_list.size(); i++)
    {
        int target_degree_cumulative = int(uniform_dist(generator) * network->GetEdgeNumber() * 2);
        int current_degree_cumulative = 0;
        int target_index = 0;
        while(target_index < i-1)
        {
            current_degree_cumulative += network->vertex_list[target_index]->GetDegree();
            if(current_degree_cumulative >= target_degree_cumulative) break;
            target_index++;
        }
        network->Connect(i, target_index);
    }
}

template<typename VT, typename ET>
void Network::InitializeBANetwork(AML::Graph<VT, ET>* network, int connection_per_step, int network_size)
{
    if(connection_per_step < 1) throw std::invalid_argument("connection_per_step < 1");
    int init_size = 2 * connection_per_step + 1;
    if(network_size < 2 * connection_per_step + 1) throw std::invalid_argument("network_size < 2 * connection_per_step + 1");
    if(network == nullptr) throw std::invalid_argument("network == nullptr");

    network->Initialize(network_size);

    ConnectBANetwork(network, connection_per_step);
}
