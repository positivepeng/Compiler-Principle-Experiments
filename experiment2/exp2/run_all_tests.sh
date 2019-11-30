DIR=./test
for FILE in ${DIR}/*.c; do
	echo "$FILE" 
	./scanner "$FILE"
done