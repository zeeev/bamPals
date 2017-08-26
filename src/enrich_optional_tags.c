#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <assert.h>

#include "htslib/sam.h"
#include "htslib/hts.h"


struct counter{
    uint32_t t_consumed;
    uint32_t q_sclipped;
}global_count;

int target_consumed(uint32_t *cigar, uint32_t ncigar){
    uint32_t k = 0;
    for (; k < ncigar; k++)
        {
            uint32_t opLen = bam_cigar_oplen(cigar[k]);
            char opCode = bam_cigar_opchr(cigar[k]);

            switch(opCode)
                {
                case 'M':
                    {
                        global_count.t_consumed += opLen;
                        break;
                    }
                case 'D':
                    {
                        global_count.t_consumed += opLen;
                        break;
                    }
                case 'S':
                    {
                        global_count.q_sclipped += opLen;
                        break;
                    }
                default:
                    break;
                }

        }
    return 0;
}

int main(int argc, char **argv){

    if(argc != 2){
        fprintf(stderr, "USAGE: enrichTags input.[sam|bam|cram] output.bam\n");
        exit(1);
    }

    char * fn_in  = argv[1];
    char * fn_out = argv[2];

    htsFile * h = hts_open(fn_in, "r");

    const htsFormat *fmt = hts_get_format(h);

    int hts_close(htsFile *fp);

    fprintf(stderr, "INFO: detected %s filetype\n", hts_format_file_extension(fmt));

    samFile *in = 0;
    bam_hdr_t *header = NULL;

    if((in = sam_open_format(fn_in, "r", fmt)) == 0){
        printf("FATAL: failed to open \"%s\" for reading\n", fn_in);
        return 1;
    }

    if ((header = sam_hdr_read(in)) == 0) {
        fprintf(stderr, "FATAL: failed to read the header \"%s\" \n", fn_in);
        return 1;
    }

    samFile *output = sam_open(fn_out, "wb");

    sam_hdr_write(output, header);


    bam1_t * read1 = bam_init1();
    fprintf(stderr, "Reading: %s\n", fn_in);

    while(sam_read1(in, header, read1) >= 0){
        global_count.t_consumed = 0;
        global_count.q_sclipped = 0;
        if(read1->core.flag & 4){
            sam_write1(output, header, read1);
        }
        else{
            target_consumed(bam_get_cigar(read1), read1->core.n_cigar);
            uint8_t *nm1 = bam_aux_get(read1, "NM");
            int e_d = bam_aux2i(nm1);
            uint32_t end = read1->core.pos + global_count.t_consumed;
            int v = bam_aux_append(read1, "XB", 'I', sizeof(uint32_t), (uint8_t*) &read1->core.pos );
            assert(v == 0);
            v = bam_aux_append(read1, "XE", 'I', sizeof(uint32_t), (uint8_t*) &end);
            assert(v == 0);

            float pi = (float)(read1->core.l_qseq - global_count.q_sclipped - e_d) ;
            pi /= (float)(read1->core.l_qseq - global_count.q_sclipped);
            pi *= 100.0;
            v = bam_aux_append(read1, "XP", 'f', sizeof(float), (uint8_t*) &pi);
            assert(v == 0);
            uint32_t len = end - read1->core.pos;

            v = bam_aux_append(read1, "XL", 'I', sizeof(uint32_t), (uint8_t*)&len);


            assert(v == 0);
            sam_write1(output, header, read1);
        }


    }


    bam_destroy1(read1);
    bam_hdr_destroy(header);
    sam_close(output);



    return 0;
}
