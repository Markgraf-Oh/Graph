#pragma once

//original sorce : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>

namespace BenchMark
{
    //timer class for benchmarking
    class Timer
    {
    public:
        Timer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            start_point = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            if(!m_Stopped)
                Stop();
        }

        void Stop()
        {
            std::chrono::time_point<std::chrono::high_resolution_clock> end_point
                = std::chrono::high_resolution_clock::now();

            std::chrono::milliseconds deltatime
                = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);

            uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

            std::stringstream  ss;
            ss << m_Name << "in thread " << thread_id << " completed : " << deltatime.count() << "ms\n";
            std::cout << ss.str();

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_point;
        bool m_Stopped;
    };
}
