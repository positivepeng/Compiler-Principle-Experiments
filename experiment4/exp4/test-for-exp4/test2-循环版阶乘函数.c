int main()
{
	int n, ans;
	n = read();
	ans = 1;
	while(n > 0){
		ans = ans * n;
		n = n - 1;
	}
	write(ans);
	return 0;
}