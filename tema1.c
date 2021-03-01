#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include<string.h>
#include<stdlib.h>

# define lenght 32
# define byte 8

// se construieste un vector de caractere ce retine numarul convertit in baza 2
void binary(int nr, int s[lenght]){
	int i, r;
	i = lenght - 1;

	while(nr != 0){
		r = nr % 2;
		s[i] = r;
		i--;
		nr = nr / 2;
	}
}

// realizeaza complementul fata de 2
void complement(int s[lenght]){
	int i;

// se neaga bitii
	for(i = 0; i < lenght; i++){
		if(s[i] == 0){
			s[i] = 1;
		}

		else s[i] = 0;
	}

// se aduna 1
	for(i = lenght - 1; i >= 0; i--){
		if(s[i] == 1){
			s[i] = 0;
		}
		else{
			s[i] = 1;
			break;
		}
	}
}

// converteste un vector de caractere reprezentand
// un numar binar intr-un numar zecimal

long long decimal(int s[lenght]){
	int i, power;
	long long nr;

	nr = 0;
	power = 1;

	for(i = lenght - 1; i >= 0; i--){
		if(s[i] == 1){
		nr = nr + power;
		}

		power = power * 2;
	}

	return nr;
}

// converteste un vector de caractere cu reprezentarea in binar
// a unui numar intr-un vector ce retine reprezentarea in baza 16

void hexa(int s[lenght], char c[byte]){
	int i, j, k, nr, power;
	int bit[4];
	j = 0;

// se parcurge vectorul din 4 in 4 pozitii
// se memoreaza intr-un alt vector fiecare grupare de 4 octeti
// si din acesta se construieste un numar in baza 10
// se converteste numarul format in baza 16
	for(i = 0; i < lenght - 1; i = i + 4){
		for(k = 0; k < 4; k++){
			bit[k] = s[i + k];
		}

		nr = 0;
		power = 1;
	
		for(k = 3; k >= 0; k--){
			if(bit[k] == 1){
				nr = nr + power;
			}
			power = power * 2;
		}

		if(nr < 10){
			c[j] = nr + '0';
			j++;
		}
		else{
			c[j]='a' + (nr - 10);
			j++;
		}
	}
}

 // se retine un numar intr-un vector de caractere
int convert_to_string(long long nr, char s[lenght], int sign){
	int nr_digits, i;
	long int power;
	long long copy = nr;
	nr_digits = 0;
	power = 1;

	// calculeaza numarul de zecimale
	while(copy != 0){
		copy = copy / 10;
		nr_digits++;
		power = power * 10;
	}
	
	power = power / 10;
	
	// se imparte numarul la puterile lui 10
	// se retine fiecare cifra intr-un vector de caractere
	for(i = sign; i < nr_digits - 1  + sign; i++){
		s[i] = nr / power  + '0';
		nr = nr % power;
		power = power / 10;
	}
	s[i] = nr + '0';
	return nr_digits;
}

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

int iocla_printf(const char *format, ...)
{
	int i, j, k, l, nr_bytes, nr_digits;
	va_list args;
	va_start(args, format);

 	i = 0;
 	nr_bytes = 0;

	while(i < strlen(format)){
		if(format[i] == '%'){
			i++;
			// se printeaza caracterul "%" retinut intr-un vector de caractere
			if(format[i] == '%'){
				char s[1];
				s[0] = format[i];
				nr_bytes += write_stdout(s, 1);
			}

			// se citeste un caracter si se memoreaza intr-un vector de caractere
			if(format[i] == 'c'){
				char ch = va_arg(args,int);
				char character[1];
				character[0] = ch;
				 nr_bytes += write_stdout(character, 1);
			}

			// pentru un int
			else if(format[i] == 'd'){
				long long nr = va_arg(args,int);
				char *s = calloc(16, sizeof(char));
				int sign;

				nr_digits = 0;
				sign = 0;

				// pentru un numar negativ se retine semnul -
				if(nr < 0){
					s[0] = '-';
					sign = 1;
					nr = nr * (-1);
				} 

				nr_digits = convert_to_string(nr, s, sign);
				nr_bytes += write_stdout(s, nr_digits + sign);
				free(s);
			}

			// daca este un unsigned int
			else if(format[i] == 'u'){
				int sign, nr_digits;
				int nr = va_arg(args,int);
				unsigned int  nr1;
				char *s = calloc(16, sizeof(char));

				// daca numarul este negativ se va realiza complementul fata de 2
				// acest numar va fi cel retinut intr-un sir de caractere si
				// se va construi un nou numar intreg
				if(nr < 0){
					nr1 = (-1) * nr;
					int *v = calloc(32, sizeof(int));
					binary(nr1, v);
					complement(v);
					nr1 = decimal(v);
				}

				else if(nr > 0){
					nr1 = nr;
				}

				sign = 0;
				// numarul final va fi memorat intr-un vector de caractere
				nr_digits = convert_to_string(nr1, s, sign);
				nr_bytes += write_stdout(s, nr_digits);
				free(s);
			}

			// daca se primeste un sir de caractere se printeaza direct
			else if(format[i] == 's'){
				 char *string = va_arg(args,char *);
				 nr_bytes += write_stdout(string, strlen(string));
			
			}

			// daca se doreste printarea unui numar in baza 16
			else if(format[i] == 'x'){
				int sign = 0;
				int nr = va_arg(args,int);
				int *base2 = calloc(lenght, sizeof(int));
				char *base16 = calloc(lenght, sizeof(char));

				// daca numarul este negativ se realizeaza complementul fata de 2
				// se transforma in baza 16
				if(nr < 0){
					sign = -1;
					nr = (-1) * nr;
				}

				binary(nr, base2);

				if(sign == -1){
					complement(base2);
				}

				hexa(base2, base16);
				
				k = 0;

				// daca transformat in baza 16 vectorul de caractere
				// contine cifre de 0 la inceput se va contrui un nou vector 
				while(base16[k] == '0'){
						k++;
				}

				char *final =  calloc(byte - k, sizeof(char));
				l = 0;

				for(j = k; j < byte; j++){
					final[l] = base16[j];
					l++;
				}

				nr_bytes += write_stdout(final, strlen(final));
				free(base2);
				free(base16);
				free(final);
			}
		}
	
		else{
			char string[1];
			string[0] = format[i];
			nr_bytes += write_stdout(string, 1);
		}
		i++;
	}
	va_end(args);
			
	return nr_bytes;
}
