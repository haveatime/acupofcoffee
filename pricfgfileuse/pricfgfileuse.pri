infile(pricfgfileuse.cfg, IS_SOME_THING_YES, yes) {
IS_SOME_THING_YES = yes
} else {
IS_SOME_THING_YES = no
}

!build_pass:message(IS_SOME_THING_YES=$${IS_SOME_THING_YES})
