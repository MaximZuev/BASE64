#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "reader.h"

static void help(){
    printf("You are using Base-64 encoder/decoder.\n");
    printf("To encode your file use pattern \"-e \'input file name\' \'output file name\'\".\n");
    printf("To decode your file use pattern \"-d \'input file name\' \'output file name\'\".\n");
    printf("You can use option \'-f N\' to encode with line break after every N symbols.\n");
    printf("You can use option \'-i\' to ignore characters not from the Base64 alphabet.");
}
int readinput(int argc, char **argv, char **in, char **out, int **options, FILE **outfile, FILE **infile ){
    int opt;
    static const char *short_opts = "f:edi";
    static const struct option long_opts[] = {
            {"help", no_argument, NULL, 'h'},
            {"encode", no_argument, NULL, 'e'},
            {"decode", no_argument, NULL, 'd'},
            {"ignore", no_argument, NULL, 'i'},
            {"linebreak", required_argument, NULL, 'f'},
            {NULL, no_argument, NULL, 0}
    };
    if (argc == 1) {
        printf("Hey, use \'--help\' for help.");
        return 0;
    }
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                (*options)[0] = 1;
                break;
            case 'e':
                (*options)[1] = 1;
                break;
            case 'd':
                (*options)[2] = 1;
                break;
            case 'i':
                (*options)[3] = 1;
                break;
            case 'f':
                (*options)[4] = (int) strtol(optarg, NULL, 10);
                break;
            default:
                break;
        }
    }
    if ((*options)[0] == 1) {
        help();
        return 0;
    }
    if ((*options)[1] && (*options)[2]){
        printf("Error with -e and -d.");
        return 0;
    }
    if ((*options)[1] && (*options)[3]){
        printf("Error with -e and -i.");
        return 0;
    }
    if ((*options)[2] && (*options)[4]){
        printf("Error with -d and -f.");
        return 0;
    }
    if (argc > 3) {
        (*in) = argv[argc - 2];
        (*out) = argv[argc - 1];
        if (((*infile) = fopen((*in), "rb")) == NULL) {
            printf("Can't open file: %s\nPlease write \'--help\'.", (*in));
            return 0;
        }
        if (((*outfile) = fopen((*out), "wb")) == NULL) {
            printf("Can't open file: %s\nPlease write \'--help\'.", (*out));
            return 0;
        }
    }
    if (!(*in) || !(*out)) {
        printf("Error with opening files, please use \'--help\'.");
        return 0;
    }
    return 1;
}
