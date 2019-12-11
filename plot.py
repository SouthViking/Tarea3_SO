import matplotlib.pyplot as plt
import numpy as np
import random
import math

def truncate(number, digits) -> float:
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

def generate_plot(dims,h):
    xvals = np.array([])
    yvals = np.array([])

    for i in range(len(dims)):
        file = open(str(dims[i]),"r")
        prom = 0.0
        n = 0
        for cpu_time in file:
            prom += float(cpu_time)
            n += 1
        prom = float(prom)/float(n)
        xvals = np.append(xvals,int(dims[i]))
        yvals = np.append(yvals,truncate(prom,3))
    
    print(xvals)
    print(yvals)

    plt.grid()
    plt.title("Tiempos de ejecución para cálculos de matrices de gran tamaño con algoritmo secuencial")
    plt.xlabel("Dimensión de la matriz")
    plt.ylabel("Tiempo de ejecución promedio (segs)")
    plt.plot(xvals,yvals,color="green",marker="o",markeredgecolor="red")
    for x,y in zip(xvals,yvals):
        label = "{:.2f}".format(y)
        plt.annotate(label, # this is the text
                 (x,y), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center 
    plt.xticks(xvals)
    plt.show()

if __name__ == "__main__":
    dims = []
    n = input("Cantidad de dimensiones calculadas: ")
    h = input("Tamaño de cada intervalo: ")
    init = input("Dimensión inicial: ")
    for i in range(int(n)):
        dims.append(int(init)+i*int(h))
        
    generate_plot(dims,h)
