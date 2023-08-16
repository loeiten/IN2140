CCFLAGS= -g

all: routing_server node

routing_server: <Insert all c-files which routing_server depends on>
	gcc <Insert all c-files which routing_server depends on> $(CCFLAGS) -o routing_server

node: <Insert all c-files which node depends on>
	gcc <Insert all c-files which node depends on> $(CCFLAGS) -o node


run: all
	bash run.sh

clean:
	rm -f routing_server
	rm -f node
