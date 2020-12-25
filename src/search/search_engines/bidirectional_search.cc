#include "bidirectional_search.h"

#include "../option_parser.h"
#include "../plugin.h"

#include "../utils/logging.h"

#include <iostream>

using namespace std;

namespace bidirectional_search {
    BidirectionalSearch::BidirectionalSearch(const Options &opts, options::Registry &registry,
                                             const options::Predefinitions &predefinitions)
            : MultipleEngineSearch(opts),
              engine_configs(opts.get_list<ParseTree>("engine_configs")) {
    }
}

void BidirectionalSearch::initialize() {
    forward_search = get_search_engine(0);

    cout << "Forward search: ";
    kptree::print_tree_bracketed(engine_configs[0], cout);
    cout << endl;

    backward_search = get_search_engine(1);

    cout << "Backward search: ";
    kptree::print_tree_bracketed(engine_configs[1], cout);
    cout << endl;

    cout << "Initializing forward search..." << endl;
    forward_search->initialize();

    cout << "Initializing backward search..." << endl;
    backward_search->initialize();
}

SearchStatus BidirectionalSearch::step() {
    return IN_PROGRESS;
}

void IteratedSearch::print_statistics() const {
    utils::g_log << "Cumulative statistics:" << endl;
    statistics.print_detailed_statistics();
}

void IteratedSearch::save_plan_if_necessary() {
    // We don't need to save here, as we automatically save after
    // each successful search iteration.
}

static shared_ptr <SearchEngine> _parse(OptionParser &parser) {
    parser.document_synopsis("Bidirectional search", "");
    parser.add_list_option<ParseTree>("engine_configs",
                                      "2 engines, first is forward, second is backward");
    SearchEngine::add_options_to_parser(parser);
    Options opts = parser.parse();

    opts.verify_list_exact_length<ParseTree>("engine_configs", 2);

    if (parser.help_mode()) {
        return nullptr;
    } else if (parser.dry_run()) {
        //check if the supplied search engines can be parsed
        for (const ParseTree &config : opts.get_list<ParseTree>("engine_configs")) {
            OptionParser test_parser(config, parser.get_registry(),
                                     parser.get_predefinitions(), true);
            test_parser.start_parsing < shared_ptr < SearchEngine >> ();
        }
        return nullptr;
    } else {
        return make_shared<BidirectionalSearch>(opts, parser.get_registry(),
                                                parser.get_predefinitions());
    }
}

static Plugin<SearchEngine> _plugin("bidirectional", _parse);
}
