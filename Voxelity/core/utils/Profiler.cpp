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
    const auto it = s_Data.find("MainLoop");
    if (it == s_Data.end()) {
        std::cerr << "[Profiler] 'MainLoop' scope not found!\n";
        return;
    }

    const long long mainLoopTotalNs = it->second.totalNs;
    const double mainLoopAvgNs = it->second.callCount ? static_cast<double>(it->second.totalNs) / it->second.callCount : 0.0;

    if (mainLoopTotalNs == 0 || mainLoopAvgNs == 0.0) {
        std::cerr << "[Profiler] Invalid 'MainLoop' data: zero total or average duration.\n";
        return;
    }

    std::vector<std::pair<std::string, ProfileData>> entries(s_Data.begin(), s_Data.end());
    std::sort(entries.begin(), entries.end(),
              [](const auto& a, const auto& b) {
                  return a.second.totalNs > b.second.totalNs;
              });

    std::cout << "\n==== PROFILER STATS (relative to MainLoop) ====\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& [name, data] : entries) {
        const double avgNs = data.callCount ? static_cast<double>(data.totalNs) / data.callCount : 0.0;
        const double totalPct = 100.0 * static_cast<double>(data.totalNs) / mainLoopTotalNs;
        const double avgPct = 100.0 * avgNs / mainLoopAvgNs;

        std::cout << " - " << name
                  << " | total: " << data.totalNs / 1e6 << " ms"
                  << " (" << totalPct << "%)"
                  << ", calls: " << data.callCount
                  << ", avg: " << avgNs / 1e6 << " ms"
                  << " (" << avgPct << "%)"
                  << "\n";
    }
    std::cout << "=============================================\n";
}

// ------------------------- //

ProfileScope::ProfileScope(std::string name)
    : m_Name(std::move(name)), m_Start(std::chrono::high_resolution_clock::now()) {}

ProfileScope::~ProfileScope() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_Start).count();
    Profiler::LogScope(m_Name, duration);
}
