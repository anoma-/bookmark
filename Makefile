OUT=book

default:
	gcc book.c -o $(OUT)

install:
	mv $(OUT) /usr/local/bin
	cp mk /usr/local/bin
	grep "function bk()" ~/.bashrc ||cat ./bashrc >> ~/.bashrc
	echo "You will need to restart a terminal session for changes to come into effect"

