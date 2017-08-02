tar xf tests/misc/test_package.tar && ./mpm make-pkg right -o some.mpx && cat some.mpx | hexdump -C && rm -rf right
