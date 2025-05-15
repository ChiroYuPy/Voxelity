//
// Created by adrian on 15/05/25.
//

#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include <chrono>
#include <climits>
#include <unordered_map>
#include <fstream>
#include <mutex>

class Profiler {
public:
    static void BeginSession(const std::string& filepath = "profile.log");
    static void EndSession();
    static void LogScope(const std::string& name, long long durationNs);

    struct ProfileData {
        long long totalNs = 0;
        long long minNs = LLONG_MAX;
        long long maxNs = 0;
        size_t callCount = 0;
    };

    static void PrintStats();

private:
    static std::unordered_map<std::string, ProfileData> s_Data;
    static std::ofstream s_LogFile;
    static std::mutex s_Mutex;
};

class ProfileScope {
public:
    ProfileScope(std::string  name);
    ~ProfileScope();
private:
    std::string m_Name;
    std::chrono::high_resolution_clock::time_point m_Start;
};

#define PROFILE_SCOPE(name) ProfileScope profileScope##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

#endif //PROFILER_H
