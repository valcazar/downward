#ifndef SEARCH_ENGINES_BIDIRECTIONAL_SEARCH_H
#define SEARCH_ENGINES_BIDIRECTIONAL_SEARCH_H

#include "./multiple_engine_search.h"

#include "../options/registries.h"
#include "../options/predefinitions.h"

namespace options {
class Options;
}

namespace bidirectional_search {
class BidirectionalSearch : public MultipleEngineSearch {
    const std::vector<options::ParseTree> engine_configs;

    std::unique_ptr<SearchEngine> forward_search;
    std::unique_ptr<SearchEngine> backward_backward;


protected:
    virtual void initialize() override;
    virtual SearchStatus step() override;

public:
    BidirectionalSearch(const options::Options &opts, options::Registry &registry,
                        const options::Predefinitions &predefinitions);

    virtual void save_plan_if_necessary() override;
    virtual void print_statistics() const override;
};
}

#endif
