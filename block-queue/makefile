bin/test: *.c
	mkdir -p ./bin
	cc $^ -lpthread -o ./$@

.PHONY: clean
clean:
	rm -rf ./bin ./lib

.PHONY: run
run:
	./bin/test

.PHONY: build
build: *.c
	mkdir -p ./lib
	cc $^ -shared -fPIC -lpthread -o ./lib/libblockqueue.so