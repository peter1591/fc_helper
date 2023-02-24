all: proto
	ng build

proto:
	npm run proto:generate

dist: all
	rm -rf /var/www/html/*
	cp -R dist/web/* /var/www/html/

.PHONY: dist proto
