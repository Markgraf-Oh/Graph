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

		/**
		* 최소 힙을 std::priority-queue로 구현하는 대신 레드-블랙 이진 트리인 std::set으로 구현했습니다.
		* 
		* 알고리즘 상 heap의 top에 위치하지 않은 vertex들의 거리 값을 변경해줘야 하는데,
		* priority queue는 데이터 변경에 따른 재정렬을 지원하지 않으므로 계속 새로운 vertex-distance pair를 넣어주고
		* lazy-deletion으로 오래된 pair가 top으로 올라왔을 때 제거해 줘야 합니다.
		* 하지만 일반적으로 오래된 pair 일 수록 distance가 더 길기 때문에 top까지 올라올 확률은 적습니다.
		* 따라서 vertex 수 V, edge 수 E에 대해 시간복잡도는 O(log V)이지만, O(E)의 공간을 차지하게 됩니다.
		* 
		* std::set의 이진 탐색트리를 사용할 경우 탐색은 O(log V)로 O(1)보다 느리지만,
		* 삽입과 삭제를 통한 데이터의 수정은 O(logV)로 heap과 동일합니다.
		* 그리고 무엇보다 O(V)의 공간을 유지할 수 있습니다.
		* 
		* 대부분의 경우 priority-queue를 사용하겠지만, 저는 이 라이브러리가 V => 100,000 / E => 400,000 인 네트워크 분석에 쓰이게 하고 싶었습니다.
		* 이 경우 지나치게 긴 priority_queue의 경우 메모리 allocation에서 문제가 생길 수 있어 std::set을 사용했습니다.
		* 
		* priority-queue대신 자체적으로 heap을 만들경우, top이 아닌 데이터 수정 후 heap 재정렬에 O(log V)의 시간이 들 것으로 보입니다.
		* 이 경우 O(V)의 공간만 차지하면서도 set보다는 빠르지만, 결국 시간복잡도는 동일한 O(logV)이기에 set으로 구현합니다.
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