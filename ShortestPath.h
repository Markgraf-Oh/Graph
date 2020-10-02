#pragma once

#include "AdjacencyMultiList.h"
#include <queue>
#include <set>

namespace ShortestPath
{
	namespace AML = AdjacencyMultiList;
	
	template<typename VT, typename ET>
	std::vector<AML::Vertex<VT,ET>*> DijkstraAlgorithmPath(AML::Graph<VT, ET> *target_graph,
														 float (*GetEdgeWeight)(AML::Edge<VT, ET>* &),
														 AML::Vertex<VT,ET>* start_vertex,
														 AML::Vertex<VT,ET>* end_vertex)
	{
		std::vector<AML::Vertex<VT, ET>*> result;
		target_graph->ResetVertexIndex();
		if((target_graph->vertex_list[start_vertex->index] != start_vertex) || (target_graph->vertex_list[end_vertex->index] != end_vertex))
			return result;//if given vertex isn't in graph

		const int network_size = target_graph->vertex_list.size();

		//vector to save distance from start_node if distance is 0.0f, it means that the vertex is already visited or start node.
		std::vector<float> distances(network_size, std::numeric_limits<float>::max());
		distances[start_vertex->index] = 0.0f;
		
		//vector to save index of previous node in shortest path
		std::vector<int> prev_index(network_size, -1);
		prev_index[start_vertex->index] = start_vertex->index;

		//distance compare function
		auto comp = [&distances](AML::Vertex<VT, ET>* front, AML::Vertex<VT, ET>* back)
			-> bool
		{
			float bd = distances[back->index];
			float fd = distances[front->index];
			if(bd != fd) return bd > fd;
			return front->index < back->index;
		};

		std::set<AML::Vertex<VT, ET>*, decltype(comp)> min_tree(comp);

		for(AML::Vertex<VT, ET>* vertex : target_graph->vertex_list)
		{
			min_tree.insert(vertex);
		}

		while(!min_tree.empty())
		{
			typename std::set<AML::Vertex<VT, ET>*, decltype(comp)>::iterator iter = min_tree.begin();
			AML::Vertex<VT, ET>* current_vertex = *iter;
			min_tree.erase(iter);
			if(current_vertex == end_vertex) break;
			AML::Edge<VT, ET>* current_edge = current_vertex->GetFront();
			while(current_edge != nullptr)
			{
				AML::Vertex<VT, ET>* opposite = current_edge->GetOpposite(current_vertex);
				if(distances[opposite->index] > 0.0f)
				{
					float alternative = distances[current_vertex->index] + GetEdgeWeight(current_edge);
					if(alternative < distances[opposite->index])
					{						
						prev_index[opposite->index] = current_vertex->index;
						min_tree.erase(min_tree.find(opposite));
						distances[opposite->index] = alternative;
						min_tree.insert(opposite);
					}
				}
				current_edge = current_edge->GetNext(current_vertex);
			}
			distances[current_vertex->index] = 0.0f;
		}

		int current_index = prev_index[end_vertex->index];
		
		while(current_index != start_vertex->index)
		{
			result.insert(result.begin(), target_graph->vertex_list[current_index]);
			current_index = prev_index[current_index];
		}
		return result;
		
		/**
		* using std::set as red-black binary search tree
		* because it's time complexity of (search, insert, delete) is O(logV) and using O(V) of space
		* 
		* if we use priority-queue with lazy-deletion, it will take O(logV) of time complexity(but faster than set)
		* but it will need O(E) of space (as the older data tend to have bigger distance, thus they usually doesn't locate on the top of heap.)
		*	
		* in most case, people will prefer later one,
		* but I wanted to use this library for the Networks made of more than 100,000 vertices and 400,000 edges.
		* in such large case, there could be memory allocation issues
		* 
		* if we use custom heap, and rearrange heap after change non-top value, it took O(log V) time with O(V) space
		* this will faster than std::set, but still have same time-complexity. so I choosed std::set.
		*/
	}

