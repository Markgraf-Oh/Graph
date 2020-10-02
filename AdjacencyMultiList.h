#pragma once

#include <vector>
#include <stack>
#include <array>
#include <string>
#include <iostream>

#define LINE_INFO (std::string(__FUNCTION__) + " in " + std::string(__FILE__) +"(line : " + std::to_string(__LINE__)+ ")")

namespace AdjacencyMultiList
{
	template<typename VT = int, typename ET = float>
	class Graph;

	template<typename VT = int, typename ET = float>
	class Vertex;

	template<typename VT = int, typename ET = float>
	class Edge;

	
	class GraphException : virtual public std::exception
	{
	protected:
		std::string base_message = "Graph Error";
		std::string location = "?";
		std::string aditional_message = "";
	public:
		GraphException(){}
		GraphException(std::string location) : location(location){}

		GraphException(std::string location, std::string aditional_message):
			location(location),
			aditional_message(aditional_message){}

		const char* what() const noexcept override
		{
			std::string output = base_message + " at (" + location +")";
			if(!aditional_message.empty())
			{
				output += aditional_message;
			}
			return output.c_str();
		}
	};

	void PrintError(std::string line_info, std::string errmessage)
	{
		std::cerr << line_info << " : " << errmessage << std::endl;
	}


	/**	Vertex Class for Adjacency Multi List Graph
	*@tparam	VT	type of data witch will be stored inside vertex
	*@tparam	ET	type of data witch will be stored inside edge
	*/
	template<typename VT = int, typename ET = float>
	class Vertex
	{
		friend Graph<VT, ET>;
		friend Edge<VT, ET>;
		//생성자, 소멸자
	public:
		Vertex();

		/**@param	init_data	initial data of this vertex
		*/
		Vertex(VT init_data);

		virtual ~Vertex();

		//변수
	public:
		VT data;

		int index = 0;

	protected:
		//Doubly-Multi-Linked-List's first element
		Edge<VT, ET>* front = nullptr;
		
		int degree = 0;

		//함수
	public:
		/**	이 vertex의 linked list를 탐색하여 degree를 구하여 저장하고 return합니다.
		*@return	degree of this vertex
		*@note		그래프의 모든 vertex에 대해 이 함수를 실행해서 mean degree를 구하는 것은 리소스 낭비입니다.
					그래프의 GetEdgeNumber를 사용하세요.
		*@see		Graph<VT,ET>::GetEdgeNumber(), Vertex<VT,ET>::GetDegree()
		*/
		int CountDegree();

		inline int GetDegree() const{ return degree; }

		Edge<VT, ET>* GetFront() const;

		Edge<VT, ET>* GetBack();

		std::vector<Vertex<VT, ET>*> GetOpposites();

		std::vector<Edge<VT, ET>*> GetEdges();

		void ResetEdgeMark();

	protected:
		/**	internal fuction to quickly delete all edges in graph.
		*/
		void _DeleteEdge();
	};

	/**	Edge Class for Adjacency Multi List Graph
	*@tparam	VT	type of data witch will be stored inside vertex
	*@tparam	ET	type of data witch will be stored inside edge
	*/
	template<typename VT = int, typename ET = float>
	class Edge
	{

		friend Graph<VT, ET>;
		friend Vertex<VT, ET>;
		//생성자, 소멸자
	public:
		Edge()
		{
			data = ET();
		};

		/**@param	init_data	initial data of this edge
		*/
		Edge(ET init_data) : data(init_data){};

		Edge(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1) :
			vertex{{vertex0, vertex1}}
		{
			if(typeid(ET) == typeid(float))
			{
				data = 1.0;
			}
			else data = ET();
		};

		Edge(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1, ET init_data) :
			vertex{{vertex0, vertex1}},
			data(init_data){};

		//변수
	public:
		ET data;

		mutable bool mark = false;

	protected:
		std::array<Vertex<VT, ET>*, 2> vertex{{nullptr, nullptr}};
		std::array<Edge<VT, ET>*, 2> next{{nullptr, nullptr}};
		std::array<Edge<VT, ET>*, 2> before{{nullptr, nullptr}};

