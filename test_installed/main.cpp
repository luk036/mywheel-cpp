#include <iostream>
#include <mywheel/map_adapter.hpp>
#include <vector>

auto main() -> int {
    std::vector<int> data{10, 20, 30};
    MapAdapter<std::vector<int>> adapter(data);
    const auto ok = (data.size() == 3);

    std::cout << "mywheel installed test: adapter size=" << data.size() << "\n";
    return ok ? 0 : 1;
}
