#!/usr/bin/env bash
set -e

#----------------------------------------------
# Checkout, build and install USD
#----------------------------------------------

cd $TMP_DIR_STUDIOEXAMPLE &&\
	pwd &&\
    rm -Rf build &&\
    mkdir build &&\
    cd build  &&\
      cmake -d .. \
            -Wno-dev \
            -DCMAKE_INSTALL_PREFIX="$BUILD_DIR" \
            -DCMAKE_MODULE_PATH="$BUILD_DIR" \
            -DCMAKE_BUILD_TYPE=RelWithDebInfo \
            -DMAYA_LOCATION=$MAYA_LOCATION \
            -DUSD_SCHEMA_PATH=/opt/usd/share/usd/plugins/usd/resources/schema.usda
            
            make -j ${BUILD_PROCS} install

rm -rf $TMP_DIR
