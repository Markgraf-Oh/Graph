[English README](README_english.md)

# Graph

네트워크 사이언스 등을 위한 **C++** 라이브러리입니다.
이 라이브러리는 **인접다중리스트**로 구현된 **그래프 자료구조** 및 연관된 여러 **알고리즘**들로 구성되어 있습니다.

개인적인 CS공부를 위해 만든 레포지토리로, 이제 막 프로그래밍에 발디딘 초보자 분들과 공유하고 싶습니다.

마음껏 이용하셔도 좋습니다. 그치만 링크 남기는 거랑 **스타** 달주시는 건 빼먹지 마시고요 ㅎㅎ

# What's In Hear?


꽤나 복잡하게 코드가 꼬여있으니 참 할말이 없습니다. 순서대로 설명하겠습니다.

## AdjacencyMultiList.h

**인접 다중 리스트**로 그래프 자료구조를 구현한 헤더파일입니다. 다른 헤더파일들의 기본이됩니다.
안에는 **Vertex, Edge, Graph**의 세가지 템플릿 클래스가 있습니다.
학문 분야에 따라 Graph는 Network, Vertex는 Node라고 이해하셔도 무방합니다.

Vertex와 Edge에 담을 자료를 VT, ET의 템플릿 타입으로 지정할 수 있습니다.
세 자료구조는 한 쌍이기 때문에 Vertex를 만들려면 Edge의 타입도 지정해줘야하며, 그 반대도 마찬가지 입니다.

Vertex와 Edge의 추가, 삭제, 연결 등은 항상 둘을 소유하고 있는 Graph 객체를 통해서 진행되어야합니다.
현재 편의상 Graph의 vertex_list를 public하게 접근할 수 있는데, 만약 vertex_list를 통해 직접 Vertex를 delete하거나 erase하면 심각한 오류가 있을 수 있습니다. (수정예정)

## MinimunSpanningTree.h

최소 신장 트리를 찾는 **Prim Algorithm**과 **Kruskal Algorithm** 이 들어있습니다.

이 두 함수는 Edge 클래스에서 weight 값을 받는 함수의 함수포인터를 파라미터로 받습니다.
이는 간단한 람다함수로 구현할 수 있습니다.


## ShortestPath.h

최단 경로 탐색 알고리즘인 **Dijkstra Algorithm**과 **A\* Algorithm**이 들어있습니다.

마찬가지로 Edge의 weight값을 구하는 함수를 파라미터로 쓰며, A\*알고리즘은 추가로 휴리스틱함수를 요구합니다.

## UnionFind.h

**Kruskal Algorithm**에 쓰이는 UnionFind Algorithm의 간단한 구현체입니다.

## Network.h

Adjacency Multi List로 네트워크 사이언스에 쓰이는 네트워크 몇가지를 만드는 간단한 알고리즘들입니다.

가장 기본적인 랜덤 네트워크인 **ER Network**를 만드는 함수,

대표적인 Scale-free Network인 **Barabasi-Albert Network**를 만드는 함수가 있습니다.

추가로 네트워크 상에서 각 노드들을 클러스터별로 분류하는 FindClusters 함수도 있습니다.

## 기타

테스트 코드 및 그에 관련된 헤더들입니다. 라이브러리에는 포함되지 않습니다.

##### Graph.cpp
메인 함수가 있는 비주얼 스튜디오 기본 파일입니다. 테스트 코드 실행할때 쓰시면 됩니다.

##### test.h
작업하면서 사용한 테스트 코드 샘플입니다. 사용법을 보기에 좋습니다.

##### BenchMark.h
유튜버 Cherno의 벤치마크 용 클래스를 참고하여 간략화 했습니다.

ref : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

# Future Feature

1. 템플릿 클래스인 Graph<VT, ET>에서 Vertex<VT, ET>, Edge<VT, ET>에 쉽게 접근 가능하도록 typedef 추가 예정.
(이거 수정하려면 클래스 내부에서 새로 싹다 수정해야 함...)

2. 현재 raw pointer로 구현되어 있는 Graph 내부의 vertex_list vector를 안정성을 위해 shared pointer로 변경할 예정.
   동시에 FindCluster 함수는 weak pointer를 쓰게 될 예정.(구조 전첸를 뜯어고쳐야 해서 걍 취소될 수도 있음.)
