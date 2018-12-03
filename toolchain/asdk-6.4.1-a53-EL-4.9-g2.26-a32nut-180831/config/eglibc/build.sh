#!/bin/bash
#
# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# Dec. 7, 2010
#

NUM_ARGS=6

if [ $# -ne $NUM_ARGS ]
then
    echo "usage: $0 function sdk target dir_build dir_eglibc dir_stage"
    exit 1
fi

FUNCTION=$1; shift
SDK=$1; shift
TARGET_NAME=$1; shift
DIR_BUILD=$1; shift
DIR_GLIBC=$1; shift
DIR_STAGE=$1

case "$SDK" in
"asdk" | "asdk64")
	CONFIG_SPECIFIC=""
	;;
"msdk")
	CONFIG_SPECIFIC="--with-mips-plt"
	;;
"rsdk")
	CONFIG_SPECIFIC="--with-mips-plt"
	;;
"*")
	echo "unknown processor"
	exit 1
	;;
esac

# auto-detect build name
HOST_NAME=$(uname | tr '[:upper:]' '[:lower:]' \
               | sed -e 's,\(linux\|darwin\|cygwin\).*,\1,')
HOST_ARCH=$(uname -m)

if [ x${HOST_NAME} = x'darwin' ]; then
	HOST_TYPE=apple
else
	HOST_TYPE=pc
fi

BUILD_NAME=${HOST_ARCH}-${HOST_TYPE}-${HOST_NAME}

TARGET_CROSS=${TARGET_NAME}-
TARGET_CC=${TARGET_CROSS}gcc
TARGET_OPT="-Os -pipe -funit-at-a-time"

