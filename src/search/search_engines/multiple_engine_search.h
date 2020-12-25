#ifndef SEARCH_ENGINES_MULTIPLE_ENGINES_SEARCH_H
#define SEARCH_ENGINES_MULTIPLE_ENGINES_SEARCH_H

#include "../option_parser.h"
#include "../option_parser_util.h"
#include "../search_engine.h"

#include "../options/registries.h"
#include "../options/predefinitions.h"

namespace options {
    class Options;
}

namespace multiple_engines_search {
    class MultipleEnginesSearch : public SearchEngine {
        const std::vector<options::ParseTree> engine_configs;
        /*
          We need to copy the registry and predefinitions here since they live
          longer than the objects referenced in the constructor.
        */
        options::Registry registry;
        options::Predefinitions predefinitions;

    protected:
        std::shared_ptr<SearchEngine> get_search_engine(int engine_configs_index) {
            OptionParser parser(engine_configs[engine_configs_index], registry, predefinitions, false);
            std::shared_ptr <SearchEngine> engine(parser.start_parsing < std::shared_ptr < SearchEngine >> ());

            std::ostringstream stream;
            kptree::print_tree_bracketed(engine_configs[engine_configs_index], stream);

            return engine;
        }

    public:
        MultipleEnginesSearch(const options::Options &opts, options::Registry &registry,
                              const options::Predefinitions &predefinitions): SearchEngine(opts),
                              registry(registry), predefinitions(predefinitions){}
    };
}

#endif
