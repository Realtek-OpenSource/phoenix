#!/bin/bash
#
# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# Dec. 7, 2010
#

if [ $# -lt 3 ]; then
    echo "usage: $0 dir_wrapper cpu_type cflags"
    exit 1
fi

DIR_WRAPPER=$1; shift
CPU_TYPE=$1; shift
TARGET_CFLAGS=$*

DIR_LIBC=${DIR_WRAPPER}/config/eglibc
DIR_SUMP=${DIR_WRAPPER}/sump
DIR_TMPFS=${DIR_WRAPPER}/tmpfs

if [ x${CPU_TYPE} = x'arm64' ]; then
	CONFIG_ARCH=aarch64
	CONFIG_SPECIFIC=""
        TARGET_CROSS=aarch64-linux-
elif [ x${CPU_TYPE} = x'arm' ]; then
	CONFIG_ARCH=arm
	CONFIG_SPECIFIC=""
        TARGET_CROSS=arm-linux-gnueabi-
elif [ x${CPU_TYPE} = x'mips' ] || [ x${CPU_TYPE} = x'rlx' ]; then
	CONFIG_ARCH=mips
	CONFIG_SPECIFIC="--with-mips-plt"
        TARGET_CROSS=mips-linux-
else
	echo "unsupported architecture"
	exit 1
fi

TARGET_NAME=`${DIR_WRAPPER}/bin/${TARGET_CROSS}gcc -dumpmachine`
DIR_SYSROOT="${DIR_WRAPPER}/${TARGET_NAME}"

with_fp="true"
(echo | ${TARGET_CROSS}gcc ${TARGET_CFLAGS} -dM -E - | grep "\(__mips_soft_float\|__SOFTFP__\)" 2>&1 > /dev/null) && with_fp="false"
if [ x${with_fp} = x'false' ]; then
  CONFIG_FLOAT="--without-fp"
else
  CONFIG_FLOAT="--with-fp"
fi

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
BUILD_CC="gcc"
AR="${TARGET_CROSS}ar"
AS="${TARGET_CROSS}gcc -c ${TARGET_CFLAGS}"
LD="${TARGET_CROSS}ld"
NM="${TARGET_CROSS}nm"
CPP="${TARGET_CROSS}cpp ${TARGET_CFLAGS}"
CC="${TARGET_CROSS}gcc ${TARGET_CFLAGS}"
CXX="${TARGET_CROSS}g++ ${TARGET_CFLAGS}"
RANLIB="${TARGET_CROSS}ranlib"
CFLAGS="-O2 -pipe -funit-at-a-time"
export BUILD_CC AR AS LD NM CC CPP CXX RANLIB CFLAGS

##
## config
##
mkdir -p ${DIR_SUMP}
#cp ${DIR_LIBC}/option-groups.defaults ${DIR_SUMP}/option-groups.config

cd ${DIR_SUMP}
libc_cv_slibdir="/lib"					\
use_ldconfig=no						\
${DIR_LIBC}/configure					\
	--prefix=					\
	--build=${BUILD_NAME}				\
	--host=${TARGET_NAME}				\
	--with-headers=${DIR_SYSROOT}/include		\
	--disable-werror				\
	--disable-profile				\
	${CONFIG_FLOAT}					\
	${CONFIG_SPECIFIC}				\
	--without-gd					\
	--without-cvs					\
	--enable-obsolete-rpc				\
	--enable-add-ons

##
## compile and install
##
make all || exit 1
make install DESTDIR=${DIR_TMPFS} || exit 1

for f in ${DIR_TMPFS}/lib/libc.so ${DIR_TMPFS}/lib/libpthread.so ; do
    if [ -f $$f -a ! -L $$f ] ; then
        sed -i -e 's,/lib/,,g' $$f ;
    fi
done

##
## postifx
##
cp ${DIR_SUMP}/nptl/libpthread_pic.a ${DIR_TMPFS}/lib/libpthread_pic.a
cp ${DIR_SUMP}/libpthread.map ${DIR_TMPFS}/lib/libpthread_pic.map
mkdir ${DIR_TMPFS}/lib/libpthread_pic
cp ${DIR_SUMP}/nptl/crt*.o ${DIR_TMPFS}/lib/libpthread_pic
cp -a ${DIR_TMPFS}/sbin ${DIR_WRAPPER}/usr
cp -a ${DIR_TMPFS}/share ${DIR_WRAPPER}/usr
