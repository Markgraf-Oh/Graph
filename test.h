#pragma once

#include <iostream>

template<typename VertexType, typename EdgeType>
class Edge;


template<typename VertexType, typename EdgeType>
class Vertex
{
public:
	VertexType verval;

	Edge<VertexType, EdgeType>* test;

	Vertex(VertexType v, EdgeType e);

	virtual ~Vertex();
};

template<typename VertexType, typename EdgeType>
class Edge
{
public:
	EdgeType myval;

	Edge(EdgeType e)
	{
		myval = e;
	};

	void print()
	{
		std::cout << "test" << std::endl;
		std::cout << myval << std::endl;
	};
};

template<typename VertexType, typename EdgeType>
inline Vertex<VertexType, EdgeType>::Vertex(VertexType v, EdgeType e)
{
	test = new Edge<VertexType, EdgeType>(e);
	verval = v;
}

template<typename VertexType, typename EdgeType>
inline Vertex<VertexType, EdgeType>::~Vertex()
{
	delete test;
	test = nullptr;
}
