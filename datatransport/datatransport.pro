TEMPLATE = subdirs

SUBDIRS +=  \
	  datatransportinterface \
	  datatransportwraper \
	  datatransportsample \
	  dtmulticastplugin \
	  dtudpclientplugin \
	  dttcpclientplugin \
	  dttcpserverplugin \
	  datatransportservice

greaterThan(QT_MAJOR_VERSION,4) : {
SUBDIRS += dtserialportplugin
}

contains(QT_CONFIG,openssl):{
SUBDIRS +=  \
	  dtsslserverplugin \
	  dtsslclientplugin
}
