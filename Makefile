#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=gcc
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -Werror
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -Werror
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.0.0.0\"
INCPATH=-I. \
  -I./include \
  -I../include/
DEP_INCPATH=

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=ded5ee3145335824ad99a5c1517249f3  COMAKE


.PHONY:all
all:comake2_makefile_check base .copy 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf base
	rm -rf ./output/bin/base
	rm -rf .copy
	rm -rf output
	rm -rf base_base.o
	rm -rf base_dict.o
	rm -rf base_strdict.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

base:base_base.o \
  base_dict.o \
  base_strdict.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mbase[0m']"
	$(CXX) base_base.o \
  base_dict.o \
  base_strdict.o -Xlinker "-("    -Xlinker "-)" -o base
	mkdir -p ./output/bin
	cp -f --link base ./output/bin

.copy:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40m.copy[0m']"
	mkdir -p output

base_base.o:base.c \
  dict.h \
  strdict.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mbase_base.o[0m']"
	$(CC) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CFLAGS)  -o base_base.o base.c

base_dict.o:dict.c \
  dict.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mbase_dict.o[0m']"
	$(CC) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CFLAGS)  -o base_dict.o dict.c

base_strdict.o:strdict.c \
  strdict.h \
  dict.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mbase_strdict.o[0m']"
	$(CC) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CFLAGS)  -o base_strdict.o strdict.c

endif #ifeq ($(shell uname -m),x86_64)


