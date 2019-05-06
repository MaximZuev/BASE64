#include <stdio.h>

#include "decoder.h"

static void decodeblock(unsigned char *in, unsigned char *out) {
    out[0] = (unsigned char) ((in[0] << 2) | (in[1] >> 4));
    out[1] = (unsigned char) ((in[1] << 4) | (in[2] >> 2));
    out[2] = (unsigned char) ((in[2] << 6) | in[3]);
}

static unsigned char newbyte(unsigned char byte){
    if ( byte >= 65 && byte <= 90) return byte - 65;
    else if ( byte >= 97 && byte <= 122) return byte - 71;
    else if ( byte >= 48 && byte <= 57) return byte + 4;
    else if ( byte == 43 ) return 62;
    else if ( byte == 47 ) return 63;
    else if ( byte == 61 ) return 0;
    return -1;
}

int decode(FILE *infile, FILE *outfile, int opt){
    unsigned char in[4], out[3];
    int i, len = 0;
    while ( feof(infile) == 0 ){
        len = 0;
        for ( i = 0; i < 4; i++ ){
            in[i] = newbyte((unsigned char) getc(infile));
            if ( feof(infile) == 0 ){
                len++;
            } else {
                in[i] = (unsigned char)0;
            }
            if (in[i] == (unsigned char)-1){
                if(opt){
                    i--;
                } else {
                    printf("Error with input file, it isn't encoded in Base64");
                    return 0;
                }
            }
        }
        if ( (len % 4 != 0) && !opt ){
            printf("Error with input file, it isn't encoded in Base64");
            return 0;
        }
        if ( len > 0 ){
            decodeblock(in, out);
            for ( i = 0; i < 3; i++ ){
                if (len > 0){
                    putc(out[i], outfile);
                }
                len--;
            }
        }
    }
    return 1;
}
