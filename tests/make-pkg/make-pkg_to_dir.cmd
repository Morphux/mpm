tar xf tests/misc/test_package.tar && ./mpm make-pkg right -d /tmp && cat /tmp/test-2.0.mpx | hexdump -C && rm -rf right && rm -f /tmp/test-2.0.mpx
