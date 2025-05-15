//
// Created by adrian on 15/05/25.
//

#include "Profiler.h"
#include <iostream>
#include <iomanip>
#include <utility>

std::unordered_map<std::string, Profiler::ProfileData> Profiler::s_Data;
std::ofstream Profiler::s_LogFile;
std::mutex Profiler::s_Mutex;

void Profiler::BeginSession(const std::string& filepath) {
    s_LogFile.open(filepath);
    s_LogFile << "Profiler Log Started\n";
}

void Profiler::EndSession() {
    s_LogFile << "Profiler Log Ended\n";
    s_LogFile.close();
    PrintStats();
}

void Profiler::LogScope(const std::string& name, const long long durationNs) {
    std::lock_guard lock(s_Mutex);
    auto&[totalNs, minNs, maxNs, callCount] = s_Data[name];
    totalNs += durationNs;
    callCount++;
    if (durationNs < minNs) minNs = durationNs;
    if (durationNs > maxNs) maxNs = durationNs;
    s_LogFile << name << ": " << durationNs << " ns\n";
}

void Profiler::PrintStats() {
    std::cout << "\n==== PROFILER STATS ====\n";
    for (const auto& [name, data] : s_Data) {
        const double avg = data.callCount ? static_cast<double>(data.totalNs) / static_cast<double>(data.callCount) : 0.0;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << " - " << name << " | "
                  << "calls: " << data.callCount
                  << ", total: " << static_cast<double>(data.totalNs) / 1e6 << " ms"
                  << ", avg: " << avg / 1e6 << " ms"
                  << ", min: " << static_cast<double>(data.minNs) / 1e6 << " ms"
                  << ", max: " << static_cast<double>(data.maxNs) / 1e6 << " ms"
                  << "\n";
    }
    std::cout << "========================\n";
}

// ------------------------- //

ProfileScope::ProfileScope(std::string name)
    : m_Name(std::move(name)), m_Start(std::chrono::high_resolution_clock::now()) {}

ProfileScope::~ProfileScope() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_Start).count();
    Profiler::LogScope(m_Name, duration);
}
