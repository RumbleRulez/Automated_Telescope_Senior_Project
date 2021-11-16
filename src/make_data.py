import csv

rows = []

def make():
    f = open(input("Source: "), "r")
    dest = open(input("Destination: "), "w")
    for x in f:
        rows = f.readline()
        data = rows[23:]
        #data.insert(10, ",")
        dest.write(data)
    dest.close()
    f.close()

make()