#pragma once

#include <random>
#include "AdjacencyMultiList.h"
#include "Network.h"

int GetRandomIndex(std::minstd_rand& generator, int size)
{
    std::uniform_int_distribution<int> dist_uniform_int(0, size - 1);

    return dist_uniform_int(generator);
}

void ShowDegreeDistribution(AdjacencyMultiList::Graph<int, float>& target_graph)
{
    std::cout << "number of vertices : " << target_graph.vertex_list.size() << std::endl;
    std::cout << "number of edges : " << target_graph.GetEdgeNumber() << std::endl;
    int degree_sum = 0;
    for(AdjacencyMultiList::Vertex<int, float>* vertex : target_graph.vertex_list)
    {
        degree_sum += vertex->GetDegree();
    }
    std::cout << "vertex degree sum : " << degree_sum << std::endl;

    int node_number = target_graph.vertex_list.size();

    std::vector<int> histogram(node_number, 0);

    for(int i = 0; i < node_number; i++)
    {
        histogram[target_graph.vertex_list[i]->CalculateDegree()] += 1;
    }

    if(histogram.size())
        for(int i = 0; (i < histogram.size() && i < 20); i++)
        {
            std::cout << histogram[i] << "\t";
            std::cout << std::string(histogram[i] / 50, '*') << std::endl;
        }
}

void test1()
{
    namespace aml = AdjacencyMultiList;

    int node_number = 10000;
    int mean_degree = 4;
    float connect_ratio = (float)mean_degree / (float)node_number;

    aml::Graph<int, float> test_graph(node_number);

    std::vector<int> histogram(node_number, 0);

    std::minstd_rand generator_basic(42);

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

    Network::InitializeBANetwork(&test_graph, 2, node_number);

    ShowDegreeDistribution(test_graph);

    std::cout << "\n\n" << std::string(30, '*') << "\n";

    std::cout << "er" << std::endl;

    Network::InitializeERNetwork(&test_graph, 4, node_number);

    ShowDegreeDistribution(test_graph);

    char a;
    std::cin >> a;
}