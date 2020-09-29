#include <set>
#include <map>
#include <iterator>
#include <iostream>

#include "Interactions.h"

intdict generateDictionary(const std::multiset<std::multiset<P>> interactions) {

    std::set<std::map<std::multiset<P>, P>> nto1s{};

    for (auto interaction{ interactions.begin() }; interaction != interactions.end(); ++interaction) {
        auto b{ interaction->begin() };
        for (int i{}; i < static_cast<int>(interaction->size()); ++i) {
            auto lefthand = *interaction;
            auto blh{ lefthand.begin() };
            std::advance(blh, i);
            auto element = *blh;
            lefthand.erase(blh);
            std::map<std::multiset<P>, P> toInsert{};
            toInsert.insert(std::make_pair(lefthand, element));
            nto1s.insert(toInsert);
        }
    }
    return nto1s;

}