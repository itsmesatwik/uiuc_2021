#include "simplecache.h"
#include<iostream>

int SimpleCache::find(int index, int tag, int block_offset) {
  // read handout for implementation details
  // checking if the index is valid or not
  
  /*if (_cache.find(index) == _cache.end() || block_offset > 3 || block_offset < 0|| index < 0) {
  	return 0xdeadbeef;
  }*/
  for (auto cache_block : _cache[index]) {
  	if (cache_block.tag() == tag && cache_block.valid()) {
  		return cache_block.get_byte(block_offset);
  	}
  }
  return 0xdeadbeef;
}

void SimpleCache::insert(int index, int tag, char data[]) {
    // Checking for bad index
  /*if (_cache.find(index) == _cache.end() || index < 0) {
  	return;
  }*/
  for (auto& cache_block : _cache[index]) {
  	if (!cache_block.valid()) {
  		cache_block.replace(tag, data);
  		return;
  	}
  }
  _cache[index][0].replace(tag,data);
}
