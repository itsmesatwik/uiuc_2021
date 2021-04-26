#include "cacheblock.h"

uint32_t Cache::Block::get_address() const {
    auto index = _cache_config.get_num_index_bits();
    auto tag = _cache_config.get_num_tag_bits();
    auto offset = 32 - tag - index;
    return (tag == 32) ? _tag : (_tag << (index + offset)) + (_index << offset);
}
