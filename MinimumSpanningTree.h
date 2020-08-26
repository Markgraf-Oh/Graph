#pragma once

#include "AdjacencyMultiList.h"
#include "UnionFind.h"
#include <random>
#include <ctime>
#include <queue>

namespace MinimunSpanningTree
{
	namespace AML = AdjacencyMultiList;

	template<typename VT, typename ET>
	std::vector<AML::Edge<VT, ET>*> PrimAlgorithm(float (*GetEdgeWeight)(AML::Edge<VT, ET>* &), AML::Graph<VT, ET> *target_graph, int start_index = -1)
	{
		std::vector<AML::Edge<VT, ET>*> result_edges;
		if(target_graph->vertex_list.size() < 2) return result_edges;
		result_edges.reserve(target_graph->vertex_list.size() - 1);
		if(start_index < 0)
		{
			srand(time(NULL));
			start_index = rand() % target_graph->vertex_list.size();
		}

		target_graph->ResetVertexIndex();

		std::vector<bool> visited_mark(target_graph->vertex_list.size(), false);

		auto comp = [&GetEdgeWeight](AML::Edge<VT, ET>* &back, AML::Edge<VT, ET>* &front)
			-> bool { return GetEdgeWeight(back) > GetEdgeWeight(front); };

		std::priority_queue<AML::Edge<VT, ET>*, std::vector<AML::Edge<VT, ET>*>, decltype(comp)> shortest_edge(comp);

		visited_mark[start_index] = true;
		{
			std::vector<AML::Edge<VT, ET>*> edges = target_graph->vertex_list[start_index]->GetEdges();
			while(!edges.empty())
			{
				shortest_edge.push(edges.back());
				edges.pop_back();
			}
		}

		while(!shortest_edge.empty())
		{
			AML::Edge<VT, ET>* current_edge = shortest_edge.top();
			shortest_edge.pop();
			AML::Vertex<VT, ET>* target_vertex = nullptr;
			for(AML::Vertex<VT, ET>* vertex : current_edge->GetVertex())
			{
				if(!visited_mark[vertex->index])
				{
					target_vertex = vertex;
					break;
				}
			}
			if(target_vertex == nullptr) continue;

			AML::Edge<VT, ET>* next_edge = target_vertex->GetFront();
			while(next_edge != nullptr)
			{
				shortest_edge.push(next_edge);
				next_edge = next_edge->GetNext(target_vertex);
			}
			visited_mark[target_vertex->index] = true;
			result_edges.push_back(current_edge);
		}

		return result_edges;
	}

	template<typename VT, typename ET>
	std::vector<AML::Edge<VT, ET>*> KruskalAlgorithm(float (*GetEdgeWeight)(AML::Edge<VT, ET>* &), AML::Graph<VT, ET> *target_graph)
	{
		std::vector<AML::Edge<VT, ET>*> result_edges;
		const int network_size = target_graph->vertex_list.size();
		if(network_size < 2) return result_edges;
		result_edges.reserve(network_size - 1);

		const int edge_queue_max_size = 50000;
		const int number_of_queue = (target_graph->GetEdgeNumber() / edge_queue_max_size) + 1;

		target_graph->ResetVertexIndex();

		UnionFindTree union_find_tree(network_size);

		auto comp = [&GetEdgeWeight](AML::Edge<VT, ET>* &back, AML::Edge<VT, ET>* &front)
			-> bool { return GetEdgeWeight(back) > GetEdgeWeight(front); };

		std::vector<std::priority_queue<AML::Edge<VT, ET>*, std::vector<AML::Edge<VT, ET>*>, decltype(comp)>> edge_queues;

		for(int i = 0; i < number_of_queue; i++)
		{
			edge_queues.emplace_back(comp);
		}

		target_graph->ResetAllEdgeMark();
		int edge_counter = 0;
		for(AML::Vertex<VT, ET>* vertex : target_graph->vertex_list)
		{
			AML::Edge<VT, ET>* current_edge = vertex->GetFront();
			while(current_edge != nullptr)
			{
				if(!current_edge->mark)
				{
					current_edge->mark = true;
					edge_queues[edge_counter / edge_queue_max_size].push(current_edge);
					++edge_counter;
				}
				current_edge = current_edge->GetNext(vertex);
			}
		}

		while(result_edges.size() < network_size - 1)
		{
			float min_weight = std::numeric_limits<float>::max();
			int target_index = -1;
			for(int i = 0; i < number_of_queue; i++)
			{
				if(!edge_queues[i].empty())
				{
					AML::Edge<VT, ET>* temp = edge_queues[i].top();
					float weight = GetEdgeWeight(temp);

					if(weight < min_weight)
					{
						min_weight = weight;
						target_index = i;
					}
				}
			}

			if(target_index == -1) break;

			AML::Edge<VT, ET>* target_edge = edge_queues[target_index].top();
			edge_queues[target_index].pop();

			//union-find algorithm »ç¿ë.
						
			if(union_find_tree.Union(target_edge->GetVertex(0)->index, target_edge->GetVertex(1)->index))
				result_edges.push_back(target_edge);
		}
		return result_edges;
	}
}
