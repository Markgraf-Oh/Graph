#pragma once

#include <random>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>
#include <iomanip>
#include "AdjacencyMultiList.h"
#include "Network.h"
#include "MinimumSpanningTree.h"
#include "BenchMark.h"

namespace Test
{
#pragma region Declare
    int GetRandomIndex(std::minstd_rand& generator, int size);

    void ShowDegreeDistribution(AdjacencyMultiList::Graph<int, float>& target_graph);

    void TestNeworkBuild();

    void TestClustering();

    //각 스레드에서 돌아갈 함수입니다.
    void ThreadBADegreeDistribution(int thread_id,
                                    std::size_t node_number,
                                    std::size_t connection_per_step,
                                    std::size_t loop_time,
                                    std::mutex &cout_mutex,
                                    std::promise<std::vector<double>> result_promise);

    /** 멀티 스레딩을 통해 BA네트워크의 Degree Distribution을 구하는 예제함수입니다.
    */
    void TestMultiThreadDegreeDistribution();

    /** Testing Prim Algorithm with BA Network graph.
    * BA Network를 사용하는 이유는 모든 vertex가 하나의 cluster로 연결되어 있기 때문.
    */
    void TestPrimAlgorithm(int network_size, int connection_step);

    /** Testing Kruskal Algorithm with BA Network graph.
    * BA Network를 사용하는 이유는 모든 vertex가 하나의 cluster로 연결되어 있기 때문.
    */
    void TestKruskalAlgorithm(int network_size, int connection_step);

#pragma endregion Decalre functions

#pragma region Definition

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

    void TestNeworkBuild()
    {
        namespace aml = AdjacencyMultiList;

        int node_number = 10000;
        int mean_degree = 4;
        float connect_ratio = (float)mean_degree / (float)node_number;

        aml::Graph<int, float> test_graph(node_number);

        std::vector<int> histogram(node_number, 0);

        std::minstd_rand generator_basic(42);

        std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

        std::cout << "Build Barabasi Albert Network" << std::endl;

        Network::InitializeBANetwork(&test_graph, mean_degree, node_number);

        ShowDegreeDistribution(test_graph);

        std::cout << "\n\n" << std::string(30, '*') << "\n";

        std::cout << "Build ER Random Network" << std::endl;

        Network::InitializeERNetwork(&test_graph, mean_degree, node_number);

        ShowDegreeDistribution(test_graph);

        char a;
        std::cin >> a;
    }

    void TestClustering()
    {

        namespace aml = AdjacencyMultiList;

        int node_number = 10000;
        int mean_degree = 4;
        float connect_ratio = (float)mean_degree / (float)node_number;

        aml::Graph<int, float> test_graph(node_number);

        std::vector<int> histogram(node_number, 0);

        std::minstd_rand generator_basic(42);

        std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

        std::cout << "Build ER Random Network" << std::endl;

        Network::InitializeERNetwork(&test_graph, mean_degree, node_number);

        ShowDegreeDistribution(test_graph);

        std::cout << "\n\n" << std::string(30, '*') << "\n";

        std::vector<std::vector<aml::Vertex<int, float>*>> clusters;

        Network::FindClusters(&test_graph, clusters);

        for(int i = 0; i < std::min<int>(20, clusters.size()); i++)
        {
            std::cout << i << " : " << clusters[i].size() << std::endl;
        }
    }

    //각 스레드에서 돌아갈 함수입니다.
    void ThreadBADegreeDistribution(int thread_id,
                                    std::size_t node_number,
                                    std::size_t connection_per_step,
                                    std::size_t loop_time,
                                    std::mutex &cout_mutex,
                                    std::promise<std::vector<double>> result_promise)
    {
        cout_mutex.lock();
        std::cout << "thread " << thread_id << " started\n";
        cout_mutex.unlock();

        std::chrono::time_point<std::chrono::high_resolution_clock> start_point
            = std::chrono::high_resolution_clock::now();

        namespace AML = AdjacencyMultiList;
        AML::Graph<int, float> network(node_number);
        network.Initialize(node_number);

        std::vector<int> degree_count;
        degree_count.reserve(node_number);

        std::vector<double> degree_distribution(node_number, 0.0);

        for(std::size_t i = 0; i < loop_time; i++)
        {
            Network::ConnectBANetwork(&network, connection_per_step);
            degree_count.assign(node_number, 0);
            for(AML::Vertex<int, float>* vertex : network.vertex_list)
            {
                degree_count[vertex->GetDegree()] += 1;
            }
            for(std::size_t i = 0; i < node_number; i++)
            {
                degree_distribution[i] += (double(degree_count[i]) / node_number) / loop_time;
            }
            network.ClearEdge();
            degree_count.clear();

            cout_mutex.lock();
            std::cout << "thread " << thread_id << " loop " << i << " ended\n";
            cout_mutex.unlock();
        }
        result_promise.set_value(degree_distribution);
        network.ClearVertex();

        std::chrono::time_point<std::chrono::high_resolution_clock> end_point
            = std::chrono::high_resolution_clock::now();

        std::chrono::milliseconds deltatime
            = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);

