#!/usr/bin/env python

import pwd, grp
for p in pwd.getpwall():
    print p[0]
