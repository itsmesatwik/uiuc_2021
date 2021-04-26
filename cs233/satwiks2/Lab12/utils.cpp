#include "utils.h"

uint32_t extract_tag(uint32_t address, const CacheConfig& cache_config) {
  // TODO
  auto tag = cache_config.get_num_tag_bits();
  return (tag != 0) ? address >> (32 - tag) : 0;
}

uint32_t extract_index(uint32_t address, const CacheConfig& cache_config) {
  // TODO
  auto tag = cache_config.get_num_tag_bits();
  auto offset = cache_config.get_num_block_offset_bits();
  return (cache_config.get_num_index_bits() != 0) ? (address << tag) >> tag >> offset : 0;
}

uint32_t extract_block_offset(uint32_t address, const CacheConfig& cache_config) {
  // TODO
  auto offset = cache_config.get_num_block_offset_bits();
  return (offset != 0) ? (address << (32 - offset) >> (32 - offset)) : 0;
}
