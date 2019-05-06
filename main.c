#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "encoder.h"
#include "decoder.h"

int main(int argc, char **argv) {
    FILE *infile = NULL, *outfile = NULL;
    int *options = calloc(5, sizeof(int));
    char *in = NULL, *out = NULL;
    while (1){
        if(readinput(argc, argv, &in, &out, &options, &outfile, &infile)){
            if (options[1]) {
                encode(infile, outfile, options[4]);
                printf("File %s was encoded into file %s", in, out);
                break;
            }
            if (options[2]) {
                if (decode(infile, outfile, options[3])) printf("File %s was decoded into file %s", in, out);
                break;
            }
        }
        break;
    }
    free(options);
    if (infile) fclose(infile);
    if (outfile) fclose(outfile);
    return 0;
}
