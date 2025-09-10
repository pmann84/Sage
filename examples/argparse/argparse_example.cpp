#include <sage/argparse/argparse.hpp>

int main(const int argc, char **argv) {
    auto parser = sage::argparse::argument_parser("Example Parser", "Test Parser");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({"-b", "--bar"}).num_args("*").help("Optional bar argument consumes all.");

    parser.parse_args(argc, argv);

    return 0;
}
