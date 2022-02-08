CLIENT_HOME=../duino-klutch-client

build:
	pio run

upload:
	pio run --target upload

uploadfs:
	pio run --target uploadfs

copyclient:
	rm data/index.html -f
	rm data/static -rdf
	cd $(CLIENT_HOME) && yarn run build_nohash
	cp $(CLIENT_HOME)/dist/index.html data/
	cp $(CLIENT_HOME)/dist/static data/ -R

apidocs:
	node apidoc/apidoc.js
