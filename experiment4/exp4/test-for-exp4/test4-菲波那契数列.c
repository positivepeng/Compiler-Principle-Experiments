int main(){
	int n, a, b, c, temp;
	n = read();
	a = 1;
	b = 1;
	c = 2;
	if(n >= 1){
		write(a);
	}
	if(n >= 2){
		write(b);
	}
	if(n >= 3){
		write(c);
		while(n > 3){
			n = n - 1;
			temp = b+c;
			a = b;
			b = c;
			c = temp;
			write(c);
		}
	}
	return 0;
}