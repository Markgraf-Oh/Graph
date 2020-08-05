#pragma once

#include <vector>
#include <stack>

template<typename VT = int, typename ET = float>
class AdjacencyMultiListGraph;

template<typename VT = int, typename ET = float>
class AMLVertex;

template<typename VT = int, typename ET = float>
class AMLEdge;

template<typename VT = int, typename ET = float>
struct Link
{
	AMLVertex<VT, ET>* vertex = nullptr;
	AMLEdge<VT, ET>* next = nullptr;

	Link(AMLVertex<VT, ET>* init_vertex) : vertex(init_vertex) {};
};

template<typename VT = int, typename ET = float>
class AMLVertex
{
	friend AdjacencyMultiListGraph<VT, ET>;
	friend AMLEdge<VT, ET>;
	//생성자, 소멸자
public:
	AMLVertex();

	/**@param	init_data	initial data of this vertex
	*/
	AMLVertex(VT init_data);

	virtual ~AMLVertex();

	//변수
public:
	VT data;

protected:
	AMLEdge<VT, ET>* front = nullptr;
	
	//함수
public:
	/**	이 vertex의 linked list를 탐색하여 degree를 구합니다.
	*@return	degree
	*@note		그래프의 모든 vertex에 대해 이 함수를 실행해서 mean degree를 구하는 것은 리소스 낭비입니다.
				그래프의 GetEdgeNumber를 사용하세요.
	*@see		AdjacencyMultiListGraph<VT,ET>::GetEdgeNumber()
	*/
	int GetDegree();

protected:
	void _DeleteEdge();
};

template<typename VT = int, typename ET = float>
class AMLEdge
{
	
	friend AdjacencyMultiListGraph<VT, ET>;
	friend AMLVertex<VT, ET>;
	//생성자, 소멸자
public:
	AMLEdge();

	/**@param	init_data	initial data of this edge
	*/
	AMLEdge(ET init_data);
	
	virtual ~AMLEdge();

	//변수
public:
	ET data;

	bool mark = false;

protected:
	Link<VT, ET> links[2];

	//함수
public:
	//AMLEdge<VT, ET>* Next(AMLVertex<VT, ET>* target_vertex);
};

template<typename VT = int, typename ET = float>
class AdjacencyMultiListGraph
{
	//생성자 소멸자
public:
	//AdjacencyMultiListGraph();
	//
	//AdjacencyMultiListGraph(int init_vertex_number);
	//
	//virtual ~AdjacencyMultiListGraph();

	//변수
public:
	std::vector<AMLVertex<VT, ET>*> vertexs;

protected:
	int vertex_number = 0;

	int edge_number = 0;

	//함수
public:
	//return number of vertices in this graph
	//int GetVertexNumber(); //ㅁㅊ vertex의 복수형이 vertices라니 진짜 영어의 규칙성은...
	//
	//void Initialize(int init_vertex_number);
	//
	//static void Connect(AMLVertex<VT, ET>* vertex1, AMLVertex<VT, ET>* vertex2);
	//
	//static void Disconnect(AMLVertex<VT, ET>* vertex1, AMLVertex<VT, ET>* vertex2);
	//
	//AMLVertex<VT, ET>* PopVertex(AMLVertex<VT, ET>* target);
	//
	//void ClearVertex();
	//
	//void ClearEdge();
};

template<typename VT, typename ET>
inline AMLVertex<VT, ET>::AMLVertex()
{
	data = VT();
}

template<typename VT, typename ET>
inline AMLVertex<VT, ET>::AMLVertex(VT init_data)
{
	data = init_data;
}

template<typename VT, typename ET>
inline AMLVertex<VT, ET>::~AMLVertex()
{
	_DeleteEdge();
}

template<typename VT, typename ET>
inline int AMLVertex<VT, ET>::GetDegree()
{
	int degree = 0;
	AMLEdge<VT, ET>* current = front;
	while (current != nullptr)
	{
		++degree;
		current = current->Next(this);
	}
	return degree;
}

template<typename VT, typename ET>
inline void AMLVertex<VT, ET>::_DeleteEdge()
{
	AMLEdge<VT, ET>* current = front;
	std::stack<AMLEdge<VT, ET>*> edge_stack;
	while (current != nullptr)
	{
		current->mark = false;
		int selector = (current->links[0].vertex == this) ? 0 : 1;
		if (current->links[!selector].vertex == nullptr)
		{
			current->mark = true;
		}
		current->links->vertex == nullptr;
		edge_stack.push(current);
		current = std::exchange(current->links[selector].next, nullptr);
	}

	while (!edge_stack.empty())
	{
		current = edge_stack.top();
		if (current->mark)
		{
			delete current;
			current = nullptr;
		}
		edge_stack.pop();
	}
}

template<typename VT, typename ET>
inline AMLEdge<VT, ET>::AMLEdge()
{
	if (typeid(ET) == typeid(float))
	{
		data = 1.0;
	}
	else data = ET();
}

template<typename VT, typename ET>
inline AMLEdge<VT, ET>::AMLEdge(ET init_data)
{
	data = init_data;
}

template<typename VT, typename ET>
inline AMLEdge<VT, ET>::~AMLEdge()
{

}
