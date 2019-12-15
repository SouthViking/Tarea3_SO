import matplotlib.pyplot as plt
import numpy as np
import math

def truncate(number, digits) -> float:
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

def generate_plot(first,h,length):
    xvalues = np.array([])
    for i in range(length):
        xvalues = np.append(xvalues,int(first+200*i))
    
    yvalues = np.zeros(shape=(2,length))
    strkey = "SP"

    for i in range(2):
        for j in range(length):
            file = open(str(int(first+200*j))+strkey[i],"r")
            prom = 0.0
            n = 0
            for value in file:
                prom += float(value)
                n+=1
            prom = prom/float(n)
            yvalues[i,j] = truncate(prom,3)
    
    plt.grid()
    plt.title("Comparación de tiempos de ejecución de algoritmo secuencial vs paralelo\n para cálculo de determinantes de matrices de gran tamaño")
    plt.xlabel("Dimensión N de la matriz")
    plt.ylabel("Tiempo de ejecución promedio (segs)")
    colors = ["green","blue"]
    for i in range(len(yvalues)):
        plt.plot(xvalues,yvalues[i],color=colors[i],marker="o",markeredgecolor="red")
    plt.legend(("Algoritmo secuencial","Algoritmo paralelo"),loc="upper left")
    for i in range(len(yvalues)):
        for x,y in zip(xvalues,yvalues[i]):
            label = "{:.2f}".format(y)
            plt.annotate(label, # this is the text
                (x,y), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center 
    plt.xticks(xvalues)
    plt.show()

if __name__ == "__main__":
    first = input("Dimensión inicial: ")
    while first.isnumeric() is not True:
        first = input("Dimensión inicial: ")
    first = int(first)
    
    h = input("Tamaño de intervalos: ")
    while h.isnumeric() is not True:
        h = input("Tamaño de intervalos")
    h = int(h)

    length = input("Cantidad de muestras: ")
    while length.isnumeric() is not True:
        length = input("Cantidad de muestras")
    length = int(length)

    generate_plot(first,h,length)