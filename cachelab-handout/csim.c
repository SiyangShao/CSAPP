#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BIT_OF_COMPUTER 64
int v = 0, s = -1, E = -1, b = -1, s_bit = 0;
int hit_cnt = 0, miss_cnt = 0, eviction_cnt = 0;
char t[1024];
typedef unsigned int ui;
typedef struct cache {
  ui valid_bits;
  int tag, stamp;
} cache_line, *cache_asso, **cache;
cache __cache = NULL;
void help_function() {
  printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
         "-h: Optional help flag that prints usage info\n"
         "-v: Optional verbose flag that displays trace info\n"
         "-s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
         "-E <E>: Associativity (number of lines per set)\n"
         "-b <b>: Number of block bits (B = 2^b is the block size)\n"
         "-t <tracefile>: Name of the valgrind trace to replay\n");
}
void init_cache() {
  __cache = (cache)malloc(sizeof(cache_asso) * s);
  for (int i = 0; i < s; ++i) {
    __cache[i] = (cache_asso)malloc(sizeof(cache_line) * E);
    for (int j = 0; j < E; ++j) {
      __cache[i][j].valid_bits = 0;
      __cache[i][j].tag = -1;
      __cache[i][j].stamp = -1;
    }
  }
}
void update(ui address) {
  ui MAX = 0xffffffff;
  ui index_diff = (address >> b) & (MAX >> (BIT_OF_COMPUTER - s_bit));
  ui tag_add = address >> (b + s_bit);
  int max_stamp = -1, max_index = -1;
  for (int i = 0; i < E; ++i) {
    if (__cache[index_diff][i].tag == tag_add) {
      // hit
      __cache[index_diff][i].stamp = 0;
      hit_cnt++;
      return;
    }
  }
  // check if there's extra space in chace. Cache Miss
  miss_cnt++;
  for (int i = 0; i < E; ++i) {
    if (__cache[index_diff][i].valid_bits == 0) {
      __cache[index_diff][i].valid_bits = 1;
      __cache[index_diff][i].tag = tag_add;
      __cache[index_diff][i].stamp = 0;
      return;
    }
  }
  // have to eviction. Use LRU
  eviction_cnt++;
  for (int i = 0; i < E; ++i) {
    if (__cache[index_diff][i].stamp > max_stamp) {
      max_stamp = __cache[index_diff][i].stamp;
      max_index = i;
    }
  }
  __cache[index_diff][max_index].tag = tag_add;
  __cache[index_diff][max_index].stamp = 0;
}
void update_stamp() {
  for (int i = 0; i < s; ++i) {
    for (int j = 0; j < E; ++j) {
      if (__cache[i][j].valid_bits > 0) {
        __cache[i][j].stamp++;
      }
    }
  }
}
int main(int argc, char *argv[]) {
  int opt;
  while (~(opt = getopt(argc, argv, "hvs:E:b:t:"))) {
    switch (opt) {
    case 'h':
      // Helper function
      help_function();
      break;
    case 'v':
      v = 1;
      break;
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      strcpy(t, optarg);
      break;
    default:
      help_function();
      break;
    }
  }
  if (s < 0 || E < 0 || b < 0) {
    return -1;
  }
  s_bit = s;
  s = 1 << s;
  FILE *fp = fopen(t, "r");
  if (fp == NULL) {
    printf("Open File Error\n");
    exit(-1);
  }
  init_cache();
  char operation;
  ui address;
  int size;
  while (~fscanf(fp, " %c %xu, %d\n", &operation, &address, &size)) {
    switch (operation) {
    case 'I':
      continue;
    case 'L':
      update(address);
      break;
    case 'M':
      update(address);
      // after address, should have another storage
    case 'S':
      update(address);
    }
    update_stamp();
  }
  fclose(fp);
  for (int i = 0; i < s; ++i) {
    free(__cache[i]);
  }
  free(__cache);
  printSummary(hit_cnt, miss_cnt, eviction_cnt);
  return 0;
}