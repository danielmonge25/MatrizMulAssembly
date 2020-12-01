#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>

extern "C" void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p);

using namespace std;
vector <float> matrizUno;
vector <float> matrizDos;
vector <float> matrizOut;

void cargarMatriz(ifstream& archivo, int &x, int &y, vector <float>& matriz) {
	int size1 = 0;
	int size2 = 0;
	char mander = ',';
	float num = 0.0;
	float aux = 0.0;
	archivo >> size1;
	archivo >> mander;
	archivo >> size2;
	x = size1;
	y = size2;
	matriz.resize(size1 * size2, 0.0);
	for (int i = 0; i < size1 * size2; i++) {
		int j = 0;
		while (mander == ',' && j != size2) {

			archivo >> aux;
			if (size2 > j + 1) {
				archivo >> mander;
			}
			num = aux;
			matriz[i] = num;
			j++;
			i++;

		}
		i--;
	}
	archivo.close();
}

bool compare(float *matriz_Uno,float *matriz_Dos, unsigned int elementos) {

	for (int i = 0; i < elementos; i++) {

		if (matriz_Uno[i] != matriz_Dos[i]) {
			return false;
		}
		else {
		
		}
	}

	return true;
	
};

void transpuesta(vector <float>& matriz, int filas, int columnas) {
	int w = 0;
	int z = 0;

	float matrix[filas][columnas];   //<- Esto hay que cambiarlo a los parametros

	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {
			matrix[i][j] = matriz[w];
			w++;
		}
	}

	for (int i = 0; i < columnas; i++) {
		for (int j = 0; j < filas; j++) {
			matriz[z] = matrix[j][i];
			z++;
		}
	}

}

int main()
{
  int fila1 = 0;
  int fila2 = 0; 

  int columna1 = 0; 
  int columna2 = 0; 
  
  int fila3 = 0; 
  int columna3 = 0; 

	ifstream caso1;
	ifstream caso2;
	ifstream casoOut;
 
	/*
	*	Abrir archivos
	*/

	caso1.open("case0_matrix1.txt");			// <- Primera Matriz
	cargarMatriz(caso1, fila1, columna1, matrizUno);
	caso1.close();

	caso2.open("case0_matrix2.txt");			// <- Segunda Matriz
	cargarMatriz(caso2, fila2, columna2, matrizDos);
	caso2.close();

	casoOut.open("case0_output.txt");			// <- Matriz Resultado
	cargarMatriz(casoOut, fila3, columna3, matrizOut);
	casoOut.close();
  int filaT=fila1;
  transpuesta(matrizDos, fila2, columna2);

  float matriz1[fila1 * columna1]; 
	float matriz2[fila2 * columna2]; 
	float matriz3[(fila1 * columna1)*columna2];
  float resultado[fila3 * columna3]; 

	for (int i = 0; i < fila1 * columna1; i++) {
		matriz1[i] = matrizUno[i];
	}

	for (int i = 0; i < fila2 * columna2; i++) { 
		matriz2[i] = matrizDos[i];
	}

  for (int i = 0; i < fila3 * columna3; i++) { 
		resultado[i] = matrizOut[i];
	}

	quickMatrixMul(matriz1, matriz2, matriz3, fila1, columna1, columna2);
  /*for(int i=0;i<(fila1 * columna1)*columna2;i++)
      cout<<matriz3[i]<<",";*/
	float *output = new float[fila1*columna2];
  
	int x = 0;
	for (int i = 0; i <(fila1 * columna1)*columna2; i++) {
		float suma = 0.0;
		int tem = 0;
		for (int j = 0; j < columna1; j++) {
			suma = suma + matriz3[i + j];
			tem ++;
		}
    i=i+tem-1;
		output[x] = suma;
		x++;
	}
  /*
  * Salida a Consola
  */

std::cout << endl;

std::cout << "Output Nuestro = {";

	for (int i = 0; i <columna3*fila3; i++) {
		std::cout << std::setprecision(2) << std::fixed << output[i] << ", ";
	}
	std::cout << std::setprecision(2) << std::fixed << output[columna3*fila3 - 1] << "}\n";


  std::cout << endl;


  std::cout << "Resultado Real = {";

  for (int i = 0; i < columna3*fila3; i++) {
		std::cout << std::setprecision(2) << std::fixed << matrizOut[i] << ", ";
	}
	std::cout << std::setprecision(2) << std::fixed << matrizOut[columna3*fila3 - 1] << "}\n";

  std::cout << endl;

  if (compare(resultado, output, columna3*fila3) == true){
    std::cout << "Multiplicacion Correcta" << endl;
  } else {
    std::cout << "Multiplicacion Incorrecta" << endl;
  }



}
	
	



//

  /*Esto es AS de gnu.  Está en una hilera RAW (Por eso tiene una R, para no tener q poner todas las comillas)
  los parámetros están en: rdi, rsi, rdx, rcx, r8, r9 en ese orden.  
  OJO QUE SI SE ENVÍA UN ARREGLO, LO QUE SE ENVÍA ES UN PUNTERO AL PRIMER ELEMENTO.  Si se enviaran floats estarían en los registros xmm directamente.*/

asm(R"( .global addf        #define el nombre de la función
        .text
  quickMatrixMul: 
                            #n=fila_matiz1, m=columna_matriz1 , p=columnas_matriz2
                            #en rcx esta n
                            #en r8 esta m 
                            #en r9 esta p

    mov  %r8, %r10           #copio el valor m en r10                 
    mov  %r9, %r11           #copio el valor p en r11    
                 
    cmp  $0,  %rcx           #si n viene en 0 no haga nada
    je   fin  

  next:
    movups (%rdi), %xmm0    #carga los 4 primeros flotantes de vector a
    movups (%rsi), %xmm1    #carga los 4 primeros flotantes de vector b
    mulps  %xmm1, %xmm0     #mul 4 flotantes de los vectors a y b
    movups %xmm0, (%rdx)    #guarda los 4 flotantes en vector c 
    
    add  $16, %rdi          #se mueve 4 posiciones en vector a        
    add  $16, %rsi          #se mueve 4 posiciones en vector b       
    add  $16, %rdx          #se mueve 4 posiciones en vector c  
    sub  $4,  %r10          #resto 4 valores a la fila 
    cmp  $0,  %r10          #si aun hay valores en la fila repito    
    jnz  next
  
  reiniciar:
    sub $16, %rdi           #retrocede 4 posiciones en vector a
    add $4,  %r10           #agrega  4 valores a la columna
    cmp %r8, %r10           #compara si ya es igual al original si no repite
    jne reiniciar  
    sub $1,  %r11
    cmp $0,  %r11           #verifica si aun hay columnas y repite
    jnz next   
    
  reiniciar2:               #reinicia valores para poder pasar a la siguiente fila de a
    add $1,  %r11  
    add  $16, %rdi         #aumenta  p en un 1 
    cmp %r9, %r11    #compara si ya es igual al original si no respite
    jne reiniciar2
   
reiniciar3:
    sub %r8, %rsi           #retrocede 4 posiciones en vector b 
    sub $1 ,%r10
    cmp $0,%r10
    jnz reiniciar3
   
reiniciar4:
    add $1,%r10
    cmp  %r8,%r10
    jne reiniciar4  
    sub $1,  %rcx           #disminuye  n en un 1
    cmp $0,  %rcx           #compara si ya es igual a 0 si no respite
    jnz next
                 
  fin:
    ret   

  )"
        
);
