# Graph

This is a **C++** Library for Network Science Study etc.
This Library consist of **Graph Data-Structure** implemented as **Adjacency-Multi-List**
and various **related Algorithms**.

I made this repository for personal study, and want to share with fellow new-comers of Computer-Science.


You are free to copy, fork, use. But don't forget to add original link, and add 1 **star :)**


# What's In Hear?


I feel shame as the code is quite messy. Let me explain in order

## AdjacencyMultiList.h

Header file implementing graph data structure with **adjacent multiple list**.
It is the basis of other header files.

Inside are three template classes: **Vertex, Edge, and Graph**.
Depending on the field of study, you can understand the Graph as the Network and Vertex as the Nodes.

You can specify Datas to store inside each Vertices and Edges as template types VT, ET.
Since the three data structures works together, you need to specify the type of Edge when makes Vertex, and vice versa.

Addition, deletion, and connection between Vertices and Edges should always be made through the Graph object that owns them.
For your convenience, you have public access to the vertex_list in Graph.
But if you delete or erase vertex directly through the vertex_list, serious errors could be occur. (will fix later)


## MinimunSpanningTree.h

Contains **Prim Algorithm** and **Kruskal Algorithm** which used to find a minimum spanning tree.

You need to define a function which returns custom weight value of Edge class, and pass the function pointer as parameter.

## ShortestPath.h

Contains Shortest Path Finding Algorithms : **Dijkstra Algorithm** and **A\* Algorithm**
Same as the Spanning Tree Algorithms, You need to define a custom GetEdgeWeight function.


## UnionFind.h

Simple Union-Find Algorithm used in **Kruskal Algorithm**

## Network.h

Functions in this headers makes some sample Networks with Adjacency Multi List.

This Networks are widely used in Network Science.

**ER Network** : Most basic Random Network

**Barabasi-Albert Network** : Typical Scale-free Network

This Header includes FindClusters function which finds every clusters of vertices inside given graph

## 기타

Test codes and related header.

##### Graph.cpp
includes main function

##### test.h
test code I used while making this libarary

##### BenchMark.h

ref : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
