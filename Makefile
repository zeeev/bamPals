
HTSROOT=/net/eichler/vol8/home/zevk/tools/htslib/

BIN=bin/enrich_optional_tags

.PHONY: clean

$(BIN) : bin/% : src/%.c
	gcc -O3 -I $(HTSROOT) -Wall $< $(HTSROOT)/libhts.a -o $@ -lbz2 -llzma -lz -lm -lpthread

clean:
	rm bin/*