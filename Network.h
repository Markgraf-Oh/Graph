#pragma once

#include <random>
#include "AdjacencyMultiList.h"

namespace Network
{
	namespace AML = AdjacencyMultiList;
	
    /** Clear the Network and Connect Nodes to make ER Network
    */
    template<typename VT, typename ET>
    void ConnectERNetwork(AML::Graph<VT, ET>* network, int mean_degree);

    /** Initialize Network with given size and Connect Nodes to make ER Network
    */
    template<typename VT, typename ET>
    void InitializeERNetwork(AML::Graph<VT, ET>* network, int mean_degree, int network_size = 0);

    /** Clear the Network and Connect Nodes to make Barabasi-Albert Network
    */
    template<typename VT, typename ET>
    void ConnectBANetwork(AML::Graph<VT, ET>* network, int connection_per_step);

    /** Initialize Network with given size and Connect Nodes to make Barabasi-Albert Network
    */
    template<typename VT, typename ET>
    void InitializeBANetwork(AML::Graph<VT, ET>* network, int connection_per_step, int network_size);

    /** Clear the Network and Connect Nodes to make Barabasi-Albert Network
    */
    template<typename VT, typename ET>
    void InitializeLatticeNetwork(AML::Graph<VT, ET>* network, int x, int y);

    /** sort by clusters
    */
    template<typename VT, typename ET>
    void FindClusters(AML::Graph<VT, ET>* network, std::vector<std::vector<AML::Vertex<VT, ET>*>> & return_reference);
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
    Network::ConnectERNetwork(network, mean_degree);
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
        for(int j = 0; j < connection_per_step;)
        {
            int target_degree_cumulative = int(uniform_dist(generator) * network->GetEdgeNumber() * 2);
            int current_degree_cumulative = 0;
            int target_index = 0;
            while(target_index < i - 1)
            {
                current_degree_cumulative += network->vertex_list[target_index]->GetDegree();
                if(current_degree_cumulative >= target_degree_cumulative) break;
                target_index++;
            }

            if(network->IsConnected(i, target_index)) continue;

            network->Connect(i, target_index);
            j++;
        }
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

    Network::ConnectBANetwork(network, connection_per_step);
}

template<typename VT, typename ET>
void Network::InitializeLatticeNetwork(AML::Graph<VT, ET>* network, int x_size, int y_size)
{
    if((x_size <= 0) || (y_size <= 0) || (network == nullptr)) return;

    network->Initialize(x_size * y_size);

    std::vector<std::pair<int, int>> directions;
    directions.emplace_back(0, 1);
    directions.emplace_back(0, -1);
    directions.emplace_back(1, 0);
    directions.emplace_back(-1, 0);

    for(int x = 0; x < x_size; x++)
    {
        for(int y = 0; y < y_size; y++)
        {
            for(std::pair<int, int> direction : directions)
            {
                int target_x = x + direction.first;
                int target_y = y + direction.second;
                if((target_x < 0) || (target_x >= x_size) || (target_y < 0) || (target_y >= y_size)) continue;
                network->Connect((x * x_size) + y, (target_x * x_size) + target_y);
            }
        }
    }

}

template<typename VT, typename ET>
void Network::FindClusters(AML::Graph<VT, ET>* network, std::vector<std::vector<AML::Vertex<VT, ET>*>>& clusters_return)
{
    clusters_return.clear();
    network->ResetVertexIndex();

    std::vector<bool> vertex_marker(network->vertex_list.size(), false);
    
    std::deque<AML::Vertex<VT, ET>*> BFSQueue;
    
    for(int i = 0; i < network->vertex_list.size(); i++)
    {
        if(vertex_marker[i] == true) continue;

        //initialize
        vertex_marker[i] = true;
        BFSQueue.clear();
        clusters_return.push_back(std::vector<AML::Vertex<VT, ET>*>());
        std::vector<AML::Vertex<VT, ET>*> &cluster = clusters_return.back();

        //Broad First Search
        BFSQueue.push_back(network->vertex_list[i]);

        while(!BFSQueue.empty())
        {
            AML::Vertex<VT, ET>* current_vertex = BFSQueue.front();
            AML::Edge<VT, ET>* current_edge = current_vertex->GetFront();
            
            while(current_edge != nullptr)
            {
                AML::Vertex<VT, ET>* opposite = current_edge->GetOpposite(current_vertex);
                if((opposite != nullptr) && (vertex_marker[opposite->index] == false))
                {
                    BFSQueue.push_back(opposite);
                    vertex_marker[opposite->index] = true;
                }
                current_edge = current_edge->GetNext(current_vertex);
            }
            cluster.push_back(current_vertex);
            BFSQueue.pop_front();
        }
    }
}