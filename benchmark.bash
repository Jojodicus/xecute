#! /bin/bash

# ripped from RootDO

$1 whoami

current=$(date +%s.%N)
for i in {1..1000}; do
	$1 whoami 2>&1 >/dev/null
done
done=$(date +%s.%N)

echo $done - $current | bc