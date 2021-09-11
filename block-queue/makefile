bin/test: *.c
	mkdir -p ./bin
	cc $^ -lpthread -o ./$@

.PHONY: clean
clean:
	rm -rf ./bin

.PHONY: run
run:
	./bin/test