##
## eglibc_make
##
eglibc_make()
{
  multi_cfg=$1
  multi_dir=$2

  TARGET_CFLAGS="$multi_cfg"
  TARGET_LIBPATH="$multi_dir"
  TARGET_OBJPATH="$( echo "$multi_cfg" | sed -r -e 's/[ =]/-/g' )"

  with_fp="true"
  (echo | ${TARGET_CC} ${TARGET_CFLAGS} -dM -E - | grep "\(__mips_soft_float\|__SOFTFP__\)" 2>&1 > /dev/null) && with_fp="false"
  if [ x${with_fp} = x'false' ]; then
      CONFIG_FLOAT="--without-fp"
  else
      CONFIG_FLOAT="--with-fp"
  fi

  BUILD_CC="gcc"
  AR="${TARGET_CROSS}ar"
  AS="${TARGET_CROSS}gcc -c ${TARGET_CFLAGS}"
  LD="${TARGET_CROSS}ld"
  NM="${TARGET_CROSS}nm"
  CPP="${TARGET_CROSS}cpp ${TARGET_CFLAGS}"
  CC="${TARGET_CROSS}gcc ${TARGET_CFLAGS}"
  CXX="${TARGET_CROSS}g++ ${TARGET_CFLAGS}"
  RANLIB="${TARGET_CROSS}ranlib"
  CFLAGS="${TARGET_OPT}"
  export BUILD_CC AR AS LD NM CC CPP CXX RANLIB CFLAGS

  DIR_SUMP=${DIR_STAGE}/eglibc-${FUNCTION}
  if [ x${TARGET_OBJPATH} != x ]; then
      DIR_SUMP=${DIR_SUMP}-${TARGET_OBJPATH}
  fi

  mkdir -p ${DIR_SUMP}/libc
  #cp ${DIR_GLIBC}/option-groups.defaults ${DIR_SUMP}/option-groups.config

  case "${FUNCTION}" in
  "prepare")
    CONFIG_PREFIX=/usr
    ;;
  "install")
    CONFIG_PREFIX=
    ;;
  esac

  (cd ${DIR_SUMP};					\
	libc_cv_slibdir="/lib"				\
	use_ldconfig=no					\
	${DIR_GLIBC}/configure				\
		--prefix=${CONFIG_PREFIX}		\
		--build=${BUILD_NAME}			\
		--host=${TARGET_NAME}			\
		--with-headers=${DIR_STAGE}/sysroot/usr/include \
		--disable-werror			\
		--disable-profile			\
		MAKEINFO=missing			\
		${CONFIG_FLOAT}				\
		${CONFIG_SPECIFIC}			\
		--without-gd				\
		--without-cvs				\
		--enable-add-ons			\
  )

  case "${FUNCTION}" in
  "prepare")
    if [ ! -f ${DIR_STAGE}/.eglibc-bootstrap ]; then
	make -C ${DIR_SUMP}				\
		install-headers				\
		DESTDIR=${DIR_STAGE}/sysroot		\
		install-bootstrap-headers=yes || exit
	touch ${DIR_STAGE}/.eglibc-bootstrap
    fi

    (cd ${DIR_SUMP};							\
        mkdir -p ${DIR_STAGE}/sysroot/usr/lib/${TARGET_LIBPATH};	\
        make csu/subdir_lib;						\
        cp csu/crt*.o ${DIR_STAGE}/sysroot/usr/lib/${TARGET_LIBPATH};	\
        ${CC} ${CFLAGS} -nostdlib -shared -x c /dev/null		\
            -o ${DIR_STAGE}/sysroot/usr/lib/${TARGET_LIBPATH}/libc.so	\
    )

    cp -f ${DIR_GLIBC}/include/gnu/stubs.h ${DIR_STAGE}/sysroot/usr/include/gnu
    ;;
  "install")
    make -C ${DIR_SUMP} all || exit
    make -C ${DIR_SUMP} install DESTDIR=${DIR_STAGE}/tmpfs

    mkdir -p ${DIR_BUILD}/lib/${TARGET_LIBPATH}
    cp -a ${DIR_STAGE}/tmpfs/lib/* ${DIR_BUILD}/lib/${TARGET_LIBPATH}
    if [ ! -f ${DIR_STAGE}/.eglibc-headered ]; then
	  cp -a ${DIR_STAGE}/tmpfs/include/* ${DIR_BUILD}/include
    else
	  cp -a ${DIR_STAGE}/tmpfs/include/gnu/stubs-*.h \
		${DIR_BUILD}/include/gnu
    fi
    rm -rf ${DIR_STAGE}/tmpfs
    touch ${DIR_STAGE}/.eglibc-headered

    (cd ${DIR_BUILD}/lib/${TARGET_LIBPATH};		\
        for f in libc.so libpthread.so libgcc_s.so ; do \
            if [ -f $f ] ; then				\
                sed -i -e 's,/lib/,,g' $f ;		\
            fi						\
        done						\
    )
    ;;
  esac
}

##
## config
##
eglibc_init()
{
  mkdir -p ${DIR_STAGE}/sysroot/usr
  mkdir -p ${DIR_STAGE}/sysroot/usr/lib

  case "${FUNCTION}" in
  "prepare")
    mkdir -p ${DIR_STAGE}/sysroot/usr/include
    cp -a ${DIR_GLIBC}/../kernel-${SDK}/* ${DIR_STAGE}/sysroot/usr/include
    ;;
  "install")
    mkdir -p ${DIR_BUILD}/include
    cp -a ${DIR_GLIBC}/../kernel-${SDK}/* ${DIR_BUILD}/include
    ;;
  esac
}

#.;
#tls;@fuse-tls
#armv7;@march=armv7-a
#armv7/tls;@march=armv7-a@fuse-tls

##
## compile and install
##
eglibc_init

multilibs=( $(${TARGET_CROSS}gcc -print-multi-lib) )

for multilib in "${multilibs[@]}"; do
    multi_cfg="$( echo "${multilib#*;}" | sed -r -e 's/@/ -/g;' )"
    multi_dir="${multilib%%;*}"
    eglibc_make "$multi_cfg" "$multi_dir"
done
