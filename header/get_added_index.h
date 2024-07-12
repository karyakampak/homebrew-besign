#ifndef GET_ADDED_INDEX_H
#define GET_ADDED_INDEX_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class GetAddedIndex {

public:
    // Constructor
    GetAddedIndex();

    // Member function declaration
    std::vector<size_t> get_index_after_add_dict(std::vector<uint8_t>& added_index);
};

#endif