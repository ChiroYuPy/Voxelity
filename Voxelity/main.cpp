#include "core/Application.h"
#include "core/utils/Logger.h"
#include "core/utils/Profiler.h"

int main() {
    Logger::setLevel(LogLevel::Trace);
    Profiler::BeginSession();
    Application app;
    app.run();
    Profiler::EndSession();
    return 0;
}
