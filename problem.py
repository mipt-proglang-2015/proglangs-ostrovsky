import time
import sys
import csv
import floyd
import xml.dom.minidom
import numpy as np


def init_matrix (filename):
    data = xml.dom.minidom.parse(filename)
	
    net = data.getElementsByTagName('net')
    resistors = data.getElementsByTagName('resistor')
    capacitors = data.getElementsByTagName('capactor')
    diods = data.getElementsByTagName('diode')
    count_nets = len(net)
    matrix = []
    for i in range (count_nets):
        matrix.append ([0] * count_nets)
    
    scheme_edges = []
    for index, edge in enumerate(resistors + capacitors + diods):
        edge = edge.attributes
        net_from = int(edge['net_from'].value)-1
        net_to = int(edge['net_to'].value) - 1
        resistance = float(edge['resistance'].value)
        scheme_edges.append ((net_from, net_to, resistance))
        if index < len(resistors) + len(capacitors):
            scheme_edges.append ((net_to, net_from, resistance))
        else:
            reverse_resistance = float(edge['reverse_resistance'].value)
            scheme_edges.append ((net_to, net_from, reverse_resistance))
    for edge in scheme_edges:
        source = edge[0]
        dest = edge[1]
        resistance = edge[2]
        if matrix[source][dest]:
            matrix[source][dest] = (matrix[source][dest] * resistance) / (matrix[source][dest] + resistance)
        else:
            matrix[source][dest] = resistance
    return matrix

def write_results (matrix, filename):
	with open (filename, 'w') as data:
	    for row in matrix:
	        for element in row:
	            data.write (str(round(element, 7)) + ',')
	        data.write('\n')

def main ():
    if len(sys.argv) != 3:
        print ('Incorrect input format\n')
    else:
        start = time.process_time()
        inp_file = sys.argv[1]
        out_file = sys.argv[2]
        matrix = init_matrix (inp_file)
        matrix = floyd.FloydAlgorithm(matrix)
        write_results(matrix, out_file)
        end = time.process_time()
        print("Work time: {} sec".format((end-start)))

main()
