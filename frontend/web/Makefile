all:
	npm run proto:generate
	ng build

dist:
	rm -rf /var/www/html/*
	cp -R dist/web/* /var/www/html/

.PHONY: dist
