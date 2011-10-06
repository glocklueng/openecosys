#!/bin/sh

NETWORKVIEWER_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/NetworkViewer//'`"

echo "NetworkViewer running from  : $PWD   Bundle : $NETWORKVIEWER_BUNDLE"

export DYLD_LIBRARY_PATH=$NETWORKVIEWER_BUNDLE/Contents/Resources/lib
export PATH=$NETWORKVIEWER_BUNDLE/Contents/Resources/bin:$PATH

#run
exec $NETWORKVIEWER_BUNDLE/Contents/Resources/bin/NetworkViewer

