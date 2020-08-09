// Graph.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <random>
#include "AdjacencyMultiList.h"

int GetRandomIndex(std::minstd_rand& generator, int size)
{
    std::uniform_int_distribution<int> dist_uniform_int(0, size-1);

    return dist_uniform_int(generator);
}

int main()
{
    namespace aml = AdjacencyMultiList;

    int node_number = 10000;
    int mean_degree = 4;
    float connect_ratio = (float)mean_degree / (float)node_number;

    aml::Graph<int, float> test_graph(node_number);

    test_graph.Initialize(node_number);

    std::vector<int> histogram(node_number, 0);

    std::minstd_rand generator_basic(42);

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

    //Creating ER Network
    for(int i = 1; i < node_number; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(uniform_dist(generator_basic) < connect_ratio)
            {
                test_graph.Connect(i, j);
            }
        }
    }

    std::cout << test_graph.GetEdgeNumber() << std::endl;

    for(int i = 0; i < node_number; i++)
    {
        histogram[test_graph.vertex_list[i]->GetDegree()] += 1;
    }

    for(int i = 0; i < 100; i++)
    {
        std::cout << histogram[i] << " ";
    }
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
