#pragma once

#include <vector>
#include <stack>
#include <array>
#include <string>
#include <list>

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

	template<typename VT = int, typename ET = float>
	class Vertex
	{
		friend Graph<VT, ET>;
		friend Edge<VT, ET>;
		//������, �Ҹ���
	public:
		Vertex();

		/**@param	init_data	initial data of this vertex
		*/
		Vertex(VT init_data);

		virtual ~Vertex();

		//����
	public:
		VT data;

	protected:
		Edge<VT, ET>* front = nullptr;

		//�Լ�
	public:
		/**	�� vertex�� linked list�� Ž���Ͽ� degree�� ���մϴ�.
		*@return	degree
		*@note		�׷����� ��� vertex�� ���� �� �Լ��� �����ؼ� mean degree�� ���ϴ� ���� ���ҽ� �����Դϴ�.
					�׷����� GetEdgeNumber�� ����ϼ���.
		*@see		Graph<VT,ET>::GetEdgeNumber()
		*/
		int GetDegree();

		void DisConnectAll();

		Edge<VT, ET>* GetFront();

		Edge<VT, ET>* GetBack();

	protected:
		void _DeleteEdge();//�̰� graph������
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
		//������, �Ҹ���
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

		//����
	public:
		ET data;

		bool mark = false;

	protected:
		std::array<Vertex<VT, ET>*, 2> vertex{{nullptr, nullptr}};
		std::array<Edge<VT, ET>*, 2> next{{nullptr, nullptr}};
		std::array<Edge<VT, ET>*, 2> before{{nullptr, nullptr}};

		//�Լ�
	public:
		Edge<VT, ET>* GetNext(Vertex<VT, ET>* target_vertex);

		Edge<VT, ET>* GetBefore(Vertex<VT, ET>* target_vertex);

		/**	�־��� vertex�� array �󿡼��� index�� ã���ϴ�.
		*@return	�ش� vertex�� ã���� 0 or 1
		*			��ã���� -1
		*/
		int FindIndex(Vertex<VT, ET>* target_vertex);

	protected:
		void append(Vertex<VT, ET>* target, Edge<VT, ET>* next_edge);
	};

	template<typename VT = int, typename ET = float>
	class Graph
	{
		//������ �Ҹ���
	public:
		Graph();

		Graph(int init_vertex_number);

		virtual ~Graph();

		//����
	public:
		std::vector<Vertex<VT, ET>*> vertex_list;

	protected:
		int target_vertex_number = 0;

		int current_edge_number = 0;

		//�Լ�
	public:
		//return number of vertices in this graph
		int GetTargetVertexNumber(){ return target_vertex_number; }; //vertex�� �������� vertices��� ��¥ ������ ��Ģ����...

		int GetEdgeNumber(){ return current_edge_number; };
		//
		void Initialize(int init_vertex_number);

		bool IsConnected(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		bool IsConnected(int i, int j);

		std::vector<Edge<VT, ET>*> FindConnections(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);
		
		//
		void Connect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		void Connect(int i, int j);
		//
		void Disconnect(Vertex<VT, ET>* vertex1, Vertex<VT, ET>* vertex2);

		Edge<VT, ET>* PopEdge(Edge<VT, ET>* target_edge, Vertex<VT, ET>* return_reference_vertex);
		
		void PopVertex(Vertex<VT, ET>* target);
		
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
			current = current->GetNext(this);
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
		while(back->GetNext(this) != nullptr)
		{
			back = back->GetNext(this);
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

		this->front = nullptr;
	}


	template<typename VT, typename ET>
	inline Edge<VT, ET>* Edge<VT, ET>::GetNext(Vertex<VT, ET>* target_vertex)
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
	inline Edge<VT, ET>* Edge<VT, ET>::GetBefore(Vertex<VT, ET>* target_vertex)
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
	inline int Edge<VT, ET>::FindIndex(Vertex<VT, ET>* target_vertex)
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
	inline bool Graph<VT, ET>::IsConnected(Vertex<VT, ET>* vertex0, Vertex<VT, ET>* vertex1)
	{
		Edge<VT, ET>* edge_list = vertex0.front;
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
	inline bool Graph<VT, ET>::IsConnected(int i, int j)
	{
		if(i >= vertex_list.size() || j >= vertex_list.size() | i==j)
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

		if(return_reference_vertex == nullptr)
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
				next_edge->before[selector] = before_edge;//������
			}
		}

		delete target_edge;
		target_edge = nullptr;
		--current_edge_number;

		return return_edge;
	}

	template<typename VT, typename ET>
	inline void Graph<VT, ET>::PopVertex(Vertex<VT, ET>* target)
	{
		int loc = -1;
		for(int i = 0; vertex_list.size(); i++)
		{
			if(vertex_list[i] == target)
			{
				loc = i;
				break;
			}
		}

		if(loc <0)
			throw std::invalid_argument("target vertex is not included in this graph");

		while(target->front != nullptr)
		{
			PopEdge(target->front, target);
		}
		delete target;
		target = nullptr;
		std::swap(vertex_list[loc], vertex_list.back());
		vertex_list.pop_back();
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