		//함수
	public:
		Edge<VT, ET>* GetNext(Vertex<VT, ET>* target_vertex) const;

		Edge<VT, ET>* GetBefore(Vertex<VT, ET>* target_vertex) const;

		/**	주어진 vertex의 array 상에서의 index를 찾습니다.
		*@return	해당 vertex를 찾으면 0 or 1
		*			못찾으면 -1
		*/
		int FindIndex(Vertex<VT, ET>* target_vertex) const;

		Vertex<VT, ET>* GetOpposite(Vertex<VT, ET>* vertex) const;

		std::array<Vertex<VT, ET>*, 2> GetVertex(){ return vertex; }

		Vertex<VT, ET>* GetVertex(int index)
		{ 
			if(index < 0 || 1 < index) return nullptr;
			return vertex[index];
		}

	protected:
		void append(Vertex<VT, ET>* target, Edge<VT, ET>* next_edge);
	};


	/**	Adjacency Multi List Graph
	*@tparam	VT	type of data witch will be stored inside vertex
	*@tparam	ET	type of data witch will be stored inside edge
	*/
	template<typename VT = int, typename ET = float>
	class Graph
	{
		//생성자 소멸자
	public:
		Graph();

		Graph(int init_vertex_number);

		virtual ~Graph();

		//변수
	public:
		std::vector<Vertex<VT, ET>*> vertex_list;

	protected:
		int target_vertex_number = 0;

		int current_edge_number = 0;

		//함수
	public:
		//return number of vertices in this graph
		int GetTargetVertexNumber(){ return target_vertex_number; }; //vertex의 복수형이 vertices라니 진짜 영어의 규칙성은...

		int GetEdgeNumber(){ return current_edge_number; };
		//
		void Initialize(int init_vertex_number, VT init_data);

		void Initialize(int init_vertex_number);

		void Initialize();

		bool IsConnected(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2) const;

		bool IsConnected(int i, int j) const;

		std::vector<Edge<VT, ET>*> FindConnections(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);
		
		void Connect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		void Connect(int i, int j);
		
		void Disconnect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		Edge<VT, ET>* PopEdge(Edge<VT, ET>* target_edge, Vertex<VT, ET>* return_reference_vertex);
		
		void PopVertex(Vertex<VT, ET>* &target);

		void PopVertex(int target_index);

		typename std::vector<Vertex<VT, ET>*>::iterator PopVertex(typename std::vector<Vertex<VT, ET>*>::iterator taret_iterator);
		
		void ClearVertex();
		
		void ClearEdge();

		void ResetVertexIndex();

		void ResetAllEdgeMark();
	};

	template<typename VT, typename ET>
	inline Vertex<VT, ET>::Vertex()
	{
		data = VT();
	}

	template<typename VT, typename ET>
	inline Vertex<VT, ET>::Vertex(VT init_data)
	{
		data = init_data;
	}

	template<typename VT, typename ET>
	inline Vertex<VT, ET>::~Vertex()
	{
		if(front != nullptr)
		{
			_DeleteEdge();
		}
	}

