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
	//������, �Ҹ���
public:
	//AMLVertex();
	//AMLVertex(VT init_data);
	//virtual ~AMLVertex();

	//����
protected:
	AMLEdge<VT, ET>* front = nullptr;

	VT data;

	//�Լ�
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
	//������, �Ҹ���
public:
	AMLEdge();
	AMLEdge(ET init_data);
	virtual ~AMLEdge();

	//����
protected:
	Link links[2];

	bool mark = false;
};

template<typename VT = int, typename ET = float>
class AdjacencyMultiListGraph
{
	//������ �Ҹ���
public:


	//����
public:
	AMLVertex<VT, ET>* vertex_array;

protected:
	int vertex_number = 0;

	int edge_number = 0;

};