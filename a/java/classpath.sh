#!/bin/sh
TOMCAT=~/zeidon/apache-tomcat-6.0.18/lib

CLASSPATH=./class
for jar in `find ~/zeidon/apache-tomcat-6.0.18/lib -name '*.jar' -print | sed 's/ /:/g'`; do
  CLASSPATH=$CLASSPATH:$jar
done

export CLASSPATH
export LD_LIBRARY_PATH=~/zeidon/10d/x/bin:/usr/lib:/lib/tls/i686/cmov:/lib
export KZV=/home/dchristensen/zeidon/10c
export ZEIDON=/home/dchristensen/zeidon/zapp