        cout_mutex.lock();
        std::cout << "thread " << thread_id << " completed : " << deltatime.count() << "ms\n";
        cout_mutex.unlock();
    }

    /** 멀티 스레딩을 통해 BA네트워크의 Degree Distribution을 구하는 예제함수입니다.
    */
    void TestMultiThreadDegreeDistribution()
    {
        namespace AML = AdjacencyMultiList;

        const std::size_t node_number = 10000;
        const std::size_t connection_per_step = 2;
        const std::size_t thread_number = 10;
        const std::size_t loop_per_thread = 10;

        std::vector<double> degree_distribution;
        std::mutex cout_mutex;

        std::promise<std::vector<double>> promise;
        std::vector<std::future<std::vector<double>>> results;
        std::vector<std::thread> threads;
        for(std::size_t i = 0; i < thread_number; i++)
        {
            promise = std::promise<std::vector<double>>();
            results.push_back(promise.get_future());
            threads.emplace_back(ThreadBADegreeDistribution, i, node_number, connection_per_step, loop_per_thread, std::ref(cout_mutex), std::move(promise));
        }

        for(std::size_t i = 0; i < thread_number; i++)
        {
            threads[i].join();
            std::vector<double> result = results[i].get();
            if(result.size() > degree_distribution.size())
            {
                degree_distribution.resize(result.size(), 0.0);
            }
            for(std::size_t j = 0; j < result.size(); j++)
            {
                degree_distribution[j] += result[j];
            }
        }
        for(std::size_t j = 0; j < degree_distribution.size(); j++)
        {
            degree_distribution[j] /= int(thread_number);
        }

        for(int i = 0; i < 20; i++)
        {
            std::cout << std::setw(10) << degree_distribution[i] << " " << std::string(degree_distribution[i] / 0.01, '*') << "\n";
        }
    }

    void TestPrimAlgorithm(int network_size, int connection_step)
    {
        std::cout << "Creating BA Network Graph\n";
        namespace AML = AdjacencyMultiList;
        namespace MST = MinimunSpanningTree;
        AML::Graph<int, float> network(network_size);
        Network::InitializeBANetwork(&network, connection_step, network_size);

        std::random_device rande;

        std::minstd_rand generator(rande());

        std::uniform_real_distribution<float> uniform_dist(0.1f, 1.0f);

        std::cout << "Initialize Edge weight\n";
        for(AML::Vertex<int, float>* vertex : network.vertex_list)
        {
            AML::Edge<int, float>* current_edge = vertex->GetFront();
            while(current_edge != nullptr)
            {
                if(!current_edge->mark)
                {
                    current_edge->data = uniform_dist(generator);
                    current_edge->mark = true;
                }
                current_edge = current_edge->GetNext(vertex);
            }
        }

        std::cout << "Start Prim ALgorithm\n";


        BenchMark::Timer timer("PrimAlgorithm");

        std::vector<AML::Edge<int, float>*> result
            = MST::PrimAlgorithm<int, float>([](AML::Edge<int, float>* &target_edge)->float{return target_edge->data; },
                                             &network, 0);
        timer.Stop();

        std::cout << "Graph Vertex Count : " << network.vertex_list.size() << std::endl;
        std::cout << "Graph Edge Count : " << network.GetEdgeNumber() << std::endl;
        std::cout << "Spanning Tree Edge Count : " << result.size() << std::endl;
        
        std::cout << "\n" << std::string(30, '*') << "\n\n";
    }

    void TestKruskalAlgorithm(int network_size, int connection_step)
    {
        std::cout << "Creating BA Network Graph\n";
        namespace AML = AdjacencyMultiList;
        namespace MST = MinimunSpanningTree;
        AML::Graph<int, float> network(network_size);
        Network::InitializeBANetwork(&network, connection_step, network_size);

        std::random_device rande;

        std::minstd_rand generator(rande());

        std::uniform_real_distribution<float> uniform_dist(0.1f, 1.0f);

        std::cout << "Initialize Edge weight\n";
        for(AML::Vertex<int, float>* vertex : network.vertex_list)
        {
            AML::Edge<int, float>* current_edge = vertex->GetFront();
            while(current_edge != nullptr)
            {
                if(!current_edge->mark)
                {
                    current_edge->data = uniform_dist(generator);
                    current_edge->mark = true;
                }
                current_edge = current_edge->GetNext(vertex);
            }            
        }

        std::cout << "Start Prim ALgorithm\n";

        network.ResetAllEdgeMark();

        BenchMark::Timer timer("PrimAlgorithm");

        std::vector<AML::Edge<int, float>*> result
            = MST::KruskalAlgorithm<int, float>([](AML::Edge<int, float>* &target_edge)->float{return target_edge->data; },
                                             &network);
        timer.Stop();

        std::cout << "Graph Vertex Count : " << network.vertex_list.size() << std::endl;
        std::cout << "Graph Edge Count : " << network.GetEdgeNumber() << std::endl;
        std::cout << "Spanning Tree Edge Count : " << result.size() << std::endl;

        std::cout << "\n" << std::string(30, '*') << "\n\n";
    }

#pragma endregion Define functions

}