	template<typename VT, typename ET>
	inline int Vertex<VT, ET>::CountDegree()
	{
		int degree_count = 0;
		Edge<VT, ET>* current = front;
		while(current != nullptr)
		{
			++degree_count;
			current = current->GetNext(this);
		}
		degree = degree_count;
		return degree_count;
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Vertex<VT, ET>::GetFront() const
	{
		return front;
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Vertex<VT, ET>::GetBack()
	{
		if(front == nullptr) return nullptr;
		Edge<VT, ET>* back = front;
		while(back->GetNext(this) != nullptr)
		{
			back = back->GetNext(this);
		}
		return back;
	}

	template<typename VT, typename ET>
	inline std::vector<Vertex<VT, ET>*> Vertex<VT, ET>::GetOpposites()
	{
		std::vector<Vertex<VT, ET>*> opposites;
		Edge<VT, ET>* current_edge = front;
		while(current_edge != nullptr)
		{
			opposites.push_back(current_edge->GetOpposite(this));
			current_edge = current_edge->GetNext(this);
		}
		return opposites;
	}

	template<typename VT, typename ET>
	inline std::vector<Edge<VT, ET>*> Vertex<VT, ET>::GetEdges()
	{
		std::vector<Edge<VT, ET>*> edges;
		Edge<VT, ET>* current_edge = front;
		while(current_edge != nullptr)
		{
			edges.push_back(current_edge);
			current_edge = current_edge->GetNext(this);
		}
		return edges;
	}

	template<typename VT, typename ET>
	inline void Vertex<VT, ET>::ResetEdgeMark()
	{
		Edge<VT, ET>* current_edge = front;
		while(current_edge != nullptr)
		{
			current_edge->mark = false;
			current_edge = current_edge->GetNext(this);
		}
	}

	template<typename VT, typename ET>
	inline void Vertex<VT, ET>::_DeleteEdge()
	{
		if(front == nullptr) return;
		Edge<VT, ET>* current = front;
		std::stack<Edge<VT, ET>*> edge_stack;
		while(current != nullptr)
		{
			int selector = current->FindIndex(this);
			current->mark = (current->vertex[!selector] == nullptr) ? true : false;
			current->vertex[selector] == nullptr;
			edge_stack.push(current);
			current = std::exchange(current->next[selector], nullptr);
		}

		while(!edge_stack.empty())
		{
			current = edge_stack.top();
			if(current->mark)
			{
				delete current;
				current = nullptr;
			}
			edge_stack.pop();
		}

		front = nullptr;
		degree = 0;
	}


	template<typename VT, typename ET>
	inline Edge<VT, ET>* Edge<VT, ET>::GetNext(Vertex<VT, ET>* target_vertex) const
	{
		if(target_vertex == nullptr) return nullptr;

		Edge<VT, ET>* findnext = nullptr;
		if(vertex[0] == target_vertex)
		{
			findnext = next[0];
		}
		else if(vertex[1] == target_vertex)
		{
			findnext = next[1];
		}

		return findnext;
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Edge<VT, ET>::GetBefore(Vertex<VT, ET>* target_vertex) const
	{
		if(target_vertex == nullptr) return nullptr;

		Edge<VT, ET>* findnext = nullptr;
		if(vertex[0] == target_vertex)
		{
			findnext = before[0];
		}
		else if(vertex[1] == target_vertex)
		{
			findnext = before[1];
		}
		return findnext;
	}

	template<typename VT, typename ET>
	inline int Edge<VT, ET>::FindIndex(Vertex<VT, ET>* target_vertex) const
	{
		int selector;
		if(vertex[0] == target_vertex)
		{
			selector = 0;
		}
		else if(vertex[1] == target_vertex)
		{
			selector = 1;
		}
		else selector = -1;

		return selector;
	}

	template<typename VT, typename ET>
	inline Vertex<VT, ET>* Edge<VT, ET>::GetOpposite(Vertex<VT, ET>* target_vertex) const
	{
		if(target_vertex == nullptr) return nullptr;
		if(vertex[0] == target_vertex) return vertex[1];
		else if(vertex[1] == target_vertex) return vertex[0];
		return nullptr; 
	}

	template<typename VT, typename ET>
	inline void Edge<VT, ET>::append(Vertex<VT, ET>* target, Edge<VT, ET>* next_edge)
	{
		if(target == nullptr) throw std::invalid_argument("target = nullptr");
		if(next_edge == nullptr) throw std::invalid_argument("next_edge = nullptr");
		int next_selector = next_edge->FindIndex(target);
		if(next_selector < 0) throw std::invalid_argument("edge and vertex is not related");

		int selector = FindIndex(target);
		if(selector < 0) throw GraphException(LINE_INFO, "can't find vertex");
		
		Edge<VT, ET>* previous_next_edge = this->next[selector];
		if(previous_next_edge != nullptr)
		{
			previous_next_edge->before[previous_next_edge->FindIndex(target)] = next_edge;
			next_edge->next[next_selector] = previous_next_edge;
		}
		this->next[selector] = next_edge;
		next_edge->before[next_selector] = this;

	}

	template<typename VT, typename ET>
	inline Graph<VT, ET>::Graph()
	{
	}

	template<typename VT, typename ET>
	inline Graph<VT, ET>::Graph(int init_vertex_number)
		: target_vertex_number(init_vertex_number)
	{
		vertex_list.reserve(init_vertex_number);
	}

	template<typename VT, typename ET>
	inline Graph<VT, ET>::~Graph()
	{

	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Initialize(int init_vertex_number, VT init_data)
	{
		if(init_vertex_number < 0) return;
		ClearVertex();
		target_vertex_number = init_vertex_number;
		vertex_list.reserve(init_vertex_number);
		for(int i = 0; i < target_vertex_number; i++)
		{
			vertex_list.push_back(new Vertex<VT, ET>(init_data));
		}
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Initialize(int init_vertex_number)
	{
		if(init_vertex_number < 0) return;
		ClearVertex();
		target_vertex_number = init_vertex_number;
		vertex_list.reserve(init_vertex_number);
		for(int i = 0; i < target_vertex_number; i++)
		{
			vertex_list.push_back(new Vertex<VT, ET>());
		}
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Initialize()
	{
		ClearVertex();
		for(int i = 0; i < target_vertex_number; i++)
		{
			vertex_list.push_back(new Vertex<VT, ET>());
		}
	}

	template<typename VT, typename ET>
	inline bool Graph<VT, ET>::IsConnected(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1) const
	{
		Edge<VT, ET>* edge_list = vertex0->GetFront();
		while(edge_list != nullptr)
		{
			if(edge_list->vertex[0] == vertex0 && edge_list->vertex[1] == vertex1)
				return true;
			else if(edge_list->vertex[0] == vertex1 && edge_list->vertex[1] == vertex0)
				return true;

			edge_list = edge_list->GetNext(vertex0);
		}

		return false;
	}

	template<typename VT, typename ET>
	inline bool Graph<VT, ET>::IsConnected(int i, int j) const
	{
		if((i >= vertex_list.size() || j >= vertex_list.size()) || i==j)
			return false;

		return this->IsConnected(vertex_list[i], vertex_list[j]);
	}

	template<typename VT, typename ET>
	inline std::vector<Edge<VT, ET>*> Graph<VT, ET>::FindConnections(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		std::vector<Edge<VT, ET>*> founds;
		Edge<VT, ET>* current_edge = vertex1.front;
		while(current_edge != nullptr)
		{
			if(current_edge->vertex[0] == vertex1 && current_edge->vertex[1] == vertex2)
				founds.push_back(current_edge);
			else if(current_edge->vertex[0] == vertex2 && current_edge->vertex[1] == vertex1)
				founds.push_back(current_edge);

			current_edge = current_edge->GetNext(vertex1);
		}

		return founds;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Connect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		if(vertex1 == nullptr || vertex2 == nullptr) throw std::invalid_argument("vertex");

		Edge<VT, ET>* back1 = vertex1->GetBack();
		Edge<VT, ET>* back2 = vertex2->GetBack();
		
		Edge<VT, ET>* connector = new Edge<VT, ET>(vertex1, vertex2);
		if(nullptr == back1)
		{
			vertex1->front = connector;
		}
		else
		{
			back1->append(vertex1, connector);
		}

		if(nullptr == back2)
		{
			vertex2->front = connector;
		}
		else
		{
			back2->append(vertex2, connector);
		}
		vertex1->degree++;
		vertex2->degree++;

		++current_edge_number;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Connect(int i, int j)
	{
		if(i >= vertex_list.size() || j >= vertex_list.size())
			throw GraphException(LINE_INFO, "given index is out of range");

		if(i == j)
			throw GraphException(LINE_INFO, "same index");


		this->Connect(vertex_list[i], vertex_list[j]);
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Disconnect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		std::vector<Edge<VT,ET>*> connections = FindConnections(vertex1, vertex2);
		for(Edge<VT, ET>* connection : connections)
		{
			PopEdge(connection);
		}
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Graph<VT, ET>::PopEdge(Edge<VT, ET>* target_edge, Vertex<VT, ET>* return_reference_vertex)
	{
		if(target_edge == nullptr)
		{
			std::cerr << LINE_INFO << " : target_edge == nullptr" << std::endl;
			return nullptr;
		}

		if(target_edge->FindIndex(return_reference_vertex) < 0)
		{
			std::cerr << LINE_INFO << " : edge and vertex is not related" << std::endl;
			return nullptr;
		}

		Edge<VT, ET>* return_edge = target_edge->GetNext(return_reference_vertex);

		for(int i = 0; i < 2; i++)
		{
			Vertex<VT, ET>* target_vertex = target_edge->vertex[i];
			Edge<VT, ET>* before_edge = target_edge->before[i];
			Edge<VT, ET>* next_edge = target_edge->next[i];
			if(before_edge == nullptr)
			{
				target_vertex->front = next_edge;
			}
			else
			{
				int selector = before_edge->FindIndex(target_vertex);
				before_edge->next[selector] = next_edge;
			}

			if(next_edge != nullptr)
			{
				int selector = next_edge->FindIndex(target_vertex);
				next_edge->before[selector] = before_edge;
			}

			target_vertex->degree--;
		}

		delete target_edge;
		target_edge = nullptr;
		--current_edge_number;

		return return_edge;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::PopVertex(Vertex<VT, ET>* &target)
	{
		typename std::vector<Vertex<VT, ET>*>::iterator target_iter = std::find(vertex_list.begin(), vertex_list.end(), target);

		if(target_iter == vertex_list.end())
			throw std::invalid_argument("target vertex is not included in this graph");

		while(target->front != nullptr)
		{
			PopEdge(target->GetFront(), target);
		}

		delete target;
		target = nullptr;

		vertex_list.erase(target_iter);
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::PopVertex(int target_index)
	{
		if(target_index < 0 || target_index > vertex_list.size()) throw std::out_of_range("given index is out of range");
				
		PopVertex(vertex_list[target_index]);
	}

	template<typename VT, typename ET>
	inline typename std::vector<Vertex<VT, ET>*>::iterator Graph<VT, ET>::PopVertex(typename std::vector<Vertex<VT, ET>*>::iterator target_iterator)
	{
		if( ! ((vertex_list.begin() <= target_iterator) && (target_iterator < vertex_list.end())) )
		{
			PrintError(LINE_INFO, "invalid iterator");
			return vertex_list.end();
		}

		Vertex<VT, ET>* target_vertex = *target_iterator;

		if(target_vertex == nullptr)
		{
			return vertex_list.erase(target_iterator);
		}

		while(target_vertex->front != nullptr)
		{
			PopEdge(target_vertex->front, target_vertex);
		}
		delete target_vertex;
		target_vertex = nullptr;
		*target_iterator = nullptr;

		return vertex_list.erase(target_iterator);
	}


	template<typename VT, typename ET>
	inline void Graph<VT, ET>::ClearVertex()
	{
		for(Vertex<VT, ET>* vertex : vertex_list)
		{
			vertex->_DeleteEdge();
			delete vertex;
		}
		vertex_list.clear();
		current_edge_number = 0;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::ClearEdge()
	{
		for(Vertex<VT, ET>* vertex : vertex_list)
		{
			vertex->_DeleteEdge();
		}
		current_edge_number = 0;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::ResetVertexIndex()
	{
		for(int i = 0; i < vertex_list.size(); i++)
		{
			vertex_list[i]->index = i;
		}
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::ResetAllEdgeMark()
	{
		for(Vertex<VT, ET>* vertex : vertex_list)
		{
			vertex->ResetEdgeMark();
		}
	}

}

