DIR=./test-for-exp4
for FILE in ${DIR}/*.c; do
	echo "$FILE" 
	./parse "$FILE"
	echo ""
done