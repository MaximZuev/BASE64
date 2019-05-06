#include <stdio.h>

#include "encoder.h"

static const char b64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void encodeblock(unsigned char *in, unsigned char *out, int len) {
    out[0] = (unsigned char) b64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) b64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? b64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ (int)(in[2] & 0x3f) ] : '=');
}

void encode(FILE *infile, FILE *outfile, int linesize){
    unsigned char in[3], out[4];
    int i, len = 0, sizecounter = 0;
    while ( feof(infile) == 0 ){
        len = 0;
        for ( i = 0; i < 3; i++ ){
            in[i] = (unsigned char) getc(infile);
            if ( feof(infile) == 0 ){
                len++;
            } else {
                in[i] = (unsigned char)0;
            }
        }
        if ( len > 0 ){
            encodeblock(in, out, len);
            for ( i = 0; i < 4; i++ ){
                if ((sizecounter == linesize) && (linesize != 0)){
                    putc('\n', outfile);
                    sizecounter = 0;
                }
                putc(out[i], outfile);
                sizecounter++;
            }
        }
    }
}