	template<typename VT, typename ET>
	std::vector<AML::Edge<VT, ET>*> DijkstraAlgorithmTree(AML::Graph<VT, ET> *target_graph,
															float (*GetEdgeWeight)(AML::Edge<VT, ET>* &),
															AML::Vertex<VT, ET>* start_vertex)
	{
		std::vector<AML::Edge<VT, ET>*> result;
		target_graph->ResetVertexIndex();
		if((target_graph->vertex_list[start_vertex->index] != start_vertex))
			return result;//if given vertex isn't in graph

		const int network_size = target_graph->vertex_list.size();
		result.assign(network_size, nullptr);

		//vector to save distance from start_node if distance is 0.0f, it means that the vertex is already visited or start node.
		std::vector<float> distances(network_size, std::numeric_limits<float>::max());
		distances[start_vertex->index] = 0.0f;

		//vector to save index of previous node in shortest path
		std::vector<AML::Edge<VT, ET>*> prev_path(network_size, nullptr);

		//distance compare function
		auto comp = [&distances](AML::Vertex<VT, ET>* front, AML::Vertex<VT, ET>* back)
			-> bool
		{
			float bd = distances[back->index];
			float fd = distances[front->index];
			if(bd != fd) return bd > fd;
			return front->index < back->index;
		};

		std::set<AML::Vertex<VT, ET>*, decltype(comp)> min_tree(comp);

		for(AML::Vertex<VT, ET>* vertex : target_graph->vertex_list)
		{
			min_tree.insert(vertex);
		}

		while(!min_tree.empty())
		{
			typename std::set<AML::Vertex<VT, ET>*, decltype(comp)>::iterator iter = min_tree.begin();
			AML::Vertex<VT, ET>* current_vertex = *iter;
			min_tree.erase(iter);
			if(distances[current_vertex->index] == std::numeric_limits<float>::max()) continue;
			AML::Edge<VT, ET>* current_edge = current_vertex->GetFront();
			while(current_edge != nullptr)
			{
				AML::Vertex<VT, ET>* opposite = current_edge->GetOpposite(current_vertex);
				if(distances[opposite->index] > 0.0f)
				{
					float alternative = distances[current_vertex->index] + GetEdgeWeight(current_edge);
					if(alternative < distances[opposite->index])
					{
						result[opposite->index] = current_edge;
						min_tree.erase(min_tree.find(opposite));
						distances[opposite->index] = alternative;
						min_tree.insert(opposite);
					}
				}
				current_edge = current_edge->GetNext(current_vertex);
			}
			distances[current_vertex->index] = 0.0f;
		}

		return result;
	}

	template<typename VT, typename ET>
	std::vector<AML::Vertex<VT, ET>*> AStarAlgorithm(AML::Graph<VT, ET> *target_graph,
													 float (*GetEdgeWeight)(AML::Edge<VT, ET>* &),
													 float (*heuristic)(AML::Vertex<VT, ET>* &, AML::Vertex<VT, ET>* &),
													 AML::Vertex<VT, ET>* start_vertex,
													 AML::Vertex<VT, ET>* end_vertex)
	{
		std::vector<AML::Vertex<VT, ET>*> result;
		target_graph->ResetVertexIndex();
		if((target_graph->vertex_list[start_vertex->index] != start_vertex) || (target_graph->vertex_list[end_vertex->index] != end_vertex))
			return result;//if given vertex isn't in graph

		const int network_size = target_graph->vertex_list.size();

		//vector to save distance from start_node if distance is 0.0f, it means that the vertex is already visited or start node.
		std::vector<float> evaluations(network_size, std::numeric_limits<float>::max());
		evaluations[start_vertex->index] = heuristic(start_vertex, end_vertex);

		//vector to save index of previous node in shortest path
		std::vector<int> prev_index(network_size, -1);
		prev_index[start_vertex->index] = start_vertex->index;

		//distance compare function
		auto comp = [&evaluations](AML::Vertex<VT, ET>* front, AML::Vertex<VT, ET>* back)
			-> bool
		{
			float bd = evaluations[back->index];
			float fd = evaluations[front->index];
			if(bd != fd) return bd > fd;
			return front->index < back->index;
		};

		std::set<AML::Vertex<VT, ET>*, decltype(comp)> min_tree(comp);

		for(AML::Vertex<VT, ET>* vertex : target_graph->vertex_list)
		{
			min_tree.insert(vertex);
		}

		while(!min_tree.empty())
		{
			typename std::set<AML::Vertex<VT, ET>*, decltype(comp)>::iterator iter = min_tree.begin();
			AML::Vertex<VT, ET>* current_vertex = *iter;
			min_tree.erase(iter);
			if(current_vertex == end_vertex) break;
			AML::Edge<VT, ET>* current_edge = current_vertex->GetFront();
			while(current_edge != nullptr)
			{
				AML::Vertex<VT, ET>* opposite = current_edge->GetOpposite(current_vertex);
				if(evaluations[opposite->index] >= 0.0f)
				{
					float alternative = evaluations[current_vertex->index] + GetEdgeWeight(current_edge) - heuristic(current_vertex, end_vertex) + heuristic(opposite, end_vertex);
					if(alternative < evaluations[opposite->index])
					{
						prev_index[opposite->index] = current_vertex->index;
						min_tree.erase(min_tree.find(opposite));
						evaluations[opposite->index] = alternative;
						min_tree.insert(opposite);
					}
				}
				current_edge = current_edge->GetNext(current_vertex);
			}
			evaluations[current_vertex->index] = -2.0f;
		}

		int current_index = prev_index[end_vertex->index];

		if(current_index == -1) return result;

		while(current_index != start_vertex->index)
		{
			result.insert(result.begin(), target_graph->vertex_list[current_index]);
			current_index = prev_index[current_index];
		}
		return result;
	}
}