#include <cxxopts.hpp>
#include <iostream>
#include <mywheel/bpqueue.hpp>
#include <mywheel/dllink.hpp>
#include <string>
#include <utility>

auto main(int argc, char** argv) -> int {
    cxxopts::Options options("MyWheel", "Bounded priority queue demo");
    options.add_options()("h,help", "Print usage")("n,count", "Number of items",
                                                   cxxopts::value<int>()->default_value("8"));

    const auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
        std::cout << options.help() << '\n';
        return 0;
    }

    const int count = result["count"].as<int>();
    const int min_key = 0;
    const int max_key = 16;

    std::vector<Dllink<std::pair<int, uint32_t>>> nodes(count);
    BPQueue<int, int32_t> bpq(min_key, max_key);

    for (int i = 0; i < count; ++i) {
        bpq.append(nodes[static_cast<size_t>(i)], i);
    }

    std::cout << "mywheel: queue empty=" << (bpq.is_empty() ? "yes" : "no") << '\n';
    std::cout << "mywheel: max key=" << bpq.get_max() << '\n';

    while (!bpq.is_empty()) {
        std::cout << ' ' << bpq.get_max();
        bpq.popleft();
    }
    std::cout << '\n';

    return 0;
}
