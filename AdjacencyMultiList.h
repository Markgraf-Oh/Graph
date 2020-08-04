#pragma once

template<typename VT = int, typename ET = float>
class AdjacencyMultiListGraph;

template<typename VT = int, typename ET = float>
class AMLVertex;

template<typename VT = int, typename ET = float>
class AMLEdge;

template<typename VT = int, typename ET = float>
class AMLVertex
{
	friend AdjacencyMultiListGraph<VT, ET>;
	friend AMLEdge<VT, ET>;
	//생성자, 소멸자
public:
	//AMLVertex();
	//AMLVertex(VT init_data);
	//virtual ~AMLVertex();

	//변수
protected:
	AMLEdge<VT, ET>* front = nullptr;

	VT data;

	//함수
};

template<typename VT = int, typename ET = float>
class AMLEdge
{
	struct Link
	{
		AMLVertex<VT, ET>* vertex = nullptr;
		AMLEdge<VT, ET>* next = nullptr;

		Link(AMLVertex<VT, ET>* init_vertex) : vertex(init_vertex) {};
	};
	friend AdjacencyMultiListGraph<VT, ET>;
	friend AMLVertex<VT, ET>;
	//생성자, 소멸자
public:
	AMLEdge();
	AMLEdge(ET init_data);
	virtual ~AMLEdge();

	//변수
protected:
	Link links[2];

	bool mark = false;
};

template<typename VT = int, typename ET = float>
class AdjacencyMultiListGraph
{
	//생성자 소멸자
public:


	//변수
public:
	AMLVertex<VT, ET>* vertex_array;

protected:
	int vertex_number = 0;

	int edge_number = 0;

};