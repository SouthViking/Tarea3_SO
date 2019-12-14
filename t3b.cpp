#include <iostream>
#include <omp.h>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <random>
#include <tuple>
#include <fstream>
#define PRINT_LIM 20 // Dimensión límite para imprimir por terminal
using namespace std;
// Firmas de funciones
void print_matrix(vector<vector<double>> &matrix);
int get_notzerovalue(vector<tuple<double,int>> &list);
void rowchange(vector<vector<double>> &matrix,int rowA,int rowB);

void Gaussian_elimination(vector<vector<double>> &matrix,bool piv){ // SIN PIVOTEO
    for(int k = 0 ; k < matrix.size()-1 ; k++){
        
        bool change = false; // Indica si en la fila actual se está dividiendo por 0 (A[k][k])
        int zeros = 0; // Cuenta cantidad de ceros para la columna extraída
        vector<tuple <double,int>> column; // Vector columna para extraer la columna en la iteración K

        for(int v = k ; v < matrix.size() ; v++){
            if(!matrix[v][k]){
                zeros++;
            }
            if(piv){ // Si se realiza el algoritmo con pivteo, se extraen los valores de la columna
                column.push_back(make_tuple(matrix[v][k],v));
            }
        }
        if(zeros == matrix.size()-k){
            cout << "Columna de 0s detectada - Determinante 0" << endl;
            exit(0);
        }
        if(matrix[k][k] == 0.0){ // Se debe realizar pivoteo
            change = true;
        }

        if(piv && change){
            int swap_row = get_notzerovalue(column);
            if(swap_row >= k){
                rowchange(matrix,swap_row,k);
            }
        }
            #pragma omp parallel for
            for(int i = k+1 ; i < matrix.size() ; i++){
                double m = (double)((double)matrix[i][k]/(double)matrix[k][k]);
                #pragma omp parallel for
                for(int j = 0 ; j < matrix.size() ; j++){
                    matrix[i][j] = matrix[i][j]-m*matrix[k][j];
                }
            }
    }
    if(matrix.size() <= PRINT_LIM){
        print_matrix(matrix);
    }
}

void rowchange(vector<vector<double>> &matrix,int swapped,int current_row){
    #pragma omp parallel for
    for(int j = 0 ; j < matrix.size() ; j++){
        double aux = matrix[current_row][j]*-1;
        matrix[current_row][j] = matrix[swapped][j];
        matrix[swapped][j] = aux;
    }
}

int get_notzerovalue(vector<tuple<double,int>> &list){ // Retorna valor no nulo de la columna extraída para hacer swap
    for(int i = 0 ; i < list.size() ; i++){
        if(get<0>(list[i]) != 0.0){
            return get<1>(list[i]);
        }
    }
}

void generate_matrix(vector<vector<double>> &matrix,int lim_inf,int lim_sup){
    default_random_engine generator;
    uniform_int_distribution<int> distribution(lim_inf,lim_sup);

    #pragma omp parallel for collapse(2)
    for(int i = 0 ; i < matrix.size() ; i++){
        for(int j = 0 ; j < matrix.size() ; j++){
            matrix[i][j] = (double)distribution(generator);
        }
    }

}

double determinant_calculator(vector<vector<double>> &matrix){
    cout << endl << endl;
    vector<double> det_values;
    double det = 1;
    
    for(int i = 0 ; i < matrix.size() ; i++){
        if(matrix.size() < PRINT_LIM){
            cout << matrix[i][i] << " ";
        }
        det *= matrix[i][i];
    }
    if(matrix.size() < PRINT_LIM){
        cout << endl;
    }
    return det;
}

void print_matrix(vector<vector<double>> &matrix){
    cout << endl;
    for(int i = 0 ; i < matrix.size() ; i++){
        for(int j = 0 ; j < matrix.size() ; j++){
            cout << "" << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[]){

    long size;
    cout << "Tamaño de la matriz: ";
    cin >> size;

    if(size < 0){
        size = 1;
    }

    string filename = to_string(size);

    ofstream file;
    file.open(filename, ios::app | ios::out);

    vector<vector<double>> matrix(size, vector<double>(size));
    srand(time(NULL));
    
    auto start = chrono::high_resolution_clock::now();

    generate_matrix(matrix,-10,10);
    if(matrix.size() <= PRINT_LIM){
        print_matrix(matrix);
    }
    Gaussian_elimination(matrix,true);
    long double det = determinant_calculator(matrix);
    cout << "Determinante = " << det << endl;


    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start).count()/1000000.0;
    cout << "Tiempo tardado: " << duration << endl;
    file << duration << "\n";
    file.close();
    return 0;
}