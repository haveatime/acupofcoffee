#
# DEFINE A TEST FUNCTION
#
defineTest(isEnvExist) {
	ENVNAME = $$1
	unix {
		ENVVALUE = $$system(echo ${$${ENVNAME}})
	} else {
		ENVVALUE = $$system(if defined $${ENVNAME} echo %$${ENVNAME}%)
	}

#	message(ENVVALUE=$$ENVVALUE)

	isEmpty(ENVVALUE) {
		return(false)
	} else {
		return(true)
	}
}

#
# DEFINE A REPLACE FUNCTION
#
defineReplace(addPrefix) {
	ENVNAME = $$1
	PREFIX = $$2
	unix {
		ENVVALUE = $$system(echo ${$${ENVNAME}})
	} else {
		ENVVALUE = $$system(if defined $${ENVNAME} echo %$${ENVNAME}%)
	}

#	message(ENVVALUE=$$ENVVALUE)

	return($$PREFIX$$ENVVALUE)
}

