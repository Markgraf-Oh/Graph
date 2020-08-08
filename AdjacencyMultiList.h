#pragma once

#include <vector>
#include <stack>
#include <array>
#include <string>

#define LINE_INFO (std::string(__FUNCTION__) + " in " + std::string(__FILE__) +"(line : " + std::to_string(__LINE__)+ ")")

namespace AdjacencyMultiList
{
	template<typename VT = int, typename ET = float>
	class Graph;

	template<typename VT = int, typename ET = float>
	class Vertex;

	template<typename VT = int, typename ET = float>
	class Edge;

	template<typename VT = int, typename ET = float>
	struct Link
	{
		Vertex<VT, ET>* vertex = nullptr;
		Edge<VT, ET>* next = nullptr;

		Link(){};
		Link(Vertex<VT, ET>* init_vertex) : vertex(init_vertex) {};
	};

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

	protected:
		Edge<VT, ET>* front = nullptr;

		//함수
	public:
		/**	이 vertex의 linked list를 탐색하여 degree를 구합니다.
		*@return	degree
		*@note		그래프의 모든 vertex에 대해 이 함수를 실행해서 mean degree를 구하는 것은 리소스 낭비입니다.
					그래프의 GetEdgeNumber를 사용하세요.
		*@see		Graph<VT,ET>::GetEdgeNumber()
		*/
		int GetDegree();

		void DisConnectAll();

		Edge<VT, ET>* GetFront();

		Edge<VT, ET>* GetBack();

