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


| tag  | type  | def  | 
|---|---|---|
| XB  | I  | The beginning of the alignment  |
| XE  | I  | The end of the alignment   |
| XP  | f  | The percent identity of the alignment (length - NM - clipping)  |
| XL  | I  | The length of the alignment block  |
