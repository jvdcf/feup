PORT=8080
FILE=../users.csv

all:


server-mock:
	gradle run --args="server-mock ${PORT}"

client:
	gradle run --args="client localhost ${PORT}"

server:
	gradle run --args="server ${PORT} ${FILE}"
