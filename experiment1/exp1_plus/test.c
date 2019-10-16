int main(int argc, char** argv){
	/*
		Comment line,
		Comment line again.
	*/

	// undefined character
	int j = a && b;
	int j = a | b;
	int j = ~i;

	// octal number
	int a = 0;
	int a = 043;
	int a = 099;

	// hexadecima number
	int b = 0x23;
	int xF33 = 0xe33;
	int _correct_dhex_number = 0xF598;
	int _wrong_dhex_number_ = 0xG598;
	int _wrong_dhex_number2_ = 0xXF598;
	int _correct_not_dhex_number3 = xF33;
	int b = 0X9Z;

	// floating point number
	float x4 = 34.e.3;
	float x1 = 34.e3.;
	float x2 = 34.e-1.2;
	float a = 23.43E+3;
	float b = 23.43E+.3;
	float c = .e34;
	float f = 3.2e-3; 
	float f = 423.;
	float f = .23e10;
}