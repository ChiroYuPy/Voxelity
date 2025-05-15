#include "Application.h"
#include "Profiler.h"

int main() {
    Profiler::BeginSession();
    Application app;
    app.run();
    Profiler::EndSession();
    return 0;
}
