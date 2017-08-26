# bamPals
An assortment of methods for bam|sam|cram files

## Dependencies
   htslib installed

## Install
   1. Clone the repository.
   2. Add the root HTSLIB path to the Makefile i.e.:
   ```
        HTSROOT=/net/eichler/vol8/home/zevk/tools/htslib/
   ```
   3. Run the make command.
## enrich_optional_tags

This program adds four additional optional tags to the bam file. 


| tag  | type  | definition  | 
|---|---|---|
| XB  | I (uint32_t) | The beginning of the alignment  |
| XE  | I (uint32_t) | The end of the alignment   |
| XP  | f (float) | The percent identity of the alignment (length - NM - clipping)  |
| XL  | I (Uint32_t) | The length of the alignment block  |
