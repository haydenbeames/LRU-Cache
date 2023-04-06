#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

//////////////////////////////////////////
//   Hayden Beames  Net-ID: hbeames7
//////////////////////////////////////////

int verbose = 0;
int s = 0;
int E = 0;
int b = 0;
int v = 0;
int h = 0;
int t = 0;
char* tracefile = NULL;
int S;
int B;
int lru_set;
uint64_t lru_count = 1;
//final counters
int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

//prints all argument options when called
void printOptions(char* argv[]) {
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

typedef struct {
    uint8_t valid;
    uint64_t lru;
    uint64_t tag;
} block_t;

void initCache(block_t cache[S][E]) {
    for (int i = 0; i < S; i++) {
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = false;
            cache[i][j].lru = j;
            cache[i][j].tag = 0;
        }
    }
}

void cacheAccess(block_t cache[S][E], uint64_t address) {
    uint64_t set = address >> B;
    uint64_t setMask = 
    set = set & (setMask);
    uint64_t tag = address >> (S + B);
    //uint64_t evict_lru;
    lru_count++;
    for (int i = 0; i < E; i++) {
        if (cache[set][E].valid && (cache[set][E].tag == tag)) {
            cache[set][E].lru = lru_count;
            hit_count++;
            return;
            printf("HIT");
        }
    }
    for (int i = 0; i < (E-1); i++) {
        // only update some of the lru counters (ones that are above)
        
    }

    for (int i = 0; i < E; i++) {
        if ()
    }
    miss_count++;

    cache[S][E].lru++;
}

void traceFile(char* traceFile) {
    FILE* fp;
    fp = fopen(tracefile, "r");
    uint64_t address;
    char trace_line;
    int size;
    if (fp != NULL) {
        while (fscanf(fp, "%c %jd,%d", &trace_line, &address, &size) == 3) {
            switch(trace_line) {
                case 'I':
                    break;
                case 'L': 
                    break;
                case 'S':
                    break;
                case 'M':
                    break;
            }
        }
    }

    
}

int main(int argc, char *argv[])
{
    //ARGUMENT CHECKS
    char opt;
    
    //initial check
    if ((s==0) || (E==0) || (b==0) || (tracefile==NULL)) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printOptions(argv);
        exit(0);
    }
    //all arg check
    while((opt = getopt(argc, argv, "s:E:b:t:vh")) != -1) { 
        switch(opt) 
        { 
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
                tracefile = optarg;
                break; 
            case 'v': 
                verbose = 1;
                break; 
                
            case 'h': 
                printOptions(argv);
                exit(0);
                break; 
                
            default: 
                printOptions(argv);
                exit(1);
        } 
    }

    S = pow(2,s);
    B = pow(2,b);
    block_t cache[S][E];
    //initialize cache
    initCache(cache);
    
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
