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
//   Colby Weber    Net-ID: cweber7
//////////////////////////////////////////

int verbose = 0;
int s = 0;
int E = 0;
int b = 0;
int v = 0;
int h = 0;
int t = 0;
char* tracefile = 0;
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
    int valid;
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


void cacheAccess(block_t cache[S][E], unsigned long long address, int s, int b, char trace_line, unsigned long long instr_count) {
    unsigned long long set = (address << ((sizeof(address)*8 - s - b))) >> ((sizeof(address)*8 - s));
    uint64_t tag = address >> (s + b);
    unsigned long long block_2_evict = 0;
    uint64_t evict_lru_maximum = 0;

    for (int i = 0; i < E; i++) {
        //printf("%jd ", cache[set][i].lru);
        //printf("          ");
    }
    printf("address: %llx     set: %llx    s: %d     b: %d      ", address, set, s,b);
    /*
    if (trace_line == 'M') {
        hit_count++;
        for (int j = 0; j < E; j++) {
               cache[set][j].lru++;
        }
    }
    */

    //check for hit, if hit set LRU counter to instr_count and return
    for (int i = 0; i < E; i++) {
        if (cache[set][i].valid && (cache[set][i].tag == tag)) {
            cache[set][i].lru = instr_count;
            cache[set][i].tag = tag;
            hit_count++;
            for (int i = 0; i < E; i++) {
                printf("%ju ", cache[set][i].lru);
                printf("      ");
            }
            for (int i = 0; i < E; i++) {
                printf("%x ", cache[set][i].valid);
                printf(" ");
            }
            printf("HIT\n");
            for (int j = 0; j < E; j++) {
                cache[set][j].lru++;
            }
            return;
        }
    }
    

    evict_lru_maximum = cache[set][0].lru;
    for (int i = 0; i < E; i++) {
        // only update some of the lru counters (ones that are above)      
        if (cache[set][i].lru < evict_lru_maximum) {
            evict_lru_maximum = cache[set][i].lru;
            block_2_evict = i;
        }
    }

    for (int i = 0; i < E; i++) {
        printf("%ju ", cache[set][i].lru);
        printf("      ");
    }

    for (int i = 0; i < E; i++) {
        printf("%x ", cache[set][i].valid);
        printf(" ");
    }
    
    
    //find block to evict in set
    if (cache[set][block_2_evict].valid) {
        printf("BLOCK_2_EVICT: %llx\n", block_2_evict);
        eviction_count++;
    }

    //printf("MISS\n");
    for (int i = 0; i < E; i++) {
        cache[set][i].lru++;
    }

    miss_count++;

    cache[set][block_2_evict].lru = instr_count;
    cache[set][block_2_evict].tag = tag;
    cache[set][block_2_evict].valid = 1;
    printf("\n");
}

void traceFile(char* traceFile) {

    
}

int main(int argc, char *argv[])
{
    //ARGUMENT CHECKS
    char opt;
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
    FILE* fp;
    fp = fopen(tracefile, "r");
    unsigned long long address;
    unsigned long long instr_count = 0;;
    char trace_line;
    int size;

    while (fscanf(fp, " %c %llx,%d", &trace_line, &address, &size) == 3) {

        
        switch(trace_line) {
            case 'I':
                //do nothing    
                break;
            case 'L': 
                instr_count++;
                cacheAccess(cache, address, s, b, trace_line, instr_count);
                //printf("L");
                break;
            case 'S':
                instr_count++;
                cacheAccess(cache, address, s, b, trace_line, instr_count);
                //printf("S");
                break;
            case 'M':
                instr_count++;
                cacheAccess(cache, address, s, b, trace_line, instr_count);
                instr_count++;
                cacheAccess(cache, address, s, b, trace_line, instr_count);
                //printf("M");
                break;
            default:
                break;
        }
    }
    fclose(fp);
    

    
    traceFile(tracefile);
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
