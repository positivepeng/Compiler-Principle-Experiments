DIR=./test
for FILE in ${DIR}/*.c; do
	echo "$FILE" 
	./parse "$FILE"
	echo ""
done