	protected:
		void _DeleteEdge();//이건 graph쪽으로
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
			
		};

		/**@param	init_data	initial data of this edge
		*/
		Edge(ET init_data) : data(init_data){};

		Edge(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1) :
			links({Link<VT, ET>(vertex0), Link<VT, ET>(vertex1)})
		{
			if(typeid(ET) == typeid(float))
			{
				data = 1.0;
			}
			else data = ET();
		};

		Edge(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1, ET init_data) :
			links({Link<VT, ET>(vertex0), Link<VT, ET>(vertex1)}),
			data(init_data){};

		//변수
	public:
		ET data;

		bool mark = false;

	protected:
		std::array<Link<VT, ET>, 2> links;

		//함수
	public:
		Edge<VT, ET>* Next(Vertex<VT, ET>* target_vertex);

		int FindIndex(Vertex<VT, ET>* target_vertex);

	protected:
		void append(Vertex<VT, ET>* target, Edge<VT, ET>* next_edge);
	};

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
		//
		void Initialize(int init_vertex_number);

		static bool IsConnected(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		std::vector<Edge<VT, ET>*> FindConnections(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);
		//
		static void Connect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);
		//
		static void Disconnect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		static void PopEdge(Edge<VT, ET>* target_edge);
		
		Vertex<VT, ET>* PopVertex(Vertex<VT, ET>* target);
		
		void ClearVertex();
		//
		void ClearEdge();

	
	};

	template<typename VT, typename ET>
	inline Vertex<VT, ET>::Vertex()
	{
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
			DisConnectAll();
		}
	}

	template<typename VT, typename ET>
	inline int Vertex<VT, ET>::GetDegree()
	{
		int degree = 0;
		Edge<VT, ET>* current = front;
		while(current != nullptr)
		{
			++degree;
			current = current->Next(this);
		}
		return degree;
	}

	template<typename VT, typename ET>
	inline void Vertex<VT, ET>::DisConnectAll()
	{
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Vertex<VT, ET>::GetFront()
	{
		return front;
	}

	template<typename VT, typename ET>
	inline Edge<VT, ET>* Vertex<VT, ET>::GetBack()
	{
		if(front == nullptr) return nullptr;
		Edge<VT, ET>* back = front;
		while(back->Next(this) != nullptr)
		{
			back = back->Next(this);
		}
		return back;
	}

	template<typename VT, typename ET>
	inline void Vertex<VT, ET>::_DeleteEdge()
	{
		Edge<VT, ET>* current = front;
		std::stack<Edge<VT, ET>*> edge_stack;
		while(current != nullptr)
		{
			int selector = current->FindIndex(this);
			current->mark = (current->links[!selector].vertex == nullptr) ? true : false;
			current->links[selector].vertex == nullptr;
			edge_stack.push(current);
			current = std::exchange(current->links[selector].next, nullptr);
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

		this->front = nullptr;
	}


	template<typename VT, typename ET>
	inline Edge<VT, ET>* Edge<VT, ET>::Next(Vertex<VT, ET>* target_vertex)
	{
		if(target_vertex == nullptr) return nullptr;

		Edge<VT, ET>* next = nullptr;
		if(links[0].vertex == target_vertex)
		{
			next = links[0].next;
		}
		else if(links[1].vertex == target_vertex)
		{
			next = links[1].next;
		}

		return next;
	}

	template<typename VT, typename ET>
	inline int Edge<VT, ET>::FindIndex(Vertex<VT, ET>* target_vertex)
	{
		int selector;
		if(links[0].vertex == target_vertex)
		{
			selector = 0;
		}
		else if(links[1].vertex == target_vertex)
		{
			selector = 1;
		}
		else selector = -1;

		return selector;
	}

	template<typename VT, typename ET>
	inline void Edge<VT, ET>::append(Vertex<VT, ET>* target, Edge<VT, ET>* next_edge)
	{
		if(target == nullptr || next_edge == nullptr) return;

		if(links[0].vertex == target)
		{
			links[0].next = next_edge;
		}
		else if(links[1].vertex == target)
		{
			links[1].next = next_edge;
		}
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
	inline void Graph<VT, ET>::Initialize(int init_vertex_number)
	{
		ClearVertex();
		target_vertex_number = init_vertex_number;
		vertex_list.reserve(init_vertex_number);
		for(int i = 0; i < target_vertex_number; i++)
		{
			vertex_list.push_back(new Vertex<VT, ET>());
		}
	}

	template<typename VT, typename ET>
	inline bool Graph<VT, ET>::IsConnected(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		bool found = false;
		Edge<VT, ET>* edge_list = vertex1.front;
		while(edge_list != nullptr)
		{
			if(edge_list->links[0].vertex == vertex1 && edge_list->links[1].vertex == vertex2)
				found = true;
			else if(edge_list->links[0].vertex == vertex2 && edge_list->links[1].vertex == vertex1)
				found = true;

			if(found)
				return true;

			edge_list = edge_list->Next(vertex1);
		}

		return false;
	}

	template<typename VT, typename ET>
	inline std::vector<Edge<VT, ET>*> Graph<VT, ET>::FindConnections(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		std::vector<Edge<VT, ET>*> founds;

		Edge<VT, ET>* current_edge = vertex1.front;
		while(current_edge != nullptr)
		{
			if(current_edge->links[0].vertex == vertex1 && current_edge->links[1].vertex == vertex2)
				founds.push_back(current_edge);
			else if(current_edge->links[0].vertex == vertex2 && current_edge->links[1].vertex == vertex1)
				founds.push_back(current_edge);

			current_edge = current_edge->Next(vertex1);
		}

		return founds;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Connect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
		Edge<VT, ET>* back1 = vertex1->GetBack();
		Edge<VT, ET>* back2 = vertex2->GetBack();
		Edge<VT, ET>* connector = new Edge<VT, ET>(vertex1, vertex2);
		if(nullptr == back1)
		{
			vertex1->front = connector;
		}
		else
		{
			back1->links[(back1->links[0].vertex == vertex1 ? 0 : 1)]
				= connector;
		}

		if(nullptr == back2)
		{
			vertex2->front = connector;
		}
		else
		{
			back2->links[(back1->links[0].vertex == vertex2 ? 0 : 1)]
				= connector;
		}
		++current_edge_number;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::Disconnect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2)
	{
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::PopEdge(Edge<VT, ET>* target_edge)
	{
		std::vector< Edge<VT, ET>*> befores;
		for(Link<VT, ET> link : target_edge->links)
		{
			if(std::find(vertex_list.front(), vertex_list.end(), link.vertex) == vertex_list.end())
				throw GraphException(LINE_INFO, "given Edge is not belong to this graph");

			befores.push_back(link.vertex->front);
		}
				
		if(befores[0] == nullptr || befores[1] == nullptr)
			throw GraphException(LINE_INFO, "The vertex doesn't have any edges");

		for(int i = 0; i < 2; i++)
		{
			Edge<VT, ET>* &before = befores[i];
			Vertex<VT, ET>* target_vertex = target_edge->links[i].vertex;
			if(before == target_edge) before == nullptr;

			while(before != nullptr)
			{
				Edge<VT, ET>* next = before->Next(target_vertex);
				if(next == target_edge) break;
				if(next == nullptr) throw GraphException(LINE_INFO, "Fail to find Edge");
				before = next;
			}
			
			if(before == nullptr)
			{
				target_vertex->front = target_edge->Next(target_vertex);
			}
			else
			{
				int selector = before->FindIndex(target_vertex);
				before->links[selector].next = target_edge->Next(target_vertex);
			}
		}
		delete target_edge;
		target_edge = nullptr;
		--current_edge_number;
	}

	template<typename VT, typename ET>
	inline Vertex<VT, ET>* Graph<VT, ET>::PopVertex(Vertex<VT, ET>* target)
	{
		return NULL;
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


}

