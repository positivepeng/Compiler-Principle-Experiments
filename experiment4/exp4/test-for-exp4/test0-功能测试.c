

int main(){
	int a, b, flag;
	a = read();
	b = read();

	if(a < b)
		flag = 1;
	else
		flag = 0;
	
	if(a == b && flag == 1)
		write(1);

	while(a == b)
		a = a - 1;

	write(a);
	write(b);

	return 0